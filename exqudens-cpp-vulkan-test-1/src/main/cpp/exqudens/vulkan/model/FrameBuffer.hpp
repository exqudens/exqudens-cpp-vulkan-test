#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct FrameBuffer {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkFramebuffer value;

  };

}
