#include "exqudens/vulkan/Logger.hpp"

namespace exqudens::vulkan {

  VkBool32 Logger::call(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* callbackData,
      void* userData
  ) {
    auto* logger = reinterpret_cast<Logger*>(userData);
    std::string message(callbackData->pMessage);
    logger->logFunction(messageSeverity, messageType, message);
    return VK_FALSE;
  }

  Logger::Logger(
      std::function<void(
          VkDebugUtilsMessageSeverityFlagBitsEXT,
          VkDebugUtilsMessageTypeFlagsEXT,
          std::string
      )>& logFunction
  ):
      logFunction(logFunction)
  {
  }

  Logger::~Logger() = default;
}
