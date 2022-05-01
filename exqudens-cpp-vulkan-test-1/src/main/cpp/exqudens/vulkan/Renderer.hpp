#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  class Renderer {

    public:

      virtual void copyToMemory(
          VkDevice& device,
          VkDeviceMemory& memory,
          VkDeviceSize offset,
          VkDeviceSize size,
          void* data
      ) = 0;

      virtual void copyToMemory(
          VkDevice& device,
          VkDeviceMemory& memory,
          VkDeviceSize offset,
          VkDeviceSize size,
          VkMemoryMapFlags flags,
          void* data
      ) = 0;

      virtual void cmdCopyBuffer(
          VkCommandBuffer& commandBuffer,
          VkBuffer& srcBuffer,
          VkBuffer& dstBuffer,
          VkDeviceSize size
      ) = 0;

      virtual void cmdCopyBufferToImage(
          VkCommandBuffer& commandBuffer,
          VkBuffer& buffer,
          VkImage& image,
          uint32_t width,
          uint32_t height
      ) = 0;

      virtual void cmdCopyBufferToImage(
          VkCommandBuffer& commandBuffer,
          VkBuffer& buffer,
          VkImage& image,
          uint32_t width,
          uint32_t height,
          uint32_t depth
      ) = 0;

      virtual void cmdPipelineBarrier(
          VkCommandBuffer& commandBuffer,
          VkImage& image,
          VkFormat format,
          VkImageLayout oldLayout,
          VkImageLayout newLayout
      ) = 0;

  };

}
