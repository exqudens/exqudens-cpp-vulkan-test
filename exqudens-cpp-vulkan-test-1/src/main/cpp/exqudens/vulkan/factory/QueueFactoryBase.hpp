#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/QueueFactory.hpp"

namespace exqudens::vulkan {

  class QueueFactoryBase:
      virtual public QueueFactory,
      virtual public UtilityBase
  {

    public:

      Queue createQueue(VkDevice& device, uint32_t queueFamilyIndex, uint32_t queueIndex) override {
        try {
          VkQueue queue = nullptr;

          functions().getDeviceQueue(device, queueFamilyIndex, queueIndex, &queue);

          if (queue == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": failed to create compute queue!");
          }

          return {
              .index = queueIndex,
              .familyIndex = queueFamilyIndex,
              .value = queue
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyQueue(Queue& queue) override {
        try {
          queue.index = 0;
          queue.familyIndex = 0;
          queue.value = nullptr;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
