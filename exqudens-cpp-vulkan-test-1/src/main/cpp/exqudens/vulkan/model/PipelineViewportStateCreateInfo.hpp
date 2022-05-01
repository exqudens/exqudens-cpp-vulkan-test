#pragma once

#include <vector>

namespace exqudens::vulkan {

  struct PipelineViewportStateCreateInfo {

    VkPipelineViewportStateCreateFlags  flags;
    std::vector<VkViewport>             viewports;
    std::vector<VkRect2D>               scissors;

  };

}
