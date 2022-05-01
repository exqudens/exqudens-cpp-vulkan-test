#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct ImageView {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkImageView value;

  };

}
