#pragma once

#include "exqudens/vulkan/model/CommandBuffer.hpp"

namespace exqudens::vulkan {

  class CommandBufferFactory {

    public:

      virtual CommandBuffer createCommandBuffer(VkDevice& device, VkCommandPool& commandPool) = 0;
      virtual std::vector<CommandBuffer> createCommandBuffers(
          VkDevice& device,
          VkCommandPool& commandPool,
          std::size_t size
      ) = 0;

      virtual void destroyCommandBuffer(CommandBuffer& commandBuffer) = 0;
      virtual void destroyCommandBuffers(std::vector<CommandBuffer>& commandBuffers) = 0;

  };

}
