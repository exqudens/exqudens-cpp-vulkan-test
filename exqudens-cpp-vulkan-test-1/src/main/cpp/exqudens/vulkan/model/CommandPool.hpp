#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct CommandPool {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkCommandPool value;

  };

}
