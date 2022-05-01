#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Buffer {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkDeviceMemory memory;
    VkDeviceSize memorySize;
    VkMemoryPropertyFlags memoryProperties; // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    VkBuffer value;

  };

}
