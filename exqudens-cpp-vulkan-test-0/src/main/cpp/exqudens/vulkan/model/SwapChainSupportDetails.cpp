#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

#include <utility>

namespace exqudens::vulkan::model {

  SwapChainSupportDetails::SwapChainSupportDetails(
      VkSurfaceCapabilitiesKHR capabilities,
      std::vector<VkSurfaceFormatKHR> formats,
      std::vector<VkPresentModeKHR> presentModes
  ):
      capabilities(capabilities),
      formats(std::move(formats)),
      presentModes(std::move(presentModes))
  {
  }

  SwapChainSupportDetails::SwapChainSupportDetails(
  ):
      SwapChainSupportDetails(
          VkSurfaceCapabilitiesKHR{},
          std::vector<VkSurfaceFormatKHR>(),
          std::vector<VkPresentModeKHR>()
      )
  {
  }

  VkSurfaceCapabilitiesKHR& SwapChainSupportDetails::getCapabilities() {
    return capabilities;
  }

  std::vector<VkSurfaceFormatKHR>& SwapChainSupportDetails::getFormats() {
    return formats;
  }

  std::vector<VkPresentModeKHR>& SwapChainSupportDetails::getPresentModes() {
    return presentModes;
  }
}
