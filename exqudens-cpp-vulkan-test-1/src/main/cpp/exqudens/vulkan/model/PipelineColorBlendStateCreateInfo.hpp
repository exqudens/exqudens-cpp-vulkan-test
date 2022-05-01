#pragma once

#include <array>
#include <vector>

namespace exqudens::vulkan {

  struct PipelineColorBlendStateCreateInfo {

    VkPipelineColorBlendStateCreateFlags              flags;
    VkBool32                                          logicOpEnable;
    VkLogicOp                                         logicOp;
    std::vector<VkPipelineColorBlendAttachmentState>  attachments;
    std::array<float, 4>                              blendConstants;

  };

}
