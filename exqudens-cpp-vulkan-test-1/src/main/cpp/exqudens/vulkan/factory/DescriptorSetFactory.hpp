#pragma once

#include "exqudens/vulkan/model/WriteDescriptorSet.hpp"
#include "exqudens/vulkan/model/DescriptorSet.hpp"

namespace exqudens::vulkan {

  class DescriptorSetFactory {

    public:

      virtual DescriptorSet createDescriptorSet(
          VkDevice& device,
          VkDescriptorPool& descriptorPool,
          VkDescriptorSetLayout& descriptorSetLayout,
          const std::vector<WriteDescriptorSet>& writeDescriptorSets
      ) = 0;

      virtual void destroyDescriptorSet(DescriptorSet& descriptorSet) = 0;
      virtual void destroyDescriptorSets(std::vector<DescriptorSet>& descriptorSets) = 0;

  };

}
