#pragma once

#include <cstring>
#include <string>
#include <optional>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <format>

#include <gtest/gtest.h>
#include <vulkan/vulkan_raii.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "exqudens/TestUtils.hpp"
#include "exqudens/test/model/Vertex.hpp"
#include "exqudens/vulkan/raii/Utility.hpp"
#include "exqudens/vulkan/raii/Factory.hpp"

namespace exqudens::vulkan::raii {

  class UiTestsB : public testing::Test {

    protected:

      class Environment {

        public:

          bool resized = false;

        private:

          std::vector<Vertex> vertices = {};
          std::vector<uint16_t> indices = {};

          Context context = {};
          Instance instance = {};
          DebugUtilsMessenger debugUtilsMessenger = {};

          //std::optional<vk::raii::Context> context = {};
          //std::optional<vk::raii::Instance> instance = {};
          //std::optional<vk::raii::DebugUtilsMessengerEXT> debugUtilsMessenger = {};
          std::optional<vk::raii::SurfaceKHR> surface = {};

          std::vector<vk::raii::PhysicalDevice> physicalDevices = {};
          std::optional<vk::PhysicalDeviceFeatures> physicalDeviceEnabledFeatures = {};
          vk::raii::PhysicalDevice* physicalDevice = nullptr;

          std::vector<std::uint32_t> transferQueueFamilyIndices = {};
          std::vector<std::uint32_t> graphicsQueueFamilyIndices = {};
          std::vector<std::uint32_t> presentQueueFamilyIndices = {};

          std::optional<vk::raii::Device> device = {};

          std::optional<vk::raii::SwapchainKHR> swapChain = {};

          std::optional<vk::raii::Buffer> vertexStagingBuffer = {};
          std::optional<vk::raii::DeviceMemory> vertexStagingBufferMemory = {};
          std::optional<vk::raii::Buffer> vertexBuffer = {};
          std::optional<vk::raii::DeviceMemory> vertexBufferMemory = {};

          std::optional<vk::raii::Buffer> indexStagingBuffer = {};
          std::optional<vk::raii::DeviceMemory> indexStagingBufferMemory = {};
          std::optional<vk::raii::Buffer> indexBuffer = {};
          std::optional<vk::raii::DeviceMemory> indexBufferMemory = {};

          std::optional<vk::raii::CommandPool> transferCommandPool = {};
          std::vector<vk::raii::CommandBuffer> transferCommandBuffers = {};
          std::optional<vk::raii::Queue> transferQueue = {};

          std::optional<vk::raii::CommandPool> graphicsCommandPool = {};
          std::vector<vk::raii::CommandBuffer> graphicsCommandBuffers = {};
          std::optional<vk::raii::Queue> graphicsQueue = {};

        public:

          void create(const std::vector<std::string>& arguments, GLFWwindow* window, uint32_t width, uint32_t height) {
            try {
              vertices = {
                  {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                  {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                  {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                  {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},

                  {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
                  {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
                  {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
                  {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
              };

              indices = {
                  0, 1, 2, 2, 3, 0,
                  4, 5, 6, 6, 7, 4
              };

              Utility::setEnvironmentVariable("VK_LAYER_PATH", arguments.front());

              context = Factory::createContext();

              uint32_t glfwExtensionCount = 0;
              const char** glfwExtensions;
              glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
              std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
              for (const char*& extension : extensions) {
                context.info.enabledExtensionNames.emplace_back(extension);
              }

              instance = Factory::createInstance(
                  context,
                  vk::ApplicationInfo()
                      .setPApplicationName("Exqudens Application")
                      .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
                      .setPEngineName("Exqudens Engine")
                      .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
                      .setApiVersion(VK_API_VERSION_1_0),
                  vk::InstanceCreateInfo()
                      .setPEnabledExtensionNames(context.info.enabledExtensionNames)
                      .setPEnabledLayerNames(context.info.enabledLayerNames)
              );

              debugUtilsMessenger = Factory::createDebugUtilsMessenger(
                  instance,
                  vk::DebugUtilsMessengerCreateInfoEXT()
                      .setPUserData(nullptr)
                      .setPfnUserCallback(&Utility::debugCallback)
                      .setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
                      .setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
              );

              VkSurfaceKHR vkSurface = nullptr;
              auto vkInstance = static_cast<VkInstance>(*instance.optional.value());
              if (glfwCreateWindowSurface(vkInstance, window, nullptr, &vkSurface) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to create surface!");
              }
              if (vkSurface == nullptr) {
                throw std::runtime_error(CALL_INFO() + ": surface is null!");
              }
              surface = vk::raii::SurfaceKHR(*instance.optional, vkSurface);

              physicalDevices = vk::raii::PhysicalDevices(instance.optional.value());
              physicalDeviceEnabledFeatures = vk::PhysicalDeviceFeatures()
                  .setSamplerAnisotropy(context.info.samplerAnisotropyEnabled);
              std::size_t physicalDeviceIndex = Utility::getPhysicalDeviceIndices(
                  physicalDevices,
                  {vk::QueueFlagBits::eCompute, vk::QueueFlagBits::eTransfer, vk::QueueFlagBits::eGraphics},
                  &surface.value(),
                  context.info.enabledDeviceExtensionNames,
                  (*physicalDeviceEnabledFeatures).samplerAnisotropy
              ).front();
              physicalDevice = &physicalDevices.at(physicalDeviceIndex);

              transferQueueFamilyIndices = Utility::getQueueFamilyIndices(
                  *physicalDevice,
                  vk::QueueFlagBits::eTransfer,
                  nullptr
              );
              graphicsQueueFamilyIndices = Utility::getQueueFamilyIndices(
                  *physicalDevice,
                  vk::QueueFlagBits::eGraphics,
                  nullptr
              );
              presentQueueFamilyIndices = Utility::getQueueFamilyIndices(
                  *physicalDevice,
                  {},
                  &surface.value()
              );

              std::set<std::uint32_t> uniqueQueueFamilyIndices = {
                  transferQueueFamilyIndices.front(),
                  graphicsQueueFamilyIndices.front(),
                  presentQueueFamilyIndices.front()
              };
              std::vector<vk::DeviceQueueCreateInfo> queueCreateInfo;
              float queuePriority = 1.0f;
              for (const uint32_t& queueFamilyIndex : uniqueQueueFamilyIndices) {
                vk::DeviceQueueCreateInfo info = vk::DeviceQueueCreateInfo()
                    .setQueueFamilyIndex(queueFamilyIndex)
                    .setQueueCount(1)
                    .setQueuePriorities(queuePriority);
                queueCreateInfo.emplace_back(info);
              }
              device = vk::raii::Device(
                  *physicalDevice,
                  vk::DeviceCreateInfo()
                      .setQueueCreateInfos(queueCreateInfo)
                      .setPEnabledFeatures(&physicalDeviceEnabledFeatures.value())
                      .setPEnabledExtensionNames(context.info.enabledDeviceExtensionNames)
                      .setPEnabledLayerNames(context.info.enabledLayerNames)
              );

              vk::SurfaceFormatKHR surfaceFormat = Utility::surfaceFormat(physicalDevice->getSurfaceFormatsKHR(*surface.value())).value();
              vk::PresentModeKHR surfacePresentMode = Utility::surfacePresentMode(physicalDevice->getSurfacePresentModesKHR(*surface.value())).value();
              vk::SurfaceCapabilitiesKHR surfaceCapabilities = physicalDevice->getSurfaceCapabilitiesKHR(*surface.value());
              vk::Extent2D surfaceExtent = Utility::surfaceExtent(surfaceCapabilities, width, height).value();
              vk::SurfaceTransformFlagBitsKHR surfaceTransform = Utility::surfaceTransform(surfaceCapabilities).value();
              vk::CompositeAlphaFlagBitsKHR surfaceCompositeAlpha = Utility::surfaceCompositeAlpha(surfaceCapabilities).value();
              swapChain = vk::raii::SwapchainKHR(
                  *device,
                  vk::SwapchainCreateInfoKHR()
                    .setFlags({})
                    .setSurface(*surface.value())
                    .setMinImageCount(surfaceCapabilities.minImageCount)
                    .setImageFormat(surfaceFormat.format)
                    .setImageColorSpace(surfaceFormat.colorSpace)
                    .setImageExtent(surfaceExtent)
                    .setImageArrayLayers(1)
                    .setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
                    .setImageSharingMode(vk::SharingMode::eExclusive)
                    .setQueueFamilyIndices({})
                    .setPreTransform(surfaceTransform)
                    .setCompositeAlpha(surfaceCompositeAlpha)
                    .setPresentMode(surfacePresentMode)
                    .setClipped(true)
                    .setOldSwapchain({})
              );

              vertexStagingBuffer = vk::raii::Buffer(
                  *device,
                  vk::BufferCreateInfo()
                    .setFlags({})
                    .setSize(sizeof(vertices[0]) * vertices.size())
                    .setUsage(vk::BufferUsageFlagBits::eTransferSrc)
                    .setSharingMode(vk::SharingMode::eExclusive)
                    .setQueueFamilyIndices({})
              );
              vertexStagingBufferMemory = vk::raii::DeviceMemory(
                  *device,
                  vk::MemoryAllocateInfo()
                    .setAllocationSize(vertexStagingBuffer.value().getMemoryRequirements().size)
                    .setMemoryTypeIndex(
                        Utility::memoryType(
                            physicalDevice->getMemoryProperties(),
                            vertexStagingBuffer.value().getMemoryRequirements().memoryTypeBits,
                            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent
                        )
                    )
              );
              void* vertexData = vertexStagingBufferMemory
                  .value()
                  .mapMemory(0, vertexStagingBuffer.value().getMemoryRequirements().size);
              std::memcpy(vertexData, vertices.data(), sizeof(vertices[0]) * vertices.size());
              vertexStagingBufferMemory.value().unmapMemory();

              transferCommandPool = vk::raii::CommandPool(
                  *device,
                  vk::CommandPoolCreateInfo()
                    .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                    .setQueueFamilyIndex(transferQueueFamilyIndices.front())
              );
              transferCommandBuffers = vk::raii::CommandBuffers(
                  *device,
                  vk::CommandBufferAllocateInfo()
                    .setCommandPool(*transferCommandPool.value())
                    .setLevel(vk::CommandBufferLevel::ePrimary)
                    .setCommandBufferCount(1)
              );
              transferQueue = vk::raii::Queue(*device, transferQueueFamilyIndices.front(), 0);

              graphicsCommandPool = vk::raii::CommandPool(
                  *device,
                  vk::CommandPoolCreateInfo()
                      .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
                      .setQueueFamilyIndex(transferQueueFamilyIndices.front())
              );
              graphicsCommandBuffers = vk::raii::CommandBuffers(
                  *device,
                  vk::CommandBufferAllocateInfo()
                      .setCommandPool(*graphicsCommandPool.value())
                      .setLevel(vk::CommandBufferLevel::ePrimary)
                      .setCommandBufferCount(1)
              );
              graphicsQueue = vk::raii::Queue(*device, graphicsQueueFamilyIndices.front(), 0);

              // TODO
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void drawFrame(int width, int height) {
            try {
              // TODO
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void waitIdle() {
            try {
              // TODO
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void destroy() {
            try {
              // TODO
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

      };

      class TestUiApplication {

        public:

          std::vector<std::string> arguments = {};
          Environment* environment = nullptr;

          TestUiApplication(const int& argc, char** argv) {
            try {
              for (std::size_t i = 0; i < argc; i++) {
                arguments.emplace_back(std::string(argv[i]));
              }
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          int run() {
            try {
              int width = 800;
              int height = 600;
              std::string title = "Vulkan";

              glfwInit();
              glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
              //glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

              GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

              glfwSetWindowUserPointer(window, this);
              glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

              environment = new Environment();
              environment->create(arguments, window, static_cast<uint32_t>(width), static_cast<uint32_t>(height));

              while (!glfwWindowShouldClose(window)) {
                glfwPollEvents();
                glfwGetFramebufferSize(window, &width, &height);
                while (width == 0 || height == 0) {
                  glfwGetFramebufferSize(window, &width, &height);
                  glfwWaitEvents();
                }
                environment->drawFrame(width, height);
              }
              environment->waitIdle();

              environment->destroy();
              delete environment;
              glfwDestroyWindow(window);
              glfwTerminate();

              return EXIT_SUCCESS;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

        private:

          static void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
            try {
              auto* app = reinterpret_cast<TestUiApplication*>(glfwGetWindowUserPointer(window));
              app->environment->resized = true;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

      };

      void SetUp() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void TearDown() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  TEST_F(UiTestsB, test1) {
    try {
      std::string executableDir = TestUtils::getExecutableDir();
      std::vector<char*> arguments = {executableDir.data()};
      int argc = static_cast<int>(arguments.size());
      char** argv = &arguments[0];
      int result = TestUiApplication(argc, argv).run();
      ASSERT_EQ(EXIT_SUCCESS, result);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
