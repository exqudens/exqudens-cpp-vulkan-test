#pragma once

#include <stdexcept>

#include <gtest/gtest.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/FactoryBase.hpp"

namespace exqudens::vulkan {

  class ShaderTests : public testing::Test, protected FactoryBase {
  };

  TEST_F(ShaderTests, test1) {
    try {
      std::map<std::string, std::string> environmentVariables = createEnvironmentVariables(TestUtils::getExecutableDir());

      for (auto const& [name, value] : environmentVariables) {
        setEnvironmentVariable(name, value);
      }

      Configuration configuration = createConfiguration();
      configuration.presentQueueFamilyRequired = false;
      configuration.deviceExtensions = {};
      std::ostringstream stream;
      Logger logger = createLogger(stream);

      Instance instance = createInstance(configuration, logger);
      DebugUtilsMessenger debugUtilsMessenger = createDebugUtilsMessenger(instance.value, logger);
      PhysicalDevice physicalDevice = createPhysicalDevice(instance.value, configuration);
      Device device = createDevice(physicalDevice.value, configuration, physicalDevice.queueFamilyIndexInfo);

      Shader vertexShader1 = createShader(device.value, "resources/shader/shader-1.vert.spv");
      Shader fragmentShader1 = createShader(device.value, "resources/shader/shader-1.frag.spv");

      ASSERT_TRUE(vertexShader1.shaderModule != nullptr);
      ASSERT_EQ(vertexShader1.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT);
      ASSERT_TRUE(fragmentShader1.shaderModule != nullptr);
      ASSERT_EQ(fragmentShader1.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT);

      Shader vertexShader2 = vertexShader1;
      Shader fragmentShader2 = fragmentShader1;

      ASSERT_TRUE(vertexShader2.shaderModule != nullptr);
      ASSERT_EQ(vertexShader2.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT);
      ASSERT_TRUE(fragmentShader2.shaderModule != nullptr);
      ASSERT_EQ(fragmentShader2.pipelineShaderStageCreateInfo.stage, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT);

      destroyShader(vertexShader1);
      destroyShader(fragmentShader1);

      ASSERT_TRUE(vertexShader2.shaderModule != nullptr);
      ASSERT_TRUE(fragmentShader2.shaderModule != nullptr);

      destroyDevice(device);
      destroyPhysicalDevice(physicalDevice);
      destroyDebugUtilsMessenger(debugUtilsMessenger);
      destroyInstance(instance);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
