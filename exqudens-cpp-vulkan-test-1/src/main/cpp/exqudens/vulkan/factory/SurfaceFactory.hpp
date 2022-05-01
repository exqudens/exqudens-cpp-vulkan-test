#pragma once

#include <vulkan/vulkan.h>

#include "exqudens/vulkan/model/Surface.hpp"

namespace exqudens::vulkan {

  class SurfaceFactory {

    public:

      virtual Surface createSurface(VkInstance& instance) = 0;

      virtual void destroySurface(Surface& surface) = 0;

  };

}
