#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "exqudens/vulkan/model/PipelineInputAssemblyStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineTessellationStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineViewportStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineRasterizationStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineMultisampleStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineDepthStencilStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineColorBlendStateCreateInfo.hpp"
#include "exqudens/vulkan/model/PipelineDynamicStateCreateInfo.hpp"

namespace exqudens::vulkan {

  struct GraphicsPipelineCreateInfo {

    VkPipelineCreateFlags                                   flags;
    std::optional<PipelineInputAssemblyStateCreateInfo>     inputAssemblyState;
    std::optional<PipelineTessellationStateCreateInfo>      tessellationState;
    std::optional<PipelineViewportStateCreateInfo>          viewportState;
    std::optional<PipelineRasterizationStateCreateInfo>     rasterizationState;
    std::optional<PipelineMultisampleStateCreateInfo>       multisampleState;
    std::optional<PipelineDepthStencilStateCreateInfo>      depthStencilState;
    std::optional<PipelineColorBlendStateCreateInfo>        colorBlendState;
    std::optional<PipelineDynamicStateCreateInfo>           dynamicState;
    VkRenderPass                                            renderPass;
    uint32_t                                                subpass;
    VkPipeline                                              basePipelineHandle;
    int32_t                                                 basePipelineIndex;

  };

}
