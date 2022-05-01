#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct PipelineTessellationStateCreateInfo {

    VkPipelineTessellationStateCreateFlags  flags;
    uint32_t                                patchControlPoints;

  };

}
