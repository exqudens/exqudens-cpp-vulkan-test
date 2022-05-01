#pragma once

#include <optional>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/model/QueueFamilyIndexInfo.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

namespace exqudens::vulkan {

  struct PhysicalDevice {

    unsigned int id;
    bool destroyed;
    QueueFamilyIndexInfo queueFamilyIndexInfo;
    std::optional<SwapChainSupportDetails> swapChainSupportDetails;
    VkPhysicalDevice value;

  };

}
