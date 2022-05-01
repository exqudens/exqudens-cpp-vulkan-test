#pragma once

#include "exqudens/vulkan/model/Device.hpp"
#include "exqudens/vulkan/model/Configuration.hpp"

namespace exqudens::vulkan {

  class DeviceFactory {

    public:

      virtual Device createDevice(
          VkPhysicalDevice& physicalDevice,
          Configuration& configuration,
          QueueFamilyIndexInfo& queueFamilyIndexInfo
      ) = 0;

      virtual void destroyDevice(Device& device) = 0;

  };

}
