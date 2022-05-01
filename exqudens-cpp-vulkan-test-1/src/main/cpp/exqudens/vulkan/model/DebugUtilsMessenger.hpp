#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct DebugUtilsMessenger {

    unsigned int id;
    bool destroyed;
    VkInstance instance;
    VkDebugUtilsMessengerEXT value;

  };

}
