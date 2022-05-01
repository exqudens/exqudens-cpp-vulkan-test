#pragma once

#include "exqudens/vulkan/model/SwapChain.hpp"

namespace exqudens::vulkan {

  class SwapChainFactory {

    public:

      virtual SwapChain createSwapChain(
          SwapChainSupportDetails& swapChainSupport,
          QueueFamilyIndexInfo& queueFamilyIndexInfo,
          VkSurfaceKHR& surface,
          VkDevice& device,
          const uint32_t& width,
          const uint32_t& height
      ) = 0;

      virtual std::vector<Image> createSwapChainImages(
          VkDevice& device,
          VkSwapchainKHR& swapChain
      ) = 0;

      virtual void destroySwapChain(SwapChain& swapChain) = 0;

  };

}
