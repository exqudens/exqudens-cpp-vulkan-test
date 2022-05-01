#pragma once

#include <cstdint>
#include <vector>

namespace exqudens::vulkan {

  struct FrameBufferCreateInfo {

    VkFramebufferCreateFlags flags;
    VkRenderPass renderPass;
    std::vector<VkImageView> attachments;
    uint32_t width;
    uint32_t height;
    uint32_t layers;

  };

}
