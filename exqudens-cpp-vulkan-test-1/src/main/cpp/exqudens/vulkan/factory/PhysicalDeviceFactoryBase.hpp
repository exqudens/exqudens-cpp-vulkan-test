#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/PhysicalDeviceFactory.hpp"

namespace exqudens::vulkan {

  class PhysicalDeviceFactoryBase:
      virtual public PhysicalDeviceFactory,
      virtual public UtilityBase
  {

    public:

      PhysicalDevice createPhysicalDevice(VkInstance& instance, Configuration& configuration) override {
        try {
          return createPhysicalDevice(instance, configuration, nullptr);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      PhysicalDevice createPhysicalDevice(
          VkInstance& instance,
          Configuration& configuration,
          const VkSurfaceKHR& surface
      ) override {
        try {
          VkPhysicalDevice physicalDevice = nullptr;

          if (configuration.presentQueueFamilyRequired && surface == nullptr) {
            throw std::runtime_error(
                CALL_INFO()
                + ": '"
                + TO_STRING(configuration.presentQueueFamilyRequired)
                + "': true but '"
                + TO_STRING(surface)
                + "' is null!"
            );
          }

          uint32_t deviceCount = 0;
          functions().enumeratePhysicalDevices(instance, &deviceCount, nullptr);

          if (deviceCount == 0) {
            throw std::runtime_error(CALL_INFO() + ": failed to find GPUs with Vulkan support!");
          }

          std::vector<VkPhysicalDevice> devices(deviceCount);
          functions().enumeratePhysicalDevices(instance, &deviceCount, devices.data());

          QueueFamilyIndexInfo queueFamilyIndexInfo = {};
          std::optional<SwapChainSupportDetails> swapChainSupportDetails = {};

          for (VkPhysicalDevice& object : devices) {

            QueueFamilyIndexInfo familyIndices = findQueueFamilies(
                object,
                configuration.computeQueueFamilyRequired,
                configuration.transferQueueFamilyRequired,
                configuration.graphicsQueueFamilyRequired,
                surface
            );

            bool extensionsSupported = checkDeviceExtensionSupport(object, configuration.deviceExtensions);

            bool swapChainAdequate = surface == nullptr;
            std::optional<SwapChainSupportDetails> optionalSwapChainSupport = {};
            if (extensionsSupported && surface != nullptr) {
              SwapChainSupportDetails swapChainSupport = querySwapChainSupport(object, surface);
              swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
              optionalSwapChainSupport = swapChainSupport;
            }

            VkPhysicalDeviceFeatures supportedFeatures;
            functions().getPhysicalDeviceFeatures(object, &supportedFeatures);

            bool anisotropyAdequate = true;
            if (configuration.anisotropyRequired) {
              anisotropyAdequate = supportedFeatures.samplerAnisotropy;
            }

            bool isDeviceSuitable = familyIndices.complete &&
                                    extensionsSupported &&
                                    swapChainAdequate &&
                                    anisotropyAdequate;

            if (isDeviceSuitable) {
              queueFamilyIndexInfo = familyIndices;
              swapChainSupportDetails = optionalSwapChainSupport;
              physicalDevice = object;
              break;
            }
          }

          if (physicalDevice == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": failed to find a suitable GPU!");
          }

          return {
            .queueFamilyIndexInfo = queueFamilyIndexInfo,
            .swapChainSupportDetails = swapChainSupportDetails,
            .value = physicalDevice
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyPhysicalDevice(PhysicalDevice& physicalDevice) override {
        try {
          physicalDevice.queueFamilyIndexInfo = {};
          physicalDevice.swapChainSupportDetails = {};
          physicalDevice.value = nullptr;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
