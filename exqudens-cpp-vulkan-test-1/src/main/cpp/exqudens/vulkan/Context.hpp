#pragma once

#include "exqudens/vulkan/Factory.hpp"

namespace exqudens::vulkan {

  class Context:
      virtual public Factory
  {

    public:

      virtual Surface add(const Surface& surface) = 0;

      virtual void destroy() = 0;

      ~Context() override = default;

  };

}
