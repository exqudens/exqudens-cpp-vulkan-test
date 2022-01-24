#pragma once

#include <string>
#include <vector>
#include <cstdlib>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "exqudens/vulkan/model/Vertex.hpp"

namespace exqudens::glfw {

  using Vertex = exqudens::vulkan::model::Vertex;

  class Application {

    private:

      std::vector<std::string> commandLineArguments;
      GLFWwindow* window = nullptr;

    public:

      Application(
          int argc,
          char** argv
      ):
          Application(
              Application::toVector(argc, argv)
          )
      {
      }

      explicit Application(
          std::vector<std::string> commandLineArguments
      ):
          commandLineArguments(std::move(commandLineArguments))
      {
      }

      int run() {
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

    private:

      static std::vector<std::string> toVector(int argc, char** argv) {
        std::vector<std::string> results;
        for (int i = 0; i < argc; i++) {
          std::string result(argv[i]);
          results.emplace_back(result);
        }
        return results;
      }

      static void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
        auto app = reinterpret_cast<Application*>(glfwGetWindowUserPointer(window));
      }

      void initWindow(
          int width = 800,
          int height = 600,
          std::string title = "Vulkan"
      ) {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
      }

      void initVulkan() {
        // TODO
      }

      void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
          glfwPollEvents();
          int width = 0, height = 0;
          glfwGetFramebufferSize(window, &width, &height);
          while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
          }
        }
      }

      void cleanup() {
        glfwDestroyWindow(window);
        glfwTerminate();
      }

  };

}
