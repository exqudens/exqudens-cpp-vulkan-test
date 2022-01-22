#pragma once

#include <string>
#include <functional>

#include "vulkan/vulkan.h"

namespace exqudens::vulkan {

  class Logger {

    private:

      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)>& logFunction;

    public:

      static VkBool32 call(
          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType,
          const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
          void* userData
      );

      explicit Logger(
          std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)>& logFunction
      );

      ~Logger();

  };

}
