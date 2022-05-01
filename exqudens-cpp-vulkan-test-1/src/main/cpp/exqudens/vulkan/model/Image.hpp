#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Image {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    //VkImageTiling tiling = VkImageTiling::VK_IMAGE_TILING_OPTIMAL;
    //VkImageUsageFlags usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
    uint32_t width;
    uint32_t height;
    VkFormat format; // VK_FORMAT_R8G8B8A8_SRGB || VK_FORMAT_R8G8B8A8_UNORM
    VkDeviceMemory memory;
    VkDeviceSize memorySize;
    VkMemoryPropertyFlags memoryProperties; // VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    VkImage value;

  };

}
