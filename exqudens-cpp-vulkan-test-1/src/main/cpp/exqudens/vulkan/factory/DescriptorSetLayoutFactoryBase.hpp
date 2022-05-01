#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/DescriptorSetLayoutFactory.hpp"

namespace exqudens::vulkan {

  class DescriptorSetLayoutFactoryBase:
      virtual public DescriptorSetLayoutFactory,
      virtual public UtilityBase
  {

    public:

      DescriptorSetLayout createDescriptorSetLayout(VkDevice& device) override {
        try {
          return createDescriptorSetLayout(
              device,
              DescriptorSetLayoutCreateInfo {
                  .flags = 0,
                  .bindings = {
                      VkDescriptorSetLayoutBinding {
                          .binding = 0,
                          .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                          .descriptorCount = 1,
                          .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                          .pImmutableSamplers = nullptr
                      },
                      VkDescriptorSetLayoutBinding {
                          .binding = 1,
                          .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                          .descriptorCount = 1,
                          .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                          .pImmutableSamplers = nullptr
                      }
                  }
              }
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      DescriptorSetLayout createDescriptorSetLayout(
          VkDevice& device,
          const DescriptorSetLayoutCreateInfo& createInfo
      ) override {
        try {
          VkDescriptorSetLayout descriptorSetLayout = nullptr;

          VkDescriptorSetLayoutCreateInfo layoutInfo = {};
          layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
          layoutInfo.flags = createInfo.flags;
          layoutInfo.bindingCount = static_cast<uint32_t>(createInfo.bindings.size());
          layoutInfo.pBindings = createInfo.bindings.data();

          if (
              functions().createDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS
              || descriptorSetLayout == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create descriptor set layout!");
          }

          return {
              .device = device,
              .value = descriptorSetLayout
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDescriptorSetLayout(DescriptorSetLayout& descriptorSetLayout) override {
        try {
          if (descriptorSetLayout.value != nullptr) {
            functions().destroyDescriptorSetLayout(descriptorSetLayout.device, descriptorSetLayout.value, nullptr);
            descriptorSetLayout.device = nullptr;
            descriptorSetLayout.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
