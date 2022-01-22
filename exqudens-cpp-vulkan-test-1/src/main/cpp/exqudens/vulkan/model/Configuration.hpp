#pragma once

#include <string>
#include <vector>
#include <functional>

#include "vulkan/vulkan.h"

namespace exqudens::vulkan::model {

  class Configuration {

    private:

      bool validationLayersEnabled;

      int frameBufferWidth;
      int frameBufferHeight;

      std::vector<std::string> commandLineArguments;
      std::vector<std::string> validationLayers;
      std::vector<std::string> extensions;
      std::vector<std::string> deviceExtensions;

      std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction;
      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction;

      std::vector<const char*> validationLayersPointers;
      std::vector<const char*> extensionsPointers;
      std::vector<const char*> deviceExtensionsPointers;

    public:

      Configuration(
          bool validationLayersEnabled,

          int frameBufferWidth,
          int frameBufferHeight,

          std::vector<std::string>  commandLineArguments,
          std::vector<std::string>  validationLayers,
          std::vector<std::string>  extensions,
          std::vector<std::string>  deviceExtensions,

          std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction,
          std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction
      );

      Configuration& setFrameBufferWidth(int frameBufferWidth);
      Configuration& setFrameBufferHeight(int frameBufferHeight);

      bool& isValidationLayersEnabled();

      int& getFrameBufferWidth();
      int& getFrameBufferHeight();

      std::vector<std::string>& getCommandLineArguments();
      std::vector<std::string>& getValidationLayers();
      std::vector<std::string>& getExtensions();
      std::vector<std::string>& getDeviceExtensions();

      std::function<VkSurfaceKHR(VkInstance)>& getCreateSurfaceFunction();
      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)>& getLogFunction();

      std::vector<const char*>& getValidationLayersPointers();
      std::vector<const char*>& getExtensionsPointers();
      std::vector<const char*>& getDeviceExtensionsPointers();

  };

}
