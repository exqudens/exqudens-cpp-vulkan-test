#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Fence {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkFence value;

  };

}
