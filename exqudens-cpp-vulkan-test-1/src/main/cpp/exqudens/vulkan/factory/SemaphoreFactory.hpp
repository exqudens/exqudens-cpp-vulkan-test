#pragma once

#include "exqudens/vulkan/model/Semaphore.hpp"

namespace exqudens::vulkan {

  class SemaphoreFactory {

    public:

      virtual Semaphore createSemaphore(VkDevice& device) = 0;
      virtual Semaphore createSemaphore(VkDevice& device, VkSemaphoreCreateFlags flags) = 0;
      virtual std::vector<Semaphore> createSemaphores(VkDevice& device, std::size_t size) = 0;
      virtual std::vector<Semaphore> createSemaphores(
          VkDevice& device,
          VkSemaphoreCreateFlags flags,
          std::size_t size
      ) = 0;

      virtual void destroySemaphore(Semaphore& semaphore) = 0;
      virtual void destroySemaphores(std::vector<Semaphore>& semaphores) = 0;

  };

}
