#pragma once

#include <map>

#include "exqudens/vulkan/Context.hpp"
#include "exqudens/vulkan/FactoryBase.hpp"

namespace exqudens::vulkan {

  class ContextBase:
      virtual public Context,
      virtual public FactoryBase
  {

    protected:

      unsigned int instanceId = 0;
      unsigned int debugUtilsMessengerId = 0;
      unsigned int physicalDeviceId = 0;
      unsigned int deviceId = 0;
      unsigned int bufferId = 0;
      unsigned int imageId = 0;
      unsigned int imageViewId = 0;
      unsigned int samplerId = 0;
      unsigned int renderPassId = 0;
      unsigned int descriptorSetLayoutId = 0;
      unsigned int descriptorPoolId = 0;
      unsigned int descriptorSetId = 0;
      unsigned int pipelineId = 0;
      unsigned int frameBufferId = 0;
      unsigned int surfaceId = 0;
      unsigned int swapChainId = 0;
      unsigned int queueId = 0;
      unsigned int commandPoolId = 0;
      unsigned int commandBufferId = 0;
      unsigned int semaphoreId = 0;
      unsigned int fenceId = 0;

      std::map<unsigned int, Instance> instances = {};
      std::map<unsigned int, DebugUtilsMessenger> debugUtilsMessengers = {};
      std::map<unsigned int, PhysicalDevice> physicalDevices = {};
      std::map<unsigned int, Device> devices = {};
      std::map<unsigned int, Buffer> buffers = {};
      std::map<unsigned int, Image> images = {};
      std::map<unsigned int, ImageView> imageViews = {};
      std::map<unsigned int, Sampler> samplers = {};
      std::map<unsigned int, RenderPass> renderPasses = {};
      std::map<unsigned int, DescriptorSetLayout> descriptorSetLayouts = {};
      std::map<unsigned int, DescriptorPool> descriptorPools = {};
      std::map<unsigned int, DescriptorSet> descriptorSets = {};
      std::map<unsigned int, Pipeline> pipelines = {};
      std::map<unsigned int, FrameBuffer> frameBuffers = {};
      std::map<unsigned int, Surface> surfaces = {};
      std::map<unsigned int, SwapChain> swapChains = {};
      std::map<unsigned int, Queue> queues = {};
      std::map<unsigned int, CommandPool> commandPools = {};
      std::map<unsigned int, CommandBuffer> commandBuffers = {};
      std::map<unsigned int, Semaphore> semaphores = {};
      std::map<unsigned int, Fence> fences = {};

    public:

      // create

      Instance createInstance(Configuration& configuration, Logger& logger) override {
        try {
          unsigned int key = instanceId++;
          Instance value = InstanceFactoryBase::createInstance(
              configuration,
              logger
          );
          value.id = key;
          value.destroyed = false;
          instances[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      DebugUtilsMessenger createDebugUtilsMessenger(VkInstance& instance, Logger& logger) override {
        try {
          unsigned int key = debugUtilsMessengerId++;
          DebugUtilsMessenger value = DebugUtilsMessengerFactoryBase::createDebugUtilsMessenger(
              instance,
              logger
          );
          value.id = key;
          value.destroyed = false;
          debugUtilsMessengers[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      PhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration,
          const VkSurfaceKHR& surface
      ) override {
        try {
          unsigned int key = physicalDeviceId++;
          PhysicalDevice value = PhysicalDeviceFactoryBase::createPhysicalDevice(
              instance,
              configuration,
              surface
          );
          value.id = key;
          value.destroyed = false;
          physicalDevices[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Device createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          QueueFamilyIndexInfo& queueFamilyIndexInfo
      ) override {
        try {
          unsigned int key = deviceId++;
          Device value = DeviceFactoryBase::createDevice(
              physicalDevice,
              configuration,
              queueFamilyIndexInfo
          );
          value.id = key;
          value.destroyed = false;
          devices[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const BufferCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      ) override {
        try {
          unsigned int key = bufferId++;
          Buffer value = BufferFactoryBase::createBuffer(
              physicalDevice,
              device,
              createInfo,
              properties
          );
          value.id = key;
          value.destroyed = false;
          buffers[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Image createImage(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const ImageCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      ) override {
        try {
          unsigned int key = imageId++;
          Image value = ImageFactoryBase::createImage(
              physicalDevice,
              device,
              createInfo,
              properties
          );
          value.id = key;
          value.destroyed = false;
          images[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      ImageView createImageView(
          VkDevice& device,
          VkImage& image,
          VkFormat format,
          VkImageAspectFlags aspectMask
      ) override {
        try {
          unsigned int key = imageViewId++;
          ImageView value = ImageViewFactoryBase::createImageView(
              device,
              image,
              format,
              aspectMask
          );
          value.id = key;
          value.destroyed = false;
          imageViews[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Sampler createSampler(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          bool anisotropyEnable
      ) override {
        try {
          unsigned int key = samplerId++;
          Sampler value = SamplerFactoryBase::createSampler(
              physicalDevice,
              device,
              anisotropyEnable
          );
          value.id = key;
          value.destroyed = false;
          samplers[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      RenderPass createRenderPass(
          VkDevice& device,
          const RenderPassCreateInfo& createInfo
      ) override {
        try {
          unsigned int key = renderPassId++;
          RenderPass value = RenderPassFactoryBase::createRenderPass(
              device,
              createInfo
          );
          value.id = key;
          value.destroyed = false;
          renderPasses[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      DescriptorSetLayout createDescriptorSetLayout(
          VkDevice& device,
          const DescriptorSetLayoutCreateInfo& createInfo
      ) override {
        try {
          unsigned int key = descriptorSetLayoutId++;
          DescriptorSetLayout value = DescriptorSetLayoutFactoryBase::createDescriptorSetLayout(
              device,
              createInfo
          );
          value.id = key;
          value.destroyed = false;
          descriptorSetLayouts[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      DescriptorPool createDescriptorPool(
          VkDevice& device,
          const DescriptorPoolCreateInfo& createInfo
      ) override {
        try {
          unsigned int key = descriptorPoolId++;
          DescriptorPool value = DescriptorPoolFactoryBase::createDescriptorPool(
              device,
              createInfo
          );
          value.id = key;
          value.destroyed = false;
          descriptorPools[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      DescriptorSet createDescriptorSet(
          VkDevice& device,
          VkDescriptorPool& descriptorPool,
          VkDescriptorSetLayout& descriptorSetLayout,
          const std::vector<WriteDescriptorSet>& writeDescriptorSets
      ) override {
        try {
          unsigned int key = descriptorSetId++;
          DescriptorSet value = DescriptorSetFactoryBase::createDescriptorSet(
              device,
              descriptorPool,
              descriptorSetLayout,
              writeDescriptorSets
          );
          value.id = key;
          value.destroyed = false;
          descriptorSets[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Pipeline createPipeline(
          VkDevice& device,
          const std::vector<std::string>& shaderPaths,
          const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions,
          const std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions,
          const PipelineLayoutCreateInfo& layoutCreateInfo,
          const GraphicsPipelineCreateInfo& createInfo
      ) override {
        try {
          unsigned int key = pipelineId++;
          Pipeline value = PipelineFactoryBase::createPipeline(
              device,
              shaderPaths,
              vertexBindingDescriptions,
              vertexAttributeDescriptions,
              layoutCreateInfo,
              createInfo
          );
          value.id = key;
          value.destroyed = false;
          pipelines[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      FrameBuffer createFrameBuffer(
          VkDevice& device,
          const FrameBufferCreateInfo& createInfo
      ) override {
        try {
          unsigned int key = frameBufferId++;
          FrameBuffer value = FrameBufferFactoryBase::createFrameBuffer(
              device,
              createInfo
          );
          value.id = key;
          value.destroyed = false;
          frameBuffers[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Surface createSurface(
          VkInstance& instance
      ) override {
        try {
          unsigned int key = surfaceId++;
          Surface value = SurfaceFactoryBase::createSurface(
                  instance
          );
          value.id = key;
          value.destroyed = false;
          surfaces[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      SwapChain createSwapChain(
          SwapChainSupportDetails& swapChainSupport,
          QueueFamilyIndexInfo& queueFamilyIndexInfo,
          VkSurfaceKHR& surface,
          VkDevice& device,
          const uint32_t& width,
          const uint32_t& height
      ) override {
        try {
          unsigned int key = swapChainId++;
          SwapChain value = SwapChainFactoryBase::createSwapChain(
              swapChainSupport,
              queueFamilyIndexInfo,
              surface,
              device,
              width,
              height
          );
          value.id = key;
          value.destroyed = false;
          swapChains[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Queue createQueue(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          uint32_t queueIndex
      ) override {
        try {
          unsigned int key = queueId++;
          Queue value = QueueFactoryBase::createQueue(
              device,
              queueFamilyIndex,
              queueIndex
          );
          value.id = key;
          value.destroyed = false;
          queues[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      CommandPool createCommandPool(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          VkCommandPoolCreateFlags flags
      ) override {
        try {
          unsigned int key = commandPoolId++;
          CommandPool value = CommandPoolFactoryBase::createCommandPool(
              device,
              queueFamilyIndex,
              flags
          );
          value.id = key;
          value.destroyed = false;
          commandPools[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      CommandBuffer createCommandBuffer(
          VkDevice& device,
          VkCommandPool& commandPool
      ) override {
        try {
          unsigned int key = commandBufferId++;
          CommandBuffer value = CommandBufferFactoryBase::createCommandBuffer(
              device,
              commandPool
          );
          value.id = key;
          value.destroyed = false;
          commandBuffers[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Semaphore createSemaphore(
          VkDevice& device,
          VkSemaphoreCreateFlags flags
      ) override {
        try {
          unsigned int key = semaphoreId++;
          Semaphore value = SemaphoreFactoryBase::createSemaphore(
              device,
              flags
          );
          value.id = key;
          value.destroyed = false;
          semaphores[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Fence createFence(
          VkDevice& device,
          VkFenceCreateFlags flags
      ) override {
        try {
          unsigned int key = fenceId++;
          Fence value = FenceFactoryBase::createFence(
              device,
              flags
          );
          value.id = key;
          value.destroyed = false;
          fences[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      // destroy

      void destroyInstance(Instance& instance) override {
        try {
          InstanceFactoryBase::destroyInstance(instance);
          instances[instance.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDebugUtilsMessenger(DebugUtilsMessenger& debugUtilsMessenger) override {
        try {
          DebugUtilsMessengerFactoryBase::destroyDebugUtilsMessenger(debugUtilsMessenger);
          debugUtilsMessengers[debugUtilsMessenger.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyPhysicalDevice(PhysicalDevice& physicalDevice) override {
        try {
          PhysicalDeviceFactoryBase::destroyPhysicalDevice(physicalDevice);
          physicalDevices[physicalDevice.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDevice(Device& device) override {
        try {
          DeviceFactoryBase::destroyDevice(device);
          devices[device.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyBuffer(Buffer& buffer) override {
        try {
          BufferFactoryBase::destroyBuffer(buffer);
          buffers[buffer.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImage(Image& image) override {
        try {
          ImageFactoryBase::destroyImage(image);
          images[image.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImageView(ImageView& imageView) override {
        try {
          ImageViewFactoryBase::destroyImageView(imageView);
          imageViews[imageView.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySampler(Sampler& sampler) override {
        try {
          SamplerFactoryBase::destroySampler(sampler);
          samplers[sampler.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyRenderPass(RenderPass& renderPass) override {
        try {
          RenderPassFactoryBase::destroyRenderPass(renderPass);
          renderPasses[renderPass.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDescriptorSetLayout(DescriptorSetLayout& descriptorSetLayout) override {
        try {
          DescriptorSetLayoutFactoryBase::destroyDescriptorSetLayout(descriptorSetLayout);
          descriptorSetLayouts[descriptorSetLayout.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDescriptorPool(DescriptorPool& descriptorPool) override {
        try {
          DescriptorPoolFactoryBase::destroyDescriptorPool(descriptorPool);
          descriptorPools[descriptorPool.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDescriptorSet(DescriptorSet& descriptorSet) override {
        try {
          DescriptorSetFactoryBase::destroyDescriptorSet(descriptorSet);
          descriptorSets[descriptorSet.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyPipeline(Pipeline& pipeline) override {
        try {
          PipelineFactoryBase::destroyPipeline(pipeline);
          pipelines[pipeline.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyFrameBuffer(FrameBuffer& frameBuffer) override {
        try {
          FrameBufferFactoryBase::destroyFrameBuffer(frameBuffer);
          frameBuffers[frameBuffer.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySurface(Surface& surface) override {
        try {
          SurfaceFactoryBase::destroySurface(surface);
          surfaces[surface.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySwapChain(SwapChain& swapChain) override {
        try {
          SwapChainFactoryBase::destroySwapChain(swapChain);
          swapChains[swapChain.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyQueue(Queue& queue) override {
        try {
          QueueFactoryBase::destroyQueue(queue);
          queues[queue.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyCommandPool(CommandPool& commandPool) override {
        try {
          CommandPoolFactoryBase::destroyCommandPool(commandPool);
          commandPools[commandPool.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyCommandBuffer(CommandBuffer& commandBuffer) override {
        try {
          CommandBufferFactoryBase::destroyCommandBuffer(commandBuffer);
          commandBuffers[commandBuffer.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySemaphore(Semaphore& semaphore) override {
        try {
          SemaphoreFactoryBase::destroySemaphore(semaphore);
          semaphores[semaphore.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyFence(Fence& fence) override {
        try {
          FenceFactoryBase::destroyFence(fence);
          fences[fence.id].destroyed = true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Surface add(const Surface& surface) override {
        try {
          unsigned int key = surfaceId++;
          Surface value = surface;
          value.id = key;
          value.destroyed = false;
          surfaces[key] = value;
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroy() override {
        try {
          // destroy fences
          for (auto& [key, value] : fences) {
            if (!value.destroyed) destroyFence(value);
          }
          fences.clear();

          // destroy semaphores
          for (auto& [key, value] : semaphores) {
            if (!value.destroyed) destroySemaphore(value);
          }
          semaphores.clear();

          // destroy commandBuffers
          for (auto& [key, value] : commandBuffers) {
            if (!value.destroyed) destroyCommandBuffer(value);
          }
          commandBuffers.clear();

          // destroy commandPools
          for (auto& [key, value] : commandPools) {
            if (!value.destroyed) destroyCommandPool(value);
          }
          commandPools.clear();

          // destroy queues
          for (auto& [key, value] : queues) {
            if (!value.destroyed) destroyQueue(value);
          }
          queues.clear();

          // destroy swapChains
          for (auto& [key, value] : swapChains) {
            if (!value.destroyed) destroySwapChain(value);
          }
          swapChains.clear();

          // destroy surfaces
          for (auto& [key, value] : surfaces) {
            if (!value.destroyed) destroySurface(value);
          }
          surfaces.clear();

          // destroy frameBuffers
          for (auto& [key, value] : frameBuffers) {
            if (!value.destroyed) destroyFrameBuffer(value);
          }
          frameBuffers.clear();

          // destroy pipelines
          for (auto& [key, value] : pipelines) {
            if (!value.destroyed) destroyPipeline(value);
          }
          pipelines.clear();

          // destroy descriptorSets
          for (auto& [key, value] : descriptorSets) {
            if (!value.destroyed) destroyDescriptorSet(value);
          }
          descriptorSets.clear();

          // destroy descriptorPools
          for (auto& [key, value] : descriptorPools) {
            if (!value.destroyed) destroyDescriptorPool(value);
          }
          descriptorPools.clear();

          // destroy descriptorSetLayouts
          for (auto& [key, value] : descriptorSetLayouts) {
            if (!value.destroyed) destroyDescriptorSetLayout(value);
          }
          descriptorSetLayouts.clear();

          // destroy renderPasses
          for (auto& [key, value] : renderPasses) {
            if (!value.destroyed) destroyRenderPass(value);
          }
          renderPasses.clear();

          // destroy samplers
          for (auto& [key, value] : samplers) {
            if (!value.destroyed) destroySampler(value);
          }
          samplers.clear();

          // destroy imageViews
          for (auto& [key, value] : imageViews) {
            if (!value.destroyed) destroyImageView(value);
          }
          imageViews.clear();

          // destroy images
          for (auto& [key, value] : images) {
            if (!value.destroyed) destroyImage(value);
          }
          images.clear();

          // destroy buffers
          for (auto& [key, value] : buffers) {
            if (!value.destroyed) destroyBuffer(value);
          }
          buffers.clear();

          // destroy devices
          for (auto& [key, value] : devices) {
            if (!value.destroyed) destroyDevice(value);
          }
          devices.clear();

          // destroy physicalDevices
          for (auto& [key, value] : physicalDevices) {
            if (!value.destroyed) destroyPhysicalDevice(value);
          }
          physicalDevices.clear();

          // destroy debugUtilsMessengers
          for (auto& [key, value] : debugUtilsMessengers) {
            if (!value.destroyed) destroyDebugUtilsMessenger(value);
          }
          debugUtilsMessengers.clear();

          // destroy instances
          for (auto& [key, value] : instances) {
            if (!value.destroyed) destroyInstance(value);
          }
          instances.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
