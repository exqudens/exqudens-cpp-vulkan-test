/*

  Copyright (c) 2022 Exqudens. All rights reserved.

  Created by Anrii Gorin on 2022-04-26.

*/

#pragma once

#include <optional>

#include <vulkan/vulkan_raii.hpp>

#include "exqudens/vulkan/raii/ContextInfo.hpp"

namespace exqudens::vulkan::raii {

  struct Context {

    ContextInfo info;
    std::optional<vk::raii::Context> optional;

  };

}
