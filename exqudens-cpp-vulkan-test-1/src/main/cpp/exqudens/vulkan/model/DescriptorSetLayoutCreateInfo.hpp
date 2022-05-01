#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DescriptorSetLayoutCreateInfo {

    VkDescriptorSetLayoutCreateFlags flags;
    std::vector<VkDescriptorSetLayoutBinding> bindings;

  };

}
