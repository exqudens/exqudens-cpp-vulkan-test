#pragma once

#include <string>
#include <optional>
#include <functional>
#include <stdexcept>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/Macros.hpp"

namespace exqudens::vulkan {

  struct Logger {

    static VkBool32 call(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
        void* userData
    ) {
      try {
        auto* logger = reinterpret_cast<Logger*>(userData);
        std::string message(callbackData->pMessage);
        std::string messageFormatted = logger->function(messageSeverity, messageType, message);
        if (logger->exceptionSeverity.has_value() && messageSeverity >= logger->exceptionSeverity.value()) {
          throw std::runtime_error(messageFormatted);
        }
        if (
            logger->outSeverity.has_value()
            && messageSeverity >= logger->outSeverity.value()
            && logger->stream != nullptr
        ) {
          *(logger->stream) << messageFormatted << std::endl;
        }
        return VK_FALSE;
      } catch (const std::exception& e) {
        std::throw_with_nested(std::runtime_error(CALL_INFO() + ": " + e.what()));
      } catch (...) {
        std::throw_with_nested(std::runtime_error(CALL_INFO()));
      }
    }

    std::function<std::string(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const std::string&)> function;
    std::optional<VkDebugUtilsMessageSeverityFlagBitsEXT> exceptionSeverity;
    std::optional<VkDebugUtilsMessageSeverityFlagBitsEXT> outSeverity;
    std::ostream* stream;

  };

}
