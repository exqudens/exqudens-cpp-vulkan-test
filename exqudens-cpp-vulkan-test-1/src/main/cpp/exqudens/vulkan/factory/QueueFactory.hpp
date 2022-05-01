#pragma once

#include "exqudens/vulkan/model/Queue.hpp"

namespace exqudens::vulkan {

  class QueueFactory {

    public:

      virtual Queue createQueue(
          VkDevice& device,
          uint32_t queueFamilyIndex,
          uint32_t queueIndex
      ) = 0;

      virtual void destroyQueue(Queue& queue) = 0;

  };

}
