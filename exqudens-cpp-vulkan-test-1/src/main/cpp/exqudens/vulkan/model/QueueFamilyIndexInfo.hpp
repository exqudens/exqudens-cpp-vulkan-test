#pragma once

#include <optional>
#include <cstdint>
#include <set>

namespace exqudens::vulkan {

  struct QueueFamilyIndexInfo {

    bool computeFamilyRequired;
    std::optional<uint32_t> computeFamily;
    uint32_t computeFamilyQueueCount;

    bool transferFamilyRequired;
    std::optional<uint32_t> transferFamily;
    uint32_t transferFamilyQueueCount;

    bool graphicsFamilyRequired;
    std::optional<uint32_t> graphicsFamily;
    uint32_t graphicsFamilyQueueCount;

    bool presentFamilyRequired;
    std::optional<uint32_t> presentFamily;
    uint32_t presentFamilyQueueCount;

    bool complete;
    std::set<uint32_t> uniqueQueueFamilyIndices;

  };

}
