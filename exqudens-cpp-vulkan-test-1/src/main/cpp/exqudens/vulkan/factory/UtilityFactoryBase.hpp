#pragma once

#include <filesystem>
#include <iostream>

#include "exqudens/vulkan/factory/UtilityFactory.hpp"

namespace exqudens::vulkan {

  class UtilityFactoryBase:
      virtual public UtilityFactory
  {

    public:

      std::map<std::string, std::string> createEnvironmentVariables(const std::string& executableDirPath) override {
        try {
          std::map<std::string, std::string> environmentVariables;
          environmentVariables["VK_LAYER_PATH"] = std::filesystem::path(executableDirPath).make_preferred().string();
          return environmentVariables;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::function<std::string(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          const std::string&
      )> createLoggerFunction() override {
        try {
          return [](
              VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT messageType,
              const std::string& message
          ) {
            std::string level;
            if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT == messageSeverity) {
              level = "[VERBOSE]";
            } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT == messageSeverity) {
              level = "[INFO]";
            } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT == messageSeverity) {
              level = "[WARNING]";
            } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT == messageSeverity) {
              level = "[ERROR]";
            } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT == messageSeverity) {
              level = "[MAX]";
            }

            std::string type;
            if (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT == messageType) {
              type = "(GENERAL)";
            } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT == messageType) {
              type = "(VALIDATION)";
            } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT == messageType) {
              type = "(PERFORMANCE)";
            } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT == messageType) {
              type = "(MAX)";
            }

            std::string line;

            line += level;
            line += " ";
            line += type;
            line += " ";
            line += "validation layer:";
            line += " ";
            line += message;

            return line;
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Configuration createConfiguration() override {
        try {
          return {
              .applicationName = "Exqudens Application",
              .applicationVersionMajor = 1,
              .applicationVersionMinor = 0,
              .applicationVersionPatch = 0,
              .engineName = "Exqudens Engine",
              .engineVersionMajor = 1,
              .engineVersionMinor = 0,
              .engineVersionPatch = 0,
              .validationLayersEnabled = true,
              .validationLayers = {"VK_LAYER_KHRONOS_validation"},
              .extensions = {VK_EXT_DEBUG_UTILS_EXTENSION_NAME},
              .deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
              .computeQueueFamilyRequired = true,
              .transferQueueFamilyRequired = true,
              .graphicsQueueFamilyRequired = true,
              .presentQueueFamilyRequired = true,
              .anisotropyRequired = true
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Logger createLogger() override {
        try {
          return createLogger(
              VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
              VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
              std::cout
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Logger createLogger(std::ostream& stream) override {
        try {
          return createLogger(
              VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
              VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT,
              stream
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Logger createLogger(
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
      ) override {
        try {
          std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              std::string
          )> function = createLoggerFunction();
          return createLogger(function, exceptionSeverity, outSeverity, std::cout);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Logger createLogger(
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
          std::ostream& stream
      ) override {
        try {
          std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              std::string
          )> function = createLoggerFunction();
          return createLogger(function, exceptionSeverity, outSeverity, stream);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Logger createLogger(
          const std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function,
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity
      ) override {
        try {
          return createLogger(function, exceptionSeverity, outSeverity, std::cout);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Logger createLogger(
          const std::function<std::string(
              VkDebugUtilsMessageSeverityFlagBitsEXT,
              VkDebugUtilsMessageTypeFlagsEXT,
              const std::string&
          )>& function,
          VkDebugUtilsMessageSeverityFlagBitsEXT exceptionSeverity,
          VkDebugUtilsMessageSeverityFlagBitsEXT outSeverity,
          std::ostream& stream
      ) override {
        try {
          return {
              .function = function,
              .exceptionSeverity = exceptionSeverity,
              .outSeverity = outSeverity,
              .stream = &stream
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
