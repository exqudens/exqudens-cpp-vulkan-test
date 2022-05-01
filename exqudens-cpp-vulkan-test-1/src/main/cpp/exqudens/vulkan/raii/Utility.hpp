#pragma once

#include <cstdlib>
#include <string>
#include <optional>
#include <vector>
#include <set>
#include <map>
#include <numeric>
#include <algorithm>
#include <stdexcept>

#include "vulkan/vulkan_raii.hpp"

#include "exqudens/vulkan/raii/Macros.hpp"

namespace exqudens::vulkan::raii {

  class Utility {

    public:

      // utility

      static void setEnvironmentVariable(const std::string& name, const std::string& value) {
        try {
#if defined(_WINDOWS)
          _putenv_s(name.c_str(), value.c_str());
#endif
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::optional<std::string> getEnvironmentVariable(const std::string& name) {
        try {
          std::optional<std::string> value;
#if defined(_WINDOWS)
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

      static VkBool32 debugCallback(
          VkDebugUtilsMessageSeverityFlagBitsEXT cSeverity,
          VkDebugUtilsMessageTypeFlagsEXT cType,
          const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
          void* data
      ) {
        try {
          auto severity = vk::DebugUtilsMessageSeverityFlagsEXT(cSeverity);
          auto type = vk::DebugUtilsMessageTypeFlagsEXT(cType);
          std::string message(callbackData->pMessage);
          std::cout << vk::to_string(severity) << " " << vk::to_string(type) << ": " << message << std::endl;
          return VK_FALSE;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static bool isValidationLayersSupported(
          const vk::raii::Context& context,
          const std::vector<const char*>& enabledLayerNames
      ) {
        try {
          uint32_t propertyCount = 0;
          vk::Result result = vk::enumerateInstanceLayerProperties(&propertyCount, nullptr, *context.getDispatcher());

          if (result != vk::Result::eSuccess) {
            throw std::runtime_error(CALL_INFO() + ": failed to enumerate instance layer properties count!");
          }

          std::vector<vk::LayerProperties> properties(propertyCount);
          result = vk::enumerateInstanceLayerProperties(&propertyCount, properties.data(), *context.getDispatcher());

          if (result != vk::Result::eSuccess) {
            throw std::runtime_error(CALL_INFO() + ": failed to enumerate instance layer properties!");
          }

          for (const char* enabledLayerName : enabledLayerNames) {
            bool layerFound = false;

            for (const vk::LayerProperties& layerProperties : properties) {
              std::string layerName = std::string(enabledLayerName);
              std::string name = std::string(layerProperties.layerName.data());
              if (layerName == name) {
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

      static bool isDeviceExtensionSupported(
          const vk::raii::PhysicalDevice& physicalDevice,
          const std::vector<const char*>& deviceExtensions
      ) {
        try {
          std::vector<vk::ExtensionProperties> properties = physicalDevice.enumerateDeviceExtensionProperties(nullptr);
          std::set<std::string> required(deviceExtensions.begin(), deviceExtensions.end());

          for (const auto& extension : properties) {
            std::string name = std::string(extension.extensionName.data());
            required.erase(name);
          }

          return required.empty();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::vector<uint32_t> getQueueFamilyIndices(
          const vk::raii::PhysicalDevice& physicalDevice,
          std::optional<vk::QueueFlagBits> type,
          const vk::raii::SurfaceKHR* surface
      ) {
        try {
          std::vector<uint32_t> queueFamilyIndices;

          std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();

          if (type.has_value()) {
            for (std::size_t i = 0; i < properties.size(); i++) {
              if (properties[i].queueFlags & *type) {
                auto queueFamilyIndex = static_cast<uint32_t>(i);
                queueFamilyIndices.emplace_back(queueFamilyIndex);
              }
            }
          }

          if (surface != nullptr) {
            for (std::size_t i = 0; i < properties.size(); i++) {
              if (physicalDevice.getSurfaceSupportKHR(static_cast<uint32_t>(i), *(*surface))) {
                auto queueFamilyIndex = static_cast<uint32_t>(i);
                queueFamilyIndices.emplace_back(queueFamilyIndex);
              }
            }
          }

          return queueFamilyIndices;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static uint32_t getQueueFamilyCount(
          const vk::raii::PhysicalDevice& physicalDevice,
          const uint32_t& queueFamilyIndex
      ) {
        try {
          std::vector<vk::QueueFamilyProperties> properties = physicalDevice.getQueueFamilyProperties();
          auto i = static_cast<std::size_t>(queueFamilyIndex);
          return properties[i].queueCount;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::vector<std::size_t> getPhysicalDeviceIndices(
          const std::vector<vk::raii::PhysicalDevice>& physicalDevices,
          const std::vector<vk::QueueFlagBits>& types,
          const vk::raii::SurfaceKHR* surface,
          const std::vector<const char*>& enabledDeviceExtensionNames,
          const bool& anisotropyRequired
      ) {
        try {
          std::vector<std::size_t> physicalDeviceIndices = {};

          for (std::size_t i = 0; i < physicalDevices.size(); i++) {

            bool queueFamilyIndicesAdequate = true;
            for (const vk::QueueFlagBits& type : types) {
              std::vector<uint32_t> indices = getQueueFamilyIndices(physicalDevices[i], type, surface);
              if (indices.empty()) {
                queueFamilyIndicesAdequate = false;
                break;
              }
            }

            bool deviceExtensionAdequate = isDeviceExtensionSupported(physicalDevices[i], enabledDeviceExtensionNames);

            bool swapChainAdequate = surface == nullptr;
            if (surface != nullptr) {
              std::vector<vk::SurfaceFormatKHR> formats = physicalDevices[i].getSurfaceFormatsKHR(*(*surface));
              std::vector<vk::PresentModeKHR> presentModes = physicalDevices[i].getSurfacePresentModesKHR(*(*surface));
              swapChainAdequate = !formats.empty() && !presentModes.empty();
            }

            bool anisotropyAdequate = !anisotropyRequired;
            if (anisotropyRequired) {
              vk::PhysicalDeviceFeatures features = physicalDevices[i].getFeatures();
              anisotropyAdequate = features.samplerAnisotropy;
            }

            if (
                queueFamilyIndicesAdequate
                && deviceExtensionAdequate
                && swapChainAdequate
                && anisotropyAdequate
            ) {
              physicalDeviceIndices.emplace_back(i);
            }
            
          }

          return physicalDeviceIndices;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::optional<vk::SurfaceFormatKHR> surfaceFormat(
          const std::vector<vk::SurfaceFormatKHR>& formats,
          const std::vector<vk::Format>& criteria1 = {vk::Format::eB8G8R8A8Srgb},
          const std::vector<vk::ColorSpaceKHR>& criteria2 = {vk::ColorSpaceKHR::eSrgbNonlinear}
      ) {
        try {
          std::optional<vk::SurfaceFormatKHR> format = {};

          if (formats.size() == 1 && formats.front() == vk::Format::eUndefined) {
            format = formats.front();
          } else {
            auto check1 = [](
                const std::vector<vk::Format>& filterValues,
                const vk::SurfaceFormatKHR& value
            ) {
              return std::ranges::any_of(
                  filterValues,
                  [&value](const vk::Format& o) { return o == value.format; }
              );
            };

            auto check2 = [](
                const std::vector<vk::ColorSpaceKHR>& filterValues,
                const vk::SurfaceFormatKHR& value
            ) {
              return std::ranges::any_of(
                  filterValues,
                  [&value](const vk::ColorSpaceKHR& o) { return o == value.colorSpace; }
              );
            };

            for (const vk::SurfaceFormatKHR& f : formats) {
              if (check1(criteria1, f) && check2(criteria2, f)) {
                format = f;
                break;
              }
            }
          }

          return format;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::optional<vk::PresentModeKHR> surfacePresentMode(
          const std::vector<vk::PresentModeKHR>& presentModes,
          const vk::PresentModeKHR& searchValue = vk::PresentModeKHR::eMailbox,
          const vk::PresentModeKHR& defaultValue = vk::PresentModeKHR::eFifo
      ) {
        try {
          std::optional<vk::PresentModeKHR> presentMode = defaultValue;

          for (const vk::PresentModeKHR& p : presentModes) {
            if (p == searchValue) {
              presentMode = searchValue;
              return presentMode;
            }
          }

          return presentMode;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::optional<vk::Extent2D> surfaceExtent(
          const vk::SurfaceCapabilitiesKHR& surfaceCapabilities,
          const uint32_t& width,
          const uint32_t& height
      ) {
        try {
          std::optional<vk::Extent2D> extend = {};

          if (surfaceCapabilities.currentExtent.width == std::numeric_limits<uint32_t>::max()) {
            extend = vk::Extent2D()
                .setWidth(std::clamp(width, surfaceCapabilities.minImageExtent.width, surfaceCapabilities.maxImageExtent.width))
                .setHeight(std::clamp(height, surfaceCapabilities.minImageExtent.height, surfaceCapabilities.maxImageExtent.height));
          } else {
            extend = surfaceCapabilities.currentExtent;
          }

          return extend;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::optional<vk::SurfaceTransformFlagBitsKHR> surfaceTransform(
          const vk::SurfaceCapabilitiesKHR& surfaceCapabilities
      ) {
        try {
          std::optional<vk::SurfaceTransformFlagBitsKHR> surfaceTransform = {};

          if (surfaceCapabilities.supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity) {
            surfaceTransform = vk::SurfaceTransformFlagBitsKHR::eIdentity;
          } else {
            surfaceTransform = surfaceCapabilities.currentTransform;
          }

          return surfaceTransform;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static std::optional<vk::CompositeAlphaFlagBitsKHR> surfaceCompositeAlpha(
          const vk::SurfaceCapabilitiesKHR& surfaceCapabilities
      ) {
        try {
          std::optional<vk::CompositeAlphaFlagBitsKHR> surfaceCompositeAlpha = {};

          if (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePreMultiplied) {
            surfaceCompositeAlpha = vk::CompositeAlphaFlagBitsKHR::ePreMultiplied;
          } else if (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::ePostMultiplied) {
            surfaceCompositeAlpha = vk::CompositeAlphaFlagBitsKHR::ePostMultiplied;
          } else if (surfaceCapabilities.supportedCompositeAlpha & vk::CompositeAlphaFlagBitsKHR::eInherit) {
            surfaceCompositeAlpha = vk::CompositeAlphaFlagBitsKHR::eInherit;
          } else {
            surfaceCompositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
          }

          return surfaceCompositeAlpha;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static uint32_t memoryType(
          const vk::PhysicalDeviceMemoryProperties& properties,
          uint32_t typeBits,
          vk::MemoryPropertyFlags requirementsMask
      ) {
        try {
          auto typeIndex = uint32_t(~0);
          for (uint32_t i = 0; i < properties.memoryTypeCount; i++) {
            if (
                (typeBits & 1)
                &&
                (properties.memoryTypes[i].propertyFlags & requirementsMask) == requirementsMask
            ) {
              typeIndex = i;
              break;
            }
            typeBits >>= 1;
          }
          if (typeIndex == uint32_t(~0)) {
            throw std::runtime_error(CALL_INFO() + ": failed to find memory type!");
          }
          return typeIndex;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
