#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct PipelineMultisampleStateCreateInfo {

    VkPipelineMultisampleStateCreateFlags    flags;
    VkSampleCountFlagBits                    rasterizationSamples;
    VkBool32                                 sampleShadingEnable;
    float                                    minSampleShading;
    const VkSampleMask*                      pSampleMask;
    VkBool32                                 alphaToCoverageEnable;
    VkBool32                                 alphaToOneEnable;

  };

}
