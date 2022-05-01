#pragma once

#include <vector>

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/model/SubPassDescription.hpp"

namespace exqudens::vulkan {

  struct RenderPassCreateInfo {

    VkRenderPassCreateFlags flags;
    std::vector<VkAttachmentDescription> attachments;
    std::vector<SubPassDescription> subPasses;
    std::vector<VkSubpassDependency> dependencies;

  };

}
