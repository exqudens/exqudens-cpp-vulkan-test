#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DescriptorPoolCreateInfo {

    VkDescriptorPoolCreateFlags flags;
    uint32_t maxSets;
    std::vector<VkDescriptorPoolSize> poolSizes;

  };

}
