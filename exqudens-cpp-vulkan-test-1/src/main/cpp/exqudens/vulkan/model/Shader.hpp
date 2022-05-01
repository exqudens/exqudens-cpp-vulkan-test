#pragma once

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Shader {

    VkDevice device;
    VkShaderModule shaderModule;
    VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo;

  };

}
