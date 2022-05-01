#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/CommandBufferFactory.hpp"

namespace exqudens::vulkan {

  class CommandBufferFactoryBase:
      virtual public CommandBufferFactory,
      virtual public UtilityBase
  {

    public:

      CommandBuffer createCommandBuffer(VkDevice& device, VkCommandPool& commandPool) override {
        try {
          VkCommandBuffer commandBuffer = nullptr;

          std::vector<VkCommandBuffer> commandBuffers;
          commandBuffers.resize(1);

          VkCommandBufferAllocateInfo allocInfo{};
          allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
          allocInfo.commandPool = commandPool;
          allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
          allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

          if (functions().allocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error(CALL_INFO() + ": failed to allocate command buffer!");
          }

          commandBuffer = commandBuffers[0];

          if (commandBuffer == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": failed to create command buffer!");
          }

          return {
              .device = device,
              .commandPool = commandPool,
              .value = commandBuffer
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<CommandBuffer> createCommandBuffers(
          VkDevice& device,
          VkCommandPool& commandPool,
          std::size_t size
      ) override {
        try {
          std::vector<CommandBuffer> commandBuffers;
          commandBuffers.resize(size);

          for (std::size_t i = 0; i < commandBuffers.size(); i++) {
            commandBuffers[i] = createCommandBuffer(device, commandPool);
          }

          return commandBuffers;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyCommandBuffer(CommandBuffer& commandBuffer) override {
        try {
          if (commandBuffer.value != nullptr) {
            functions().freeCommandBuffers(commandBuffer.device, commandBuffer.commandPool, 1, &commandBuffer.value);
            commandBuffer.device = nullptr;
            commandBuffer.commandPool = nullptr;
            commandBuffer.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyCommandBuffers(std::vector<CommandBuffer>& commandBuffers) override {
        try {
          for (std::size_t i = 0; i < commandBuffers.size(); i++) {
            destroyCommandBuffer(commandBuffers[i]);
          }
          commandBuffers.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
