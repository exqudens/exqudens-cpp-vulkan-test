#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Pipeline {

    unsigned int id;
    bool destroyed;
    VkDevice device;
    VkPipelineLayout layout;
    VkPipeline value;

  };

}
