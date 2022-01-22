#include <stdexcept>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <utility>
#include <chrono>

#include "exqudens/glfw/Application.hpp"

namespace exqudens::glfw {

  Application::Application(
      int argc,
      char** argv
  ):
      Application(
          Application::toVector(argc, argv)
      )
  {
  }

  Application::Application(
      std::vector<std::string> commandLineArguments
  ):
      commandLineArguments(std::move(commandLineArguments)),
      window(nullptr),
      vulkanEnvironment(nullptr)
  {
  }

  int Application::run() {
    try {
      initWindow();
      initVulkan();
      mainLoop();
      cleanup();
      return EXIT_SUCCESS;
    } catch (const std::exception& e) {
      std::cerr << e.what() << std::endl;
      return EXIT_FAILURE;
    }
  }

  std::vector<std::string> Application::toVector(int argc, char** argv) {
    std::vector<std::string> results;
    for (int i = 0; i < argc; i++) {
      std::string result(argv[i]);
      results.emplace_back(result);
    }
    return results;
  }

  void Application::frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
    app->vulkanEnvironment->setFrameBufferResized();
  }

  void Application::initWindow(
      int width,
      int height,
      std::string title
  ) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
  }

  void Application::initVulkan() {
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    std::vector<const char*> requiredExtensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    bool validationLayersEnabled = true;
    std::vector<std::string> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    std::vector<std::string> extensions;
    std::transform(
        requiredExtensions.begin(),
        requiredExtensions.end(),
        std::back_inserter(extensions),
        [] (const char* c) -> std::string { return std::string(c); }
    );
    std::vector<std::string> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    GLFWwindow* w = window;

    std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction = [w](
        VkInstance instance
    ) {
      VkSurfaceKHR surface;
      if (glfwCreateWindowSurface(instance, w, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface!");
      }
      return surface;
    };
    std::function<VkBool32(
        VkDebugUtilsMessageSeverityFlagBitsEXT,
        VkDebugUtilsMessageTypeFlagsEXT,
        std::string
    )> logFunction = [](
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        std::string message
    ) {
      //std::string function = std::string("(") + __FUNCTION__ + ")";

      std::string level;
      if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT == messageSeverity) {
        level = "[VERBOSE]";
      } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT == messageSeverity) {
        level = "[INFO]";
      } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT == messageSeverity) {
        level = "[WARNING]";
      } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT == messageSeverity) {
        level = "[ERROR]";
      } else if (VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT == messageSeverity) {
        level = "[MAX]";
      }

      std::string type;
      if (VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT == messageType) {
        type = "(GENERAL)";
      } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT == messageType) {
        type = "(VALIDATION)";
      } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT == messageType) {
        type = "(PERFORMANCE)";
      } else if (VK_DEBUG_UTILS_MESSAGE_TYPE_FLAG_BITS_MAX_ENUM_EXT == messageType) {
        type = "(MAX)";
      }

      std::string line;

      //line += function;
      //line += " ";
      line += level;
      line += " ";
      line += type;
      line += " ";
      line += "validation layer:";
      line += " ";
      line += message;

      std::cerr << line << std::endl;

      return VK_FALSE;
    };

    vulkanEnvironment = new exqudens::vulkan::Environment(
        validationLayersEnabled,

        width,
        height,

        commandLineArguments,
        validationLayers,
        extensions,
        deviceExtensions,

        createSurfaceFunction,
        logFunction
    );
  }

  void Application::mainLoop() {
    auto start = std::chrono::high_resolution_clock::now();
    bool updated = false;
    std::vector<Vertex> vertices1({
        {{-0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    });
    std::vector<uint16_t> indices1({
        0, 1, 2, 2, 3, 0
    });
    std::vector<Vertex> vertices2({
        {{-0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    });
    std::vector<uint16_t> indices2({
        0, 1, 2, 2, 3, 0
    });
    int index = 1;
    while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      int width = 0, height = 0;
      glfwGetFramebufferSize(window, &width, &height);
      while (width == 0 || height == 0) {
        glfwGetFramebufferSize(window, &width, &height);
        glfwWaitEvents();
      }
      vulkanEnvironment->setFrameBufferWidth(width);
      vulkanEnvironment->setFrameBufferHeight(height);
      auto current = std::chrono::high_resolution_clock::now();
      if (!updated && std::chrono::duration_cast<std::chrono::seconds>(current-start).count() > 2) {
        start = current;
        if (index == 1) {
          vulkanEnvironment->update(vertices1, indices1);
          index = 2;
        } else if (index == 2) {
          vulkanEnvironment->update(vertices2, indices2);
          index = 1;
        }
        //updated = true;
      }
      vulkanEnvironment->drawFrame();
    }
    vulkanEnvironment->waitIdle();
  }

  void Application::cleanup() {
    delete vulkanEnvironment;
    glfwDestroyWindow(window);
    glfwTerminate();
  }

}
