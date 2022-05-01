#pragma once

#include "exqudens/vulkan/model/BufferCreateInfo.hpp"
#include "exqudens/vulkan/model/Buffer.hpp"

namespace exqudens::vulkan {

  class BufferFactory {

    public:

      virtual Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const BufferCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      ) = 0;
      virtual Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkDeviceSize memorySize,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties
      ) = 0;
      virtual std::vector<Buffer> createBuffers(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkDeviceSize memorySize,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties,
          std::size_t size
      ) = 0;

      virtual void destroyBuffer(Buffer& buffer) = 0;
      virtual void destroyBuffer(Buffer& buffer, bool unmapMemory) = 0;
      virtual void destroyBuffers(std::vector<Buffer>& buffers) = 0;
      virtual void destroyBuffers(std::vector<Buffer>& buffers, bool unmapMemory) = 0;

  };

}
