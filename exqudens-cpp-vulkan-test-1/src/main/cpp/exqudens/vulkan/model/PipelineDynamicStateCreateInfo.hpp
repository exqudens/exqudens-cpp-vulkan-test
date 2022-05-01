#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct PipelineDynamicStateCreateInfo {

    VkPipelineDynamicStateCreateFlags   flags;
    std::vector<VkDynamicState>         dynamicStates;

  };

}
