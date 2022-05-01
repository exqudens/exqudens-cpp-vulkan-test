#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct CommandBuffer {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkCommandPool commandPool;
    VkCommandBuffer value;

  };

}
