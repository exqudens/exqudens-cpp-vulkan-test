#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct SubPassDescription {

    VkSubpassDescriptionFlags flags;
    VkPipelineBindPoint pipelineBindPoint;
    std::vector<VkAttachmentReference> inputAttachments;
    std::vector<VkAttachmentReference> colorAttachments;
    std::vector<VkAttachmentReference> resolveAttachments;
    std::optional<VkAttachmentReference> depthStencilAttachment;
    std::vector<uint32_t> preserveAttachments;

  };

}
