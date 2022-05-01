#pragma once

#include <vector>

#include "vulkan/vulkan.h"

namespace exqudens::vulkan {

  struct SwapChainSupportDetails {

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

  };

}
