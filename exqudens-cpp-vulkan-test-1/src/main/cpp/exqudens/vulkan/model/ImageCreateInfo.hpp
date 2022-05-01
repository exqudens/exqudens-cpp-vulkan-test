#pragma once

#include <cstdint>
#include <vector>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct ImageCreateInfo {

    VkImageCreateFlags      flags;
    VkImageType             imageType;
    VkFormat                format;
    VkExtent3D              extent;
    uint32_t                mipLevels;
    uint32_t                arrayLayers;
    VkSampleCountFlagBits   samples;
    VkImageTiling           tiling;
    VkImageUsageFlags       usage;
    VkSharingMode           sharingMode;
    std::vector<uint32_t>   queueFamilyIndices;
    VkImageLayout           initialLayout;

  };

}
