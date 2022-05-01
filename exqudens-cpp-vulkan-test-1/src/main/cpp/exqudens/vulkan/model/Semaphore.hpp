#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Semaphore {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkSemaphore value;

  };

}
