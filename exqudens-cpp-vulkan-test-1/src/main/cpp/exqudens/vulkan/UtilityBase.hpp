#pragma once

#include <cstdlib>
#include <set>
#include <fstream>
#include <stdexcept>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/FunctionsProviderBase.hpp"

namespace exqudens::vulkan {

  class UtilityBase:
      virtual public Utility,
      virtual public FunctionsProviderBase
  {

    public:

      void setEnvironmentVariable(const std::string& name, const std::string& value) override {
        try {
#ifdef _WIN32
          _putenv_s(name.c_str(), value.c_str());
#elif _WIN64
          _putenv_s(name.c_str(), value.c_str());
#endif
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::optional<std::string> getEnvironmentVariable(const std::string& name) override {
        try {
          std::optional<std::string> value;
#ifdef _WIN32
          char* buffer;
          size_t size;
          errno_t error = _dupenv_s(&buffer, &size, name.c_str());
          if (error) {
            return value;
          }
          if (buffer != nullptr) {
            value.emplace(std::string(buffer));
          }
#elif _WIN64
          char* buffer;
    size_t size;
    errno_t error = _dupenv_s(&buffer, &size, name.c_str());
    if (error) {
      return value;
    }
    if (buffer != nullptr) {
      value.emplace(std::string(buffer));
    }
#endif
          return value;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<char> readFile(const std::string& path) override {
        try {
          std::ifstream file(path, std::ios::ate | std::ios::binary);

          if (!file.is_open()) {
            throw std::runtime_error(CALL_INFO() + ": failed to open file: '" + path + "'!");
          }

          std::streamsize fileSize = file.tellg();
          std::vector<char> buffer(fileSize);

          file.seekg(0);
          file.read(buffer.data(), fileSize);

          file.close();

          return buffer;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers) override {
        try {
          uint32_t layerCount;
          functions().enumerateInstanceLayerProperties(&layerCount, nullptr);

          std::vector<VkLayerProperties> availableLayers(layerCount);
          functions().enumerateInstanceLayerProperties(&layerCount, availableLayers.data());

          for (const std::string& layerName: validationLayers) {
            bool layerFound = false;

            for (const VkLayerProperties& layerProperties : availableLayers) {
              if (std::string(layerName) == std::string(layerProperties.layerName)) {
                layerFound = true;
                break;
              }
            }

            if (!layerFound) {
              return false;
            }
          }

          return true;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object, Logger& logger) override {
        try {
          object = {};
          object.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
          object.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
              | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
              | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
          object.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
              | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
              | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
          object.pfnUserCallback = &Logger::call;
          object.pUserData = &logger;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      QueueFamilyIndexInfo findQueueFamilies(
          VkPhysicalDevice& physicalDevice,
          bool computeFamilyRequired,
          bool transferFamilyRequired,
          bool graphicsFamilyRequired,
          const VkSurfaceKHR& surface
      ) override {
        try {
          QueueFamilyIndexInfo result;
          result.computeFamilyRequired = computeFamilyRequired;
          result.transferFamilyRequired = transferFamilyRequired;
          result.graphicsFamilyRequired = graphicsFamilyRequired;
          result.presentFamilyRequired = surface != nullptr;

          uint32_t queueFamilyCount = 0;
          functions().getPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

          std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
          functions().getPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

          int i = 0;
          for (const auto& queueFamily : queueFamilies) {
            if (result.computeFamilyRequired) {
              if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                result.computeFamily = i;
                result.computeFamilyQueueCount = queueFamily.queueCount;
              }
            }

            if (result.transferFamilyRequired) {
              if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                result.transferFamily = i;
                result.transferFamilyQueueCount = queueFamily.queueCount;
              }
            }

            if (result.graphicsFamilyRequired) {
              if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                result.graphicsFamily = i;
                result.graphicsFamilyQueueCount = queueFamily.queueCount;
              }
            }

            if (result.presentFamilyRequired) {
              VkBool32 presentSupport = false;
              functions().getPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
              if (presentSupport) {
                result.presentFamily = i;
                result.presentFamilyQueueCount = queueFamily.queueCount;
              }
            }

            bool computeFamilyComplete = !result.computeFamilyRequired || result.computeFamily.has_value();
            bool transferFamilyComplete = !result.transferFamilyRequired || result.transferFamily.has_value();
            bool graphicsFamilyComplete = !result.graphicsFamilyRequired || result.graphicsFamily.has_value();
            bool presentFamilyComplete = !result.presentFamilyRequired || result.presentFamily.has_value();
            bool complete = computeFamilyComplete && transferFamilyComplete && graphicsFamilyComplete && presentFamilyComplete;

            if (complete) {
              result.complete = complete;
              break;
            }

            result.computeFamilyQueueCount = 0;
            result.transferFamilyQueueCount = 0;
            result.graphicsFamilyQueueCount = 0;
            result.presentFamilyQueueCount = 0;

            i++;
          }

          if (result.complete) {
            if (result.computeFamilyRequired) {
              result.uniqueQueueFamilyIndices.insert(result.computeFamily.value());
            }
            if (result.transferFamilyRequired) {
              result.uniqueQueueFamilyIndices.insert(result.transferFamily.value());
            }
            if (result.graphicsFamilyRequired) {
              result.uniqueQueueFamilyIndices.insert(result.graphicsFamily.value());
            }
            if (result.presentFamilyRequired) {
              result.uniqueQueueFamilyIndices.insert(result.presentFamily.value());
            }
          }

          return result;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      bool checkDeviceExtensionSupport(
          VkPhysicalDevice& physicalDevice,
          const std::vector<const char*>& deviceExtensions
      ) override {
        try {
          uint32_t extensionCount;
          functions().enumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);

          std::vector<VkExtensionProperties> availableExtensions(extensionCount);
          functions().enumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

          std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

          for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
          }

          return requiredExtensions.empty();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      SwapChainSupportDetails querySwapChainSupport(
          VkPhysicalDevice& physicalDevice,
          const VkSurfaceKHR& surface
      ) override {
        try {
          if (surface == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": failed to get swap chain support details!");
          }

          SwapChainSupportDetails details;

          functions().getPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

          uint32_t formatCount;
          functions().getPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

          if (formatCount != 0) {
            details.formats.resize(formatCount);
            functions().getPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
          }

          uint32_t presentModeCount;
          functions().getPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

          if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            functions().getPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
          }

          return details;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) override {
        try {
          for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
              return availableFormat;
            }
          }
          return availableFormats[0];
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) override {
        try {
          for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
              return availablePresentMode;
            }
          }
          return VK_PRESENT_MODE_FIFO_KHR;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      VkExtent2D chooseSwapExtent(
          VkSurfaceCapabilitiesKHR& capabilities,
          const uint32_t& width,
          const uint32_t& height
      ) override {
        try {
          if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
          } else {
            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };
            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
            return actualExtent;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      uint32_t findMemoryType(
          VkPhysicalDevice& physicalDevice,
          uint32_t typeFilter,
          VkMemoryPropertyFlags properties
      ) override {
        try {
          VkPhysicalDeviceMemoryProperties memProperties;
          functions().getPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

          for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
              return i;
            }
          }

          throw std::runtime_error(CALL_INFO() + ": failed to find suitable memory type!");
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      VkFormat findSupportedFormat(
          VkPhysicalDevice& physicalDevice,
          const std::vector<VkFormat>& candidates,
          VkImageTiling tiling,
          VkFormatFeatureFlags features
      ) override {
        try {
          for (VkFormat format : candidates) {
            VkFormatProperties props;
            functions().getPhysicalDeviceFormatProperties(physicalDevice, format, &props);
            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
              return format;
            } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
              return format;
            }
          }
          throw std::runtime_error(CALL_INFO() + ": failed to find supported format!");
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      VkFormat findDepthFormat(VkPhysicalDevice& physicalDevice) override {
        try {
          return findSupportedFormat(
              physicalDevice,
              {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT},
              VK_IMAGE_TILING_OPTIMAL,
              VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
