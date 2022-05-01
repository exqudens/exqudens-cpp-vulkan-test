#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/BufferFactory.hpp"

namespace exqudens::vulkan {

  class BufferFactoryBase:
      virtual public BufferFactory,
      virtual public UtilityBase
  {

    public:

      Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const BufferCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      ) override {
        try {
          VkBuffer buffer = nullptr;

          VkBufferCreateInfo bufferInfo = {
              .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
              .flags = createInfo.flags,
              .size = createInfo.size,
              .usage = createInfo.usage,
              .sharingMode = createInfo.sharingMode,
              .queueFamilyIndexCount = static_cast<uint32_t>(createInfo.queueFamilyIndices.size()),
              .pQueueFamilyIndices = createInfo.queueFamilyIndices.empty() ? nullptr : createInfo.queueFamilyIndices.data()
          };

          if (
              functions().createBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS
              || buffer == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create buffer!");
          }

          VkDeviceMemory bufferMemory = nullptr;

          VkMemoryRequirements memRequirements;
          functions().getBufferMemoryRequirements(device, buffer, &memRequirements);

          VkMemoryAllocateInfo allocInfo{};
          allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
          allocInfo.allocationSize = memRequirements.size;
          allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

          if (
              functions().allocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS
              || bufferMemory == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to allocate buffer memory!");
          }

          if (functions().bindBufferMemory(device, buffer, bufferMemory, 0) != VK_SUCCESS) {
            throw std::runtime_error(CALL_INFO() + ": failed to bind buffer memory!");
          }

          return {
              .device = device,
              .memory = bufferMemory,
              .memorySize = createInfo.size,
              .memoryProperties = properties,
              .value = buffer
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Buffer createBuffer(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkDeviceSize memorySize,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties
      ) override {
        try {
          return createBuffer(
              physicalDevice,
              device,
              BufferCreateInfo {
                  .flags = 0,
                  .size = memorySize,
                  .usage = usage,
                  .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                  .queueFamilyIndices = {}
              },
              properties
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<Buffer> createBuffers(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkDeviceSize memorySize,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties,
          std::size_t size
      ) override {
        try {
          std::vector<Buffer> buffers;
          buffers.resize(size);
          for (std::size_t i = 0; i < size; i++) {
            buffers[i] = createBuffer(physicalDevice, device, memorySize, usage, properties);
          }
          return buffers;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyBuffer(Buffer& buffer) override {
        try {
          destroyBuffer(buffer, false);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyBuffer(Buffer& buffer, bool unmapMemory) override {
        try {
          if (buffer.memory != nullptr) {
            if (
                unmapMemory
                && (buffer.memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            ) {
              functions().unmapMemory(buffer.device, buffer.memory);
            }
            functions().freeMemory(buffer.device, buffer.memory, nullptr);
            buffer.memory = nullptr;
          }
          if (buffer.value != nullptr) {
            functions().destroyBuffer(buffer.device, buffer.value, nullptr);
            buffer.device = nullptr;
            buffer.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyBuffers(std::vector<Buffer>& buffers) override {
        try {
          for (Buffer& buffer : buffers) {
            destroyBuffer(buffer);
          }
          buffers.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyBuffers(std::vector<Buffer>& buffers, bool unmapMemory) override {
        try {
          for (Buffer& buffer : buffers) {
            destroyBuffer(buffer, unmapMemory);
          }
          buffers.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
