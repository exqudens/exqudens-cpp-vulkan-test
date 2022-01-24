#pragma once

#include <optional>

namespace exqudens::vulkan::model {

  struct QueueFamilyIndices {

    std::optional<uint32_t> transferFamily;
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
      return graphicsFamily.has_value() && presentFamily.has_value() && transferFamily.has_value();
    }

  };

}
