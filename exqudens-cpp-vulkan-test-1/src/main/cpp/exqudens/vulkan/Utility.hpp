#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <vector>
#include <functional>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndexInfo.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"

namespace exqudens::vulkan {

  class Utility {

    public:

      virtual void setEnvironmentVariable(
          const std::string& name,
          const std::string& value
      ) = 0;

      virtual std::optional<std::string> getEnvironmentVariable(
          const std::string& name
      ) = 0;

      virtual std::vector<char> readFile(
          const std::string& path
      ) = 0;

      virtual bool checkValidationLayerSupport(
          const std::vector<const char*>& validationLayers
      ) = 0;

      virtual void populateDebugMessengerCreateInfo(
          VkDebugUtilsMessengerCreateInfoEXT& object,
          Logger& logger
      ) = 0;

      virtual QueueFamilyIndexInfo findQueueFamilies(
          VkPhysicalDevice& physicalDevice,
          bool computeFamilyRequired,
          bool transferFamilyRequired,
          bool graphicsFamilyRequired,
          const VkSurfaceKHR& surface
      ) = 0;

      virtual bool checkDeviceExtensionSupport(
          VkPhysicalDevice& physicalDevice,
          const std::vector<const char*>& deviceExtensions
      ) = 0;

      virtual SwapChainSupportDetails querySwapChainSupport(
          VkPhysicalDevice& physicalDevice,
          const VkSurfaceKHR& surface
      ) = 0;

      virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(
          const std::vector<VkSurfaceFormatKHR>& availableFormats
      ) = 0;

      virtual VkPresentModeKHR chooseSwapPresentMode(
          const std::vector<VkPresentModeKHR>& availablePresentModes
      ) = 0;

      virtual VkExtent2D chooseSwapExtent(
          VkSurfaceCapabilitiesKHR& capabilities,
          const uint32_t& width,
          const uint32_t& height
      ) = 0;

      virtual uint32_t findMemoryType(
          VkPhysicalDevice& physicalDevice,
          uint32_t typeFilter,
          VkMemoryPropertyFlags properties
      ) = 0;

      virtual VkFormat findSupportedFormat(
          VkPhysicalDevice& physicalDevice,
          const std::vector<VkFormat>& candidates,
          VkImageTiling tiling,
          VkFormatFeatureFlags features
      ) = 0;

      virtual VkFormat findDepthFormat(
          VkPhysicalDevice& physicalDevice
      ) = 0;

      virtual ~Utility() = default;

  };

}
