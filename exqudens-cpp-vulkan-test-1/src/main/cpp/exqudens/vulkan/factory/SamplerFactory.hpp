#pragma once

#include "exqudens/vulkan/model/Sampler.hpp"

namespace exqudens::vulkan {

  class SamplerFactory {

    public:

      virtual Sampler createSampler(VkPhysicalDevice& physicalDevice, VkDevice& device) = 0;

      virtual Sampler createSampler(VkPhysicalDevice& physicalDevice, VkDevice& device, bool anisotropyEnable) = 0;

      virtual void destroySampler(Sampler& sampler) = 0;

  };

}
