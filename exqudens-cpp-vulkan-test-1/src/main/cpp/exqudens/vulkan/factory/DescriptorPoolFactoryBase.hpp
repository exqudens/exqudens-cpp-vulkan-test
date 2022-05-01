#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/DescriptorPoolFactory.hpp"

namespace exqudens::vulkan {

  class DescriptorPoolFactoryBase:
      virtual public DescriptorPoolFactory,
      virtual public UtilityBase
  {

    public:

      DescriptorPool createDescriptorPool(VkDevice& device, uint32_t maxSets) override {
        try {
          return createDescriptorPool(
              device,
              DescriptorPoolCreateInfo {
                  .maxSets = maxSets,
                  .poolSizes = {
                      VkDescriptorPoolSize {
                          .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                          .descriptorCount = maxSets
                      },
                      VkDescriptorPoolSize {
                          .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                          .descriptorCount = maxSets
                      }
                  }
              }
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      DescriptorPool createDescriptorPool(VkDevice& device, const DescriptorPoolCreateInfo& createInfo) override {
        try {
          VkDescriptorPool descriptorPool = nullptr;

          VkDescriptorPoolCreateInfo poolInfo = {};
          poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
          poolInfo.poolSizeCount = static_cast<uint32_t>(createInfo.poolSizes.size());
          poolInfo.pPoolSizes = createInfo.poolSizes.data();
          poolInfo.maxSets = createInfo.maxSets;
          poolInfo.flags = createInfo.flags;

          if (
              functions().createDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS
              || descriptorPool == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create descriptor pool!");
          }

          return {
              .device = device,
              .value = descriptorPool
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyDescriptorPool(DescriptorPool& descriptorPool) override {
        try {
          if (descriptorPool.value != nullptr) {
            functions().destroyDescriptorPool(descriptorPool.device, descriptorPool.value, nullptr);
            descriptorPool.device = nullptr;
            descriptorPool.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
