#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/InstanceFactory.hpp"

namespace exqudens::vulkan {

  class InstanceFactoryBase:
      virtual public InstanceFactory,
      virtual public UtilityBase
  {

    public:

      Instance createInstance(Configuration& configuration, Logger& logger) override {
        try {
          VkInstance instance = nullptr;

          if (configuration.validationLayersEnabled && !checkValidationLayerSupport(configuration.validationLayers)) {
            throw std::runtime_error(CALL_INFO() + ": validation layers requested, but not available!");
          }

          VkApplicationInfo appInfo{};
          appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
          appInfo.pApplicationName = "Exqudens Application";
          appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
          appInfo.pEngineName = "Exqudens Engine";
          appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
          appInfo.apiVersion = VK_API_VERSION_1_0;

          VkInstanceCreateInfo createInfo{};
          createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
          createInfo.pApplicationInfo = &appInfo;
          createInfo.enabledExtensionCount = static_cast<uint32_t>(configuration.extensions.size());
          createInfo.ppEnabledExtensionNames = configuration.extensions.data();

          // declared before if statement for to not be destroyed before call vkCreateInstance
          VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
          if (configuration.validationLayersEnabled) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(configuration.validationLayers.size());
            createInfo.ppEnabledLayerNames = configuration.validationLayers.data();
            populateDebugMessengerCreateInfo(debugCreateInfo, logger);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*) &debugCreateInfo;
          } else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
          }

          if (functions().createInstance(&createInfo, nullptr, &instance) != VK_SUCCESS || instance == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": failed to create instance!");
          }

          return {
            .value = instance
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyInstance(Instance& instance) override {
        try {
          if (instance.value != nullptr) {
            functions().destroyInstance(instance.value, nullptr);
            instance.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
