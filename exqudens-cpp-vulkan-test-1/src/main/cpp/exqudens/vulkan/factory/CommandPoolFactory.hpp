#pragma once

#include "exqudens/vulkan/model/CommandPool.hpp"

namespace exqudens::vulkan {

  class CommandPoolFactory {

    public:

      virtual CommandPool createCommandPool(
          VkDevice& device,
          uint32_t queueFamilyIndex
      ) = 0;
      virtual CommandPool createCommandPool(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          VkCommandPoolCreateFlags flags
      ) = 0;

      virtual void destroyCommandPool(CommandPool& commandPool) = 0;

  };

}
