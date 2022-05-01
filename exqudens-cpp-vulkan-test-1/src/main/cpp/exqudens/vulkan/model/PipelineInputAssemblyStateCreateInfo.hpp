#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct PipelineInputAssemblyStateCreateInfo {

    VkPipelineInputAssemblyStateCreateFlags   flags;
    VkPrimitiveTopology                       topology;
    VkBool32                                  primitiveRestartEnable;

  };

}
