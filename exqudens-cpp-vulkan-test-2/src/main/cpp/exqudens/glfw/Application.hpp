#pragma once

#include <string>
#include <vector>

#define GLFW_INCLUDE_VULKAN
#include "GLFW/glfw3.h"

#include "exqudens/vulkan/Environment.hpp"
#include "exqudens/vulkan/model/Vertex.hpp"

namespace exqudens::glfw {

  using Vertex = exqudens::vulkan::model::Vertex;

  class Application {

    private:

      std::vector<std::string> commandLineArguments;
      GLFWwindow* window;
      exqudens::vulkan::Environment* vulkanEnvironment;

    public:

      Application(int argc, char** argv);
      explicit Application(std::vector<std::string> commandLineArguments);

      int run();

    private:

      static std::vector<std::string> toVector(int argc, char** argv);

      static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);

      void initWindow(
          int width = 800,
          int height = 600,
          std::string title = "Vulkan"
      );

      void initVulkan();

      void mainLoop();

      void cleanup();

  };

}
