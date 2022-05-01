#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Surface {

    unsigned int id;
    bool destroyed;
    VkInstance instance;
    VkSurfaceKHR value;

  };

}
