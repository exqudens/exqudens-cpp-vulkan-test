#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Sampler {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkSampler value;

  };

}
