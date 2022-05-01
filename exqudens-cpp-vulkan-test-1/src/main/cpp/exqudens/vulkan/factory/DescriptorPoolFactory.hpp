#pragma once

#include "exqudens/vulkan/model/DescriptorPoolCreateInfo.hpp"
#include "exqudens/vulkan/model/DescriptorPool.hpp"

namespace exqudens::vulkan {

  class DescriptorPoolFactory {

    public:

      virtual DescriptorPool createDescriptorPool(VkDevice& device, uint32_t maxSets) = 0;
      virtual DescriptorPool createDescriptorPool(VkDevice& device, const DescriptorPoolCreateInfo& createInfo) = 0;

      virtual void destroyDescriptorPool(DescriptorPool& descriptorPool) = 0;

  };

}
