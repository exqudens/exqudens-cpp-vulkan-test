#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DescriptorSetLayout {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkDescriptorSetLayout value;

  };

}
