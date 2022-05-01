#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/DebugUtilsMessengerFactory.hpp"

namespace exqudens::vulkan {

  class DebugUtilsMessengerFactoryBase:
      virtual public DebugUtilsMessengerFactory,
      virtual public UtilityBase
  {

    public:

      DebugUtilsMessenger createDebugUtilsMessenger(VkInstance& instance, Logger& logger) override {
        try {
          VkDebugUtilsMessengerEXT debugUtilsMessenger = nullptr;

          VkResult result;

          auto func = (PFN_vkCreateDebugUtilsMessengerEXT) functions().getInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");

          if (func != nullptr) {
            VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
            populateDebugMessengerCreateInfo(createInfo, logger);
            result = func(instance, &createInfo, nullptr, &debugUtilsMessenger);
          } else {
            result = VK_ERROR_EXTENSION_NOT_PRESENT;
          }

          if (result != VK_SUCCESS || debugUtilsMessenger == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": failed create debug messenger!");
          }

          return {
            .instance = instance,
            .value = debugUtilsMessenger
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDebugUtilsMessenger(DebugUtilsMessenger& debugUtilsMessenger) override {
        try {
          if (debugUtilsMessenger.value != nullptr) {
            auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) functions().getInstanceProcAddr(debugUtilsMessenger.instance, "vkDestroyDebugUtilsMessengerEXT");
            func(debugUtilsMessenger.instance, debugUtilsMessenger.value, nullptr);
            debugUtilsMessenger.instance = nullptr;
            debugUtilsMessenger.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
