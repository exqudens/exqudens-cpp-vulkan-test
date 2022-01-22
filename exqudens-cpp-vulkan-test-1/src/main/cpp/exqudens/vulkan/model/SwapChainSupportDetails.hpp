#pragma once

#include <vector>

#include "vulkan/vulkan.h"

namespace exqudens::vulkan::model {

  class SwapChainSupportDetails {

    private:

      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;

    public:

      SwapChainSupportDetails(
          VkSurfaceCapabilitiesKHR capabilities,
          std::vector<VkSurfaceFormatKHR> formats,
          std::vector<VkPresentModeKHR> presentModes
      );

      SwapChainSupportDetails();

      VkSurfaceCapabilitiesKHR& getCapabilities();

      std::vector<VkSurfaceFormatKHR>& getFormats();

      std::vector<VkPresentModeKHR>& getPresentModes();

  };

}
