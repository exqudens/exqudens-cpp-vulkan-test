#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct RenderPass {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkRenderPass value;

  };

}
