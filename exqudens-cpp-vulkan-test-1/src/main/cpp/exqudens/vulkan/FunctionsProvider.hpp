#pragma once

#include "exqudens/vulkan/model/Functions.hpp"

namespace exqudens::vulkan {

  class FunctionsProvider {

    public:

      virtual Functions functions() = 0;

      virtual ~FunctionsProvider() = default;

  };

}
