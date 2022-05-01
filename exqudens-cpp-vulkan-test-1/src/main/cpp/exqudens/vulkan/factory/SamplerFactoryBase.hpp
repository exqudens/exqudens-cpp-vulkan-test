#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/SamplerFactory.hpp"

namespace exqudens::vulkan {

  class SamplerFactoryBase:
      virtual public SamplerFactory,
      virtual public UtilityBase
  {

    public:

      Sampler createSampler(VkPhysicalDevice& physicalDevice, VkDevice& device) override {
        try {
          return createSampler(physicalDevice, device, false);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Sampler createSampler(VkPhysicalDevice& physicalDevice, VkDevice& device, bool anisotropyEnable) override {
        try {
          VkSampler sampler = nullptr;

          VkSamplerCreateInfo samplerInfo = {};
          samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
          samplerInfo.magFilter = VK_FILTER_LINEAR;
          samplerInfo.minFilter = VK_FILTER_LINEAR;
          samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
          samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
          samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;

          if (anisotropyEnable) {
            VkPhysicalDeviceProperties properties = {};
            functions().getPhysicalDeviceProperties(physicalDevice, &properties);

            samplerInfo.anisotropyEnable = VK_TRUE;
            samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
          }

          samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
          samplerInfo.unnormalizedCoordinates = VK_FALSE;
          samplerInfo.compareEnable = VK_FALSE;
          samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
          samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

          if (
              functions().createSampler(device, &samplerInfo, nullptr, &sampler) != VK_SUCCESS
              || sampler == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create sampler!");
          }

          return {
              .device = device,
              .value = sampler
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySampler(Sampler& sampler) override {
        try {
          if (sampler.value != nullptr) {
            functions().destroySampler(sampler.device, sampler.value, nullptr);
            sampler.device = nullptr;
            sampler.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
