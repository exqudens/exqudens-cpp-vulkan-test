#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DescriptorPool {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkDescriptorPool value;

  };

}
