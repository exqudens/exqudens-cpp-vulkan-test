#pragma once

#include <stdexcept>

#include <gtest/gtest.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/FactoryBase.hpp"

namespace exqudens::vulkan {

  class ConfigurationTests : public testing::Test, protected FactoryBase {
  };

  TEST_F(ConfigurationTests, test1) {
    try {
      Configuration configuration = createConfiguration();

      ASSERT_EQ(true, configuration.validationLayersEnabled);
      ASSERT_EQ(std::string("VK_LAYER_KHRONOS_validation"), std::string(configuration.validationLayers[0]));
      ASSERT_EQ(std::string(VK_EXT_DEBUG_UTILS_EXTENSION_NAME), std::string(configuration.extensions[0]));
      ASSERT_EQ(std::string(VK_KHR_SWAPCHAIN_EXTENSION_NAME), std::string(configuration.deviceExtensions[0]));
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
