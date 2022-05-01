#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/DescriptorSetFactory.hpp"

namespace exqudens::vulkan {

  class DescriptorSetFactoryBase:
      virtual public DescriptorSetFactory,
      virtual public UtilityBase
  {

    public:

      DescriptorSet createDescriptorSet(
          VkDevice& device,
          VkDescriptorPool& descriptorPool,
          VkDescriptorSetLayout& descriptorSetLayout,
          const std::vector<WriteDescriptorSet>& writeDescriptorSets
      ) override {
        try {
          VkDescriptorSet descriptorSet = nullptr;

          VkDescriptorSetAllocateInfo allocInfo = {};
          allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
          allocInfo.descriptorPool = descriptorPool;
          allocInfo.descriptorSetCount = descriptorSetLayout == nullptr ? 0 : 1;
          allocInfo.pSetLayouts = descriptorSetLayout == nullptr ? nullptr : &descriptorSetLayout;

          if (
              functions().allocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS
              || descriptorSet == nullptr
          ) {
            throw std::runtime_error(CALL_INFO()+ ": failed to allocate descriptor sets!");
          }

          std::vector<VkWriteDescriptorSet> writes;
          writes.resize(writeDescriptorSets.size());

          for (std::size_t i = 0; i < writes.size(); i++) {
            writes[i] = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .pNext = nullptr,
                .dstSet = descriptorSet,
                .dstBinding = writeDescriptorSets[i].dstBinding,
                .dstArrayElement = writeDescriptorSets[i].dstArrayElement,
                .descriptorCount = writeDescriptorSets[i].descriptorCount,
                .descriptorType = writeDescriptorSets[i].descriptorType,
                .pImageInfo = writeDescriptorSets[i].imageInfo.empty() ? nullptr : writeDescriptorSets[i].imageInfo.data(),
                .pBufferInfo = writeDescriptorSets[i].bufferInfo.empty() ? nullptr : writeDescriptorSets[i].bufferInfo.data(),
                .pTexelBufferView = writeDescriptorSets[i].texelBufferView.empty() ? nullptr : writeDescriptorSets[i].texelBufferView.data()
            };
          }

          functions().updateDescriptorSets(device, static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);

          return {
              .value = descriptorSet
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDescriptorSet(DescriptorSet& descriptorSet) override {
        try {
          if (descriptorSet.value != nullptr) {
            descriptorSet.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDescriptorSets(std::vector<DescriptorSet>& descriptorSets) override {
        try {
          for (DescriptorSet& descriptorSet : descriptorSets) {
            descriptorSet.value = nullptr;
          }
          descriptorSets.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
