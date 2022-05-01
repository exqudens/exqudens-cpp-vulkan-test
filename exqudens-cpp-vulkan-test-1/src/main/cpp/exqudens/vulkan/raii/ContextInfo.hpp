/*

  Copyright (c) 2022 Exqudens. All rights reserved.

  Created by Anrii Gorin on 2022-04-26.

*/

#pragma once

#include <vector>

namespace exqudens::vulkan::raii {

  struct ContextInfo {

    std::vector<const char*> enabledLayerNames;
    std::vector<const char*> enabledExtensionNames;
    std::vector<const char*> enabledDeviceExtensionNames;
    bool samplerAnisotropyEnabled;

  };

}
