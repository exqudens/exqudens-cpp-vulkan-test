/*

  Copyright (c) 2022 Exqudens. All rights reserved.

  Created by Anrii Gorin on 2022-04-26.

*/

#pragma once

#include <optional>

#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan::raii {

  struct Instance {

    vk::ApplicationInfo applicationInfo;
    vk::InstanceCreateInfo createInfo;
    std::optional<vk::raii::Instance> optional;

  };

}
