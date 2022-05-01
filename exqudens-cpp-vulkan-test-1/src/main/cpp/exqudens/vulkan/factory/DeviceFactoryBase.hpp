#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/DeviceFactory.hpp"

namespace exqudens::vulkan {

  class DeviceFactoryBase:
      virtual public DeviceFactory,
      virtual public UtilityBase
  {

    public:

      Device createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          QueueFamilyIndexInfo& queueFamilyIndexInfo
      ) override {
        try {
          VkDevice device = nullptr;

          std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

          float queuePriority = 1.0f;
          for (uint32_t queueFamily : queueFamilyIndexInfo.uniqueQueueFamilyIndices) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
          }

          VkPhysicalDeviceFeatures deviceFeatures{};
          deviceFeatures.samplerAnisotropy = VK_TRUE;

          VkDeviceCreateInfo createInfo{};
          createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

          createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
          createInfo.pQueueCreateInfos = queueCreateInfos.data();

          createInfo.pEnabledFeatures = &deviceFeatures;

          createInfo.enabledExtensionCount = static_cast<uint32_t>(configuration.deviceExtensions.size());
          createInfo.ppEnabledExtensionNames = configuration.deviceExtensions.data();

          if (configuration.validationLayersEnabled) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(configuration.validationLayers.size());
            createInfo.ppEnabledLayerNames = configuration.validationLayers.data();
          } else {
            createInfo.enabledLayerCount = 0;
          }

          if (
              functions().createDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS
              || device == nullptr
              ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create logical device!");
          }

          return {
            .value = device
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDevice(Device& device) override {
        try {
          if (device.value != nullptr) {
            functions().destroyDevice(device.value, nullptr);
            device.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
