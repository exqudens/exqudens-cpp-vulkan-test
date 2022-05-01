#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/SemaphoreFactory.hpp"

namespace exqudens::vulkan {

  class SemaphoreFactoryBase:
      virtual public SemaphoreFactory,
      virtual public UtilityBase
  {

    public:

      Semaphore createSemaphore(VkDevice& device) override {
        try {
          return createSemaphore(device, 0);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Semaphore createSemaphore(VkDevice& device, VkSemaphoreCreateFlags flags) override {
        try {
          VkSemaphore semaphore = nullptr;

          VkSemaphoreCreateInfo semaphoreInfo = {};
          semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
          semaphoreInfo.flags = flags;

          if (
              functions().createSemaphore(device, &semaphoreInfo, nullptr, &semaphore) != VK_SUCCESS
              || semaphore == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create semaphore!");
          }

          return {
              .device = device,
              .value = semaphore
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<Semaphore> createSemaphores(VkDevice& device, std::size_t size) override {
        try {
          std::vector<Semaphore> semaphores;
          semaphores.resize(size);

          for (std::size_t i = 0; i < size; i++) {
            semaphores[i] = createSemaphore(device);
          }

          return semaphores;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<Semaphore> createSemaphores(
          VkDevice& device,
          VkSemaphoreCreateFlags flags,
          std::size_t size
      ) override {
        try {
          std::vector<Semaphore> semaphores;
          semaphores.resize(size);

          for (std::size_t i = 0; i < size; i++) {
            semaphores[i] = createSemaphore(device, flags);
          }

          return semaphores;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySemaphore(Semaphore& semaphore) override {
        try {
          if (semaphore.value != nullptr) {
            functions().destroySemaphore(semaphore.device, semaphore.value, nullptr);
            semaphore.device = nullptr;
            semaphore.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySemaphores(std::vector<Semaphore>& semaphores) override {
        try {
          for (Semaphore& semaphore : semaphores) {
            destroySemaphore(semaphore);
          }
          semaphores.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
