#pragma once

#include <vector>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct WriteDescriptorSet {

    uint32_t dstBinding;
    uint32_t dstArrayElement;
    uint32_t descriptorCount;
    VkDescriptorType descriptorType;
    std::vector<VkDescriptorImageInfo> imageInfo;
    std::vector<VkDescriptorBufferInfo> bufferInfo;
    std::vector<VkBufferView> texelBufferView;

  };

}
