#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct BufferCreateInfo {

    VkBufferCreateFlags   flags;
    VkDeviceSize          size;
    VkBufferUsageFlags    usage;
    VkSharingMode         sharingMode;
    std::vector<uint32_t> queueFamilyIndices;

  };

}
