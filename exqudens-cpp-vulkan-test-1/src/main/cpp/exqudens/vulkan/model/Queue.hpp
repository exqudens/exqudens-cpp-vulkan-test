#pragma once

#include <cstdint>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Queue {

    unsigned int id;
    bool destroyed;
    uint32_t index;
    uint32_t familyIndex;
    VkQueue value;

  };

}
