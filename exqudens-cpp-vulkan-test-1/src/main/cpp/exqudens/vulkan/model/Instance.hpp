#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Instance {

    unsigned int id;
    bool destroyed;
    VkInstance value;

  };

}
