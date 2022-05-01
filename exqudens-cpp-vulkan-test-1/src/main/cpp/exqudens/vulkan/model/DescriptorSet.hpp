#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DescriptorSet {

    unsigned int id;
    bool destroyed;
    VkDescriptorSet value;

  };

}
