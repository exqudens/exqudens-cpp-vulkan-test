#pragma once

#include <cstring>

#include "exqudens/vulkan/Renderer.hpp"
#include "exqudens/vulkan/FunctionsProviderBase.hpp"

namespace exqudens::vulkan {

  class RendererBase:
      virtual public Renderer,
      virtual public FunctionsProviderBase
  {

    public:

      void copyToMemory(
          VkDevice& device,
          VkDeviceMemory& memory,
          VkDeviceSize offset,
          VkDeviceSize size,
          void* data
      ) override {
        try {
          copyToMemory(device, memory, offset, size, 0, data);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void copyToMemory(
          VkDevice& device,
          VkDeviceMemory& memory,
          VkDeviceSize offset,
          VkDeviceSize size,
          VkMemoryMapFlags flags,
          void* data
      ) override {
        try {
          void* dst;
          if (functions().mapMemory(device, memory, offset, size, flags, &dst) != VK_SUCCESS) {
            throw std::runtime_error(CALL_INFO() + ": failed to map memory!");
          }
          std::memcpy(dst, data, (std::size_t) size);
          functions().unmapMemory(device, memory);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void cmdCopyBuffer(
          VkCommandBuffer& commandBuffer,
          VkBuffer& srcBuffer,
          VkBuffer& dstBuffer,
          VkDeviceSize size
      ) override {
        try {
          VkBufferCopy copyRegion = {
              .size = size
          };

          functions().cmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void cmdCopyBufferToImage(
          VkCommandBuffer& commandBuffer,
          VkBuffer& buffer,
          VkImage& image,
          uint32_t width,
          uint32_t height
      ) override {
        try {
          cmdCopyBufferToImage(commandBuffer, buffer, image, width, height, 1);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void cmdCopyBufferToImage(
          VkCommandBuffer& commandBuffer,
          VkBuffer& buffer,
          VkImage& image,
          uint32_t width,
          uint32_t height,
          uint32_t depth
      ) override {
        try {
          VkBufferImageCopy region = {
              .bufferOffset = 0,
              .bufferRowLength = 0,
              .imageSubresource = VkImageSubresourceLayers {
                  .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                  .mipLevel = 0,
                  .baseArrayLayer = 0,
                  .layerCount = 1
              },
              .imageOffset = VkOffset3D {
                  .x = 0,
                  .y = 0,
                  .z = 0
              },
              .imageExtent = VkExtent3D {
                  .width = width,
                  .height = height,
                  .depth = depth
              }
          };

          functions().cmdCopyBufferToImage(
              commandBuffer,
              buffer,
              image,
              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
              1,
              &region
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void cmdPipelineBarrier(
          VkCommandBuffer& commandBuffer,
          VkImage& image,
          VkFormat format,
          VkImageLayout oldLayout,
          VkImageLayout newLayout
      ) override {
        try {
          VkImageMemoryBarrier barrier = {};
          barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
          barrier.oldLayout = oldLayout;
          barrier.newLayout = newLayout;
          barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
          barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
          barrier.image = image;
          barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
          barrier.subresourceRange.baseMipLevel = 0;
          barrier.subresourceRange.levelCount = 1;
          barrier.subresourceRange.baseArrayLayer = 0;
          barrier.subresourceRange.layerCount = 1;

          VkPipelineStageFlags sourceStage;
          VkPipelineStageFlags destinationStage;

          if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

            if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) {
              barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
          } else {
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
          }

          if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
          } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
          } else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
          } else {
            throw std::invalid_argument(CALL_INFO() + ": unsupported layout transition!");
          }

          functions().cmdPipelineBarrier(
              commandBuffer,
              sourceStage, destinationStage,
              0,
              0, nullptr,
              0, nullptr,
              1, &barrier
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
