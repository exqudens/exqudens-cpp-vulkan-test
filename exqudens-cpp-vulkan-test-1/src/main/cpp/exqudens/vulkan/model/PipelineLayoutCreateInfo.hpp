#pragma once

#include <vector>

namespace exqudens::vulkan {

  struct PipelineLayoutCreateInfo {

    VkPipelineLayoutCreateFlags         flags;
    std::vector<VkDescriptorSetLayout>  setLayouts;
    std::vector<VkPushConstantRange>    pushConstantRanges;

  };

}
