#pragma once

#include <vector>

#include "vulkan/vulkan.h"

namespace exqudens::vulkan::model {

  struct SwapChainSupportDetails {

    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

  };

}
