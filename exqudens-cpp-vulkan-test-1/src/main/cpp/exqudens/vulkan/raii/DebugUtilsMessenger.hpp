/*

  Copyright (c) 2022 Exqudens. All rights reserved.

  Created by Anrii Gorin on 2022-04-26.

*/

#pragma once

#include <optional>
#include <vulkan/vulkan_raii.hpp>

namespace exqudens::vulkan::raii {

  struct DebugUtilsMessenger {

    vk::DebugUtilsMessengerCreateInfoEXT createInfo;
    std::optional<vk::raii::DebugUtilsMessengerEXT> optional;

  };

}
