#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/SwapChainFactory.hpp"

namespace exqudens::vulkan {

  class SwapChainFactoryBase:
      virtual public SwapChainFactory,
      virtual public UtilityBase
  {

    public:

      SwapChain createSwapChain(
          SwapChainSupportDetails& swapChainSupport,
          QueueFamilyIndexInfo& queueFamilyIndexInfo,
          VkSurfaceKHR& surface,
          VkDevice& device,
          const uint32_t& width,
          const uint32_t& height
      ) override {
        try {
          if (surface == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": surface is null failed to create swap chain!");
          }

          VkSwapchainKHR swapChain = nullptr;

          VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
          VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
          VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, width, height);

          uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

          if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
          }

          VkSwapchainCreateInfoKHR createInfo{};
          createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
          createInfo.surface = surface;

          createInfo.minImageCount = imageCount;
          createInfo.imageFormat = surfaceFormat.format;
          createInfo.imageColorSpace = surfaceFormat.colorSpace;
          createInfo.imageExtent = extent;
          createInfo.imageArrayLayers = 1;
          createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

          std::vector<uint32_t> queueFamilyIndices = {
              queueFamilyIndexInfo.graphicsFamily.value(),
              queueFamilyIndexInfo.presentFamily.value()
          };

          if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
          } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
          }

          createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
          createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
          createInfo.presentMode = presentMode;
          createInfo.clipped = VK_TRUE;

          createInfo.oldSwapchain = nullptr;

          if (
              functions().createSwapchainKHR(device, &createInfo, nullptr, &swapChain) != VK_SUCCESS
              || swapChain == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create swap chain!");
          }

          return {
              .device = device,
              .format = surfaceFormat.format,
              .extent = extent,
              .imageCount = imageCount,
              .width = width,
              .height = height,
              .value = swapChain
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<Image> createSwapChainImages(VkDevice& device, VkSwapchainKHR& swapChain) override {
        try {
          uint32_t imageCount = 0;
          std::vector<VkImage> vkImages;

          if (functions().getSwapchainImagesKHR(device, swapChain, &imageCount, nullptr) != VK_SUCCESS) {
            throw std::runtime_error("failed to get swap chain image count!");
          }

          vkImages.resize(imageCount);

          if (functions().getSwapchainImagesKHR(device, swapChain, &imageCount, vkImages.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to get swap chain images!");
          }

          std::vector<Image> images;
          images.resize(vkImages.size());

          for (std::size_t i = 0; i < images.size(); i++) {
            images[i] = {
                .value = vkImages[i]
            };
          }

          return images;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySwapChain(SwapChain& swapChain) override {
        try {
          if (swapChain.value != nullptr) {
            functions().destroySwapchainKHR(swapChain.device, swapChain.value, nullptr);
            swapChain.device = nullptr;
            swapChain.value = nullptr;
          }
          swapChain.extent = {};
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
