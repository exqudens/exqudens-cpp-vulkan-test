#pragma once

#include <string>
#include <map>
#include <functional>
#include <ostream>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"

namespace exqudens::vulkan {

  class UtilityFactory {

    public:

      virtual std::map<std::string, std::string> createEnvironmentVariables(
          const std::string& executableDirPath
      ) = 0;

      virtual std::function<std::string(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          const std::string&
      )> createLoggerFunction(
      ) = 0;

      virtual Configuration createConfiguration(
      ) = 0;

      virtual Logger createLogger(
      ) = 0;
      virtual Logger createLogger(
          std::ostream& stream
      ) = 0;
      virtual Logger createLogger(
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
      ) = 0;
      virtual Logger createLogger(
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
          std::ostream& stream
      ) = 0;
      virtual Logger createLogger(
          const std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function,
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
      ) = 0;
      virtual Logger createLogger(
          const std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function,
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
          std::ostream& stream
      ) = 0;

  };

}
