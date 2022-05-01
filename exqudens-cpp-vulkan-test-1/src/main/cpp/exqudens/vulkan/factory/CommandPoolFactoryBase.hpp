#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/CommandPoolFactory.hpp"

namespace exqudens::vulkan {

  class CommandPoolFactoryBase:
      virtual public CommandPoolFactory,
      virtual public UtilityBase
  {

    public:

      CommandPool createCommandPool(VkDevice& device, uint32_t queueFamilyIndex) override {
        try {
          return createCommandPool(device, queueFamilyIndex, 0);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      CommandPool createCommandPool(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          VkCommandPoolCreateFlags flags
      ) override {
        try {
          VkCommandPool commandPool = nullptr;

          VkCommandPoolCreateInfo createInfo = {};
          createInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
          createInfo.queueFamilyIndex = queueFamilyIndex;
          createInfo.flags = flags;

          if (
              functions().createCommandPool(device, &createInfo, nullptr, &commandPool) != VK_SUCCESS
              || commandPool == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create compute command pool!");
          }

          return {
            .device = device,
            .value = commandPool
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyCommandPool(CommandPool& commandPool) override {
        try {
          if (commandPool.value != nullptr) {
            functions().destroyCommandPool(commandPool.device, commandPool.value, nullptr);
            commandPool.device = nullptr;
            commandPool.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
