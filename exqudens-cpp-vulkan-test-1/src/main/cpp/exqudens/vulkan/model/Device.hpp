#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Device {

    unsigned int id;
    bool destroyed;
    VkDevice value;

  };

}
