#pragma once

#include <stdexcept>

#include "vulkan/vulkan_raii.hpp"

#include "exqudens/vulkan/raii/Context.hpp"
#include "exqudens/vulkan/raii/Instance.hpp"
#include "exqudens/vulkan/raii/DebugUtilsMessenger.hpp"
#include "exqudens/vulkan/raii/Macros.hpp"

namespace exqudens::vulkan::raii {

  class Factory {

    public:

      // create

      static Context createContext() {
        try {
          return {
            .info = ContextInfo {
              .enabledLayerNames = {"VK_LAYER_KHRONOS_validation"},
              .enabledExtensionNames = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME},
              .enabledDeviceExtensionNames = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
              .samplerAnisotropyEnabled = true
            },
            .optional = vk::raii::Context()
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static Instance createInstance(
          Context& context,
          const vk::ApplicationInfo& applicationInfo,
          const vk::InstanceCreateInfo& createInfo
      ) {
        try {
          Instance instance = {};
          instance.applicationInfo = applicationInfo;
          instance.createInfo = createInfo;

          instance.createInfo.pApplicationInfo = &instance.applicationInfo;

          instance.optional = vk::raii::Instance(
              context.optional.value(),
              instance.createInfo
          );

          return instance;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      static DebugUtilsMessenger createDebugUtilsMessenger(
          Instance& instance,
          const vk::DebugUtilsMessengerCreateInfoEXT& createInfo
      ) {
        try {
          DebugUtilsMessenger debugUtilsMessenger = {};

          debugUtilsMessenger.createInfo = createInfo;
          debugUtilsMessenger.optional = vk::raii::DebugUtilsMessengerEXT(
              instance.optional.value(),
              createInfo
          );

          return debugUtilsMessenger;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
