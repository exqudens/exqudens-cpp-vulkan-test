#include <stdexcept>
#include <utility>
#include <iostream>

#include "exqudens/vulkan/Environment.hpp"

#define TO_EXCEPTION_MESSAGE(message) std::string(__FUNCTION__) + "(" + __FILE__ + ":" + std::to_string(__LINE__) + "): " + message
#define THROW_RUNTIME_ERROR(message) throw std::runtime_error(TO_EXCEPTION_MESSAGE(message))

namespace exqudens::vulkan {

  Environment::Environment(
      bool validationLayersEnabled,

      int frameBufferWidth,
      int frameBufferHeight,

      std::vector<std::string> commandLineArguments,
      std::vector<std::string> validationLayers,
      std::vector<std::string> extensions,
      std::vector<std::string> deviceExtensions,

      std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction,
      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction
  ) {
  }

}
