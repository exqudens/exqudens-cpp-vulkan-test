#pragma once

#include <vector>
#include <functional>

#include "vulkan/vulkan.h"

namespace exqudens::vulkan::model {

  class Context {

    private:

      bool validationLayersEnabled;

      int frameBufferWidth;
      int frameBufferHeight;

      std::vector<const char*> commandLineArguments;
      std::vector<const char*> validationLayers;
      std::vector<const char*> extensions;
      std::vector<const char*> deviceExtensions;

      std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction;
      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction;

  };

}
