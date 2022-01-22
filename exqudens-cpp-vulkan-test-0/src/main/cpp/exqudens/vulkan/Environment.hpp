#pragma once

#include <string>
#include <vector>
#include <map>

#include "vulkan/vulkan.h"

#include "exqudens/vulkan/model/Context.hpp"
#include "exqudens/vulkan/Logger.hpp"

namespace exqudens::vulkan {

  using Context = model::Context;

  class Environment {

    private:

      Context context;

    public:

      Environment(
          bool validationLayersEnabled,

          int frameBufferWidth,
          int frameBufferHeight,

          std::vector<std::string> commandLineArguments,
          std::vector<std::string> validationLayers,
          std::vector<std::string> extensions,
          std::vector<std::string> deviceExtensions,

          std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction,
          std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction
      );

  };

}
