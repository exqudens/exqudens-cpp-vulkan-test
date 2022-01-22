#include <utility>
#include <functional>
#include <algorithm>
#include <iterator>

#include "vulkan/vulkan.h"

#include "exqudens/vulkan/model/Configuration.hpp"

namespace exqudens::vulkan::model {

  Configuration::Configuration(
      bool validationLayersEnabled,

      int frameBufferWidth,
      int frameBufferHeight,

      std::vector<std::string> commandLineArguments,
      std::vector<std::string> validationLayers,
      std::vector<std::string> extensions,
      std::vector<std::string> deviceExtensions,

      std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction,
      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction
  ):
      validationLayersEnabled(validationLayersEnabled),

      frameBufferWidth(frameBufferWidth),
      frameBufferHeight(frameBufferHeight),

      commandLineArguments(std::move(commandLineArguments)),
      validationLayers(std::move(validationLayers)),
      extensions(std::move(extensions)),
      deviceExtensions(std::move(deviceExtensions)),

      createSurfaceFunction(std::move(createSurfaceFunction)),
      logFunction(std::move(logFunction)),

      validationLayersPointers(std::vector<const char*>()),
      extensionsPointers(std::vector<const char*>()),
      deviceExtensionsPointers(std::vector<const char*>())
  {
    if (this->validationLayersEnabled) {
      this->extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    std::function<const char*(const std::string&)> function = [] (
        const std::string& s
    ) -> const char* {
      return s.c_str();
    };
    std::transform(
        this->validationLayers.begin(),
        this->validationLayers.end(),
        std::back_inserter(this->validationLayersPointers),
        function
    );
    std::transform(
        this->extensions.begin(),
        this->extensions.end(),
        std::back_inserter(this->extensionsPointers),
        function
    );
    std::transform(
        this->deviceExtensions.begin(),
        this->deviceExtensions.end(),
        std::back_inserter(this->deviceExtensionsPointers),
        function
    );
  }

  Configuration& Configuration::setFrameBufferWidth(int frameBufferWidth) {
    this->frameBufferWidth = frameBufferWidth;
    return *this;
  }

  Configuration& Configuration::setFrameBufferHeight(int frameBufferHeight) {
    this->frameBufferHeight = frameBufferHeight;
    return *this;
  }

  bool& Configuration::isValidationLayersEnabled() {
    return validationLayersEnabled;
  }

  int& Configuration::getFrameBufferWidth() {
    return frameBufferWidth;
  }

  int& Configuration::getFrameBufferHeight() {
    return frameBufferHeight;
  }

  std::vector<std::string>& Configuration::getCommandLineArguments() {
    return commandLineArguments;
  }

  std::vector<std::string>& Configuration::getValidationLayers() {
    return validationLayers;
  }

  std::vector<std::string>& Configuration::getExtensions() {
    return extensions;
  }

  std::vector<std::string>& Configuration::getDeviceExtensions() {
    return deviceExtensions;
  }

  std::function<VkSurfaceKHR(VkInstance)>& Configuration::getCreateSurfaceFunction() {
    return createSurfaceFunction;
  }

  std::function<void(
      VkDebugUtilsMessageSeverityFlagBitsEXT,
      VkDebugUtilsMessageTypeFlagsEXT,
      std::string
  )>& Configuration::getLogFunction() {
    return logFunction;
  }

  std::vector<const char*>& Configuration::getValidationLayersPointers() {
    return validationLayersPointers;
  }

  std::vector<const char*>& Configuration::getExtensionsPointers() {
    return extensionsPointers;
  }

  std::vector<const char*>& Configuration::getDeviceExtensionsPointers() {
    return deviceExtensionsPointers;
  }
}
