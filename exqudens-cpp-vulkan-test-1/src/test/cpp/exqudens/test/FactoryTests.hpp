#pragma once

#include <stdexcept>

#include <gtest/gtest.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/FactoryBase.hpp"

namespace exqudens::vulkan {

  class FactoryTests : public testing::Test, protected FactoryBase {

    protected:

      void submitBlocking(VkDevice& device, VkCommandBuffer& cmdBuffer, VkQueue& queue) {
        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &cmdBuffer;
        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VkFence fence = nullptr;

        if (vkCreateFence(device, &fenceInfo, nullptr, &fence) != VkResult::VK_SUCCESS || fence == nullptr) {
          throw std::runtime_error(CALL_INFO() + ": failed to create fence!");
        }

        vkResetFences(device, 1, &fence);

        if (vkQueueSubmit(queue, 1, &submitInfo, fence) != VkResult::VK_SUCCESS) {
          throw std::runtime_error(CALL_INFO() + ": failed to submit queue!");
        }

        if (vkWaitForFences(device, 1, &fence, VK_TRUE, UINT64_MAX) != VkResult::VK_SUCCESS) {
          throw std::runtime_error(CALL_INFO() + ": failed to wait for fences!");
        }

        vkDestroyFence(device, fence, nullptr);
      }

  };

  TEST_F(FactoryTests, test1) {
    try {
      std::map<std::string, std::string> environmentVariables = createEnvironmentVariables(TestUtils::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        setEnvironmentVariable(name, value);
      }

      Configuration configuration = createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};
      std::ostringstream stream;
      Logger logger = createLogger(stream);

      Instance instance = createInstance(configuration, logger);
      DebugUtilsMessenger debugUtilsMessenger = createDebugUtilsMessenger(instance.value, logger);
      PhysicalDevice physicalDevice = createPhysicalDevice(instance.value, configuration);
      Device device = createDevice(physicalDevice.value, configuration, physicalDevice.queueFamilyIndexInfo);
      Queue graphicsQueue = createQueue(device.value, physicalDevice.queueFamilyIndexInfo.graphicsFamily.value(), 0);
      CommandPool graphicsCommandPool = createCommandPool(device.value, graphicsQueue.familyIndex);
      Image image = createImage(physicalDevice.value, device.value, 800, 600, VkFormat::VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      ImageView imageView = createImageView(device.value, image.value, image.format);
      RenderPass renderPass = createRenderPass(
          device.value,
          RenderPassCreateInfo {
              .attachments = {
                  VkAttachmentDescription {
                      .format = image.format,
                      .samples = VK_SAMPLE_COUNT_1_BIT,
                      .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                      .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                      .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                      .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                      .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                      .finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
                  }
              },
              .subPasses = {
                  SubPassDescription {
                      .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                      .colorAttachments = {
                          VkAttachmentReference {
                              .attachment = 0,
                              .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                          }
                      }
                  }
              },
              .dependencies = {
                  VkSubpassDependency {
                      .srcSubpass = VK_SUBPASS_EXTERNAL,
                      .dstSubpass = 0,
                      .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                      .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                      .srcAccessMask = 0,
                      .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
                  }
              }
          }
      );
      FrameBuffer frameBuffer = createFrameBuffer(
          device.value,
          FrameBufferCreateInfo {
            .flags = 0,
            .renderPass = renderPass.value,
            .attachments = {imageView.value},
            .width = image.width,
            .height = image.height,
            .layers = 1
          }
      );
      Pipeline graphicsPipeline = createGraphicsPipeline(
          device.value,
          {.width = image.width, .height = image.height},
          {"resources/shader/shader-1.vert.spv", "resources/shader/shader-1.frag.spv"},
          renderPass.value
      );
      CommandBuffer graphicsCommandBuffer = createCommandBuffer(device.value, graphicsCommandPool.value);
      Image imageOut = createImage(
          physicalDevice.value,
          device.value,
          800,
          600,
          image.format,
          VK_IMAGE_TILING_LINEAR,
          VK_IMAGE_USAGE_TRANSFER_DST_BIT,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
      );
      CommandBuffer copyCommandBuffer = createCommandBuffer(device.value, graphicsCommandPool.value);

      VkCommandBufferBeginInfo beginInfo = {};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      if (vkBeginCommandBuffer(graphicsCommandBuffer.value, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to begin graphics command buffer!");
      }

      VkRenderPassBeginInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      renderPassInfo.renderPass = renderPass.value;
      renderPassInfo.framebuffer = frameBuffer.value;
      renderPassInfo.renderArea.offset = {0, 0};
      renderPassInfo.renderArea.extent = {.width = image.width, .height = image.height};

      VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
      renderPassInfo.clearValueCount = 1;
      renderPassInfo.pClearValues = &clearColor;

      vkCmdBeginRenderPass(graphicsCommandBuffer.value, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      vkCmdBindPipeline(graphicsCommandBuffer.value, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.value);

      vkCmdDraw(graphicsCommandBuffer.value, 3, 1, 0, 0);

      vkCmdEndRenderPass(graphicsCommandBuffer.value);

      if (vkEndCommandBuffer(graphicsCommandBuffer.value) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to end graphics command buffer!");
      }

      submitBlocking(device.value, graphicsCommandBuffer.value, graphicsQueue.value);
      vkDeviceWaitIdle(device.value);

      /*VkImageSubresourceRange subresourceRange = {};
      subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      subresourceRange.baseMipLevel = 0;
      subresourceRange.levelCount = 1;
      subresourceRange.baseArrayLayer = 0;
      subresourceRange.layerCount = 1;

      VkImageMemoryBarrier memoryBarrier = {};
      memoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      memoryBarrier.image = imageOut.value;
      memoryBarrier.srcAccessMask = 0;
      memoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      memoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      memoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      memoryBarrier.subresourceRange = subresourceRange;*/

      if (vkBeginCommandBuffer(copyCommandBuffer.value, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to begin copy command buffer!");
      }

      VkImageMemoryBarrier imageMemoryBarrier1 = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
          .srcAccessMask = 0,
          .dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
          .oldLayout = VK_IMAGE_LAYOUT_UNDEFINED,
          .newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          .image = imageOut.value,
          .subresourceRange = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1
          }
      };

      vkCmdPipelineBarrier(
          copyCommandBuffer.value,
          VK_PIPELINE_STAGE_TRANSFER_BIT,
          VK_PIPELINE_STAGE_TRANSFER_BIT,
          0,
          0,
          nullptr,
          0,
          nullptr,
          1,
          &imageMemoryBarrier1
      );

      VkImageCopy imageCopyRegion = {};
      imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.srcSubresource.layerCount = 1;
      imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.dstSubresource.layerCount = 1;
      imageCopyRegion.extent.width = image.width;
      imageCopyRegion.extent.height = image.height;
      imageCopyRegion.extent.depth = 1;

      vkCmdCopyImage(
          copyCommandBuffer.value,
          image.value,
          VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
          imageOut.value,
          VkImageLayout::VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          1,
          &imageCopyRegion
      );

      VkImageMemoryBarrier imageMemoryBarrier2 = {
          .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
          .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
          .dstAccessMask = VK_ACCESS_MEMORY_READ_BIT,
          .oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
          .newLayout = VK_IMAGE_LAYOUT_GENERAL,
          .image = imageOut.value,
          .subresourceRange = {
              .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
              .baseMipLevel = 0,
              .levelCount = 1,
              .baseArrayLayer = 0,
              .layerCount = 1
          }
      };

      vkCmdPipelineBarrier(
          copyCommandBuffer.value,
          VK_PIPELINE_STAGE_TRANSFER_BIT,
          VK_PIPELINE_STAGE_TRANSFER_BIT,
          0,
          0,
          nullptr,
          0,
          nullptr,
          1,
          &imageMemoryBarrier2
      );

      if (vkEndCommandBuffer(copyCommandBuffer.value) != VK_SUCCESS) {
        throw std::runtime_error(CALL_INFO() + ": failed to end copy command buffer!!");
      }

      submitBlocking(device.value, copyCommandBuffer.value, graphicsQueue.value);

      ///

      VkImageSubresource subResource = {
          .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT
      };
      VkSubresourceLayout subResourceLayout;

      vkGetImageSubresourceLayout(device.value, imageOut.value, &subResource, &subResourceLayout);

      unsigned char* imageData;
      vkMapMemory(device.value, imageOut.memory, 0, imageOut.memorySize, 0, (void**) &imageData);
      imageData += subResourceLayout.offset;

      //////
      std::vector<std::vector<std::vector<unsigned char>>> imageDataVector;
      std::size_t depth = 4;
      imageDataVector.resize(imageOut.height);
      for (uint32_t y = 0; y < imageOut.height; y++) {
        imageDataVector[y].resize(imageOut.width);
        for (uint32_t x = 0; x < imageOut.width; x++) {
          imageDataVector[y][x].resize(depth);
          for (uint32_t z = 0; z < depth; z++) {
            std::size_t yOffset = y * subResourceLayout.rowPitch;
            std::size_t xOffset = x * depth;
            std::size_t zOffset = z;
            std::size_t offset = yOffset + xOffset + zOffset;
            imageDataVector[y][x][z] = imageData[offset];
          }
        }
      }
      TestUtils::writePng(
          imageDataVector,
          std::filesystem::path(TestUtils::getExecutableDir())
              .append("resources")
              .append("png")
              .append("core-tests-test3-result.png")
              .make_preferred()
              .string()
      );
      //////

      vkUnmapMemory(device.value, imageOut.memory);

      ///

      vkQueueWaitIdle(graphicsQueue.value);

      destroyCommandBuffer(copyCommandBuffer);
      destroyImage(imageOut);
      destroyCommandBuffer(graphicsCommandBuffer);
      destroyPipeline(graphicsPipeline);
      destroyFrameBuffer(frameBuffer);
      destroyRenderPass(renderPass);
      destroyImageView(imageView);
      destroyImage(image);
      destroyCommandPool(graphicsCommandPool);
      destroyQueue(graphicsQueue);
      destroyDevice(device);
      destroyPhysicalDevice(physicalDevice);
      destroyDebugUtilsMessenger(debugUtilsMessenger);
      destroyInstance(instance);

      std::cout << stream.str();

      std::vector<std::vector<std::vector<unsigned char>>> image1 = TestUtils::readPng(
          std::filesystem::path(TestUtils::getExecutableDir())
              .append("resources")
              .append("png")
              .append("core-tests-test3-expected.png")
              .make_preferred()
              .string()
      );
      std::vector<std::vector<std::vector<unsigned char>>> image2 = TestUtils::readPng(
          std::filesystem::path(TestUtils::getExecutableDir())
              .append("resources")
              .append("png")
              .append("core-tests-test3-result.png")
              .make_preferred()
              .string()
      );
      ASSERT_EQ(image1.size(), image2.size());
      ASSERT_EQ(image1[0].size(), image2[0].size());
      ASSERT_EQ(image1[0][0].size(), image2[0][0].size());
      ASSERT_EQ(image1, image2);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
