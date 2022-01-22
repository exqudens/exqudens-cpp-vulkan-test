#pragma once

#include <string>
#include <vector>
#include <map>

#include "vulkan/vulkan.h"

#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/Initializer.hpp"

namespace exqudens::vulkan {

  using Configuration = model::Configuration;

  class Environment {

    private:

      Configuration configuration;
      Initializer* initializer;
      bool initializerDelete;

      std::map<std::string, std::string> environmentVariables;
      std::map<std::string, std::string> environmentVariablesOld;

      Logger* logger;
      bool loggerDelete;

      VkInstance instance;
      VkDebugUtilsMessengerEXT debugMessenger;
      VkSurfaceKHR surface;

      VkPhysicalDevice physicalDevice;

      VkDevice device;
      VkQueue graphicsQueue;
      VkQueue presentQueue;

      VkSwapchainKHR swapChain;
      std::vector<VkImage> swapChainImages;
      VkFormat swapChainImageFormat;
      VkExtent2D swapChainExtent;
      std::vector<VkImageView> swapChainImageViews;

      VkRenderPass renderPass;
      VkPipelineLayout graphicsPipelineLayout;
      VkPipeline graphicsPipeline;

      std::vector<VkFramebuffer> swapChainFrameBuffers;

      VkCommandPool commandPool;

      std::vector<Vertex> vertices;
      std::vector<uint16_t> indices;
      VkBuffer vertexBuffer;
      VkDeviceMemory vertexBufferMemory;
      VkBuffer indexBuffer;
      VkDeviceMemory indexBufferMemory;

      std::vector<VkCommandBuffer> commandBuffers;

      int maxFramesInFlight;
      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      size_t currentFrame;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;

      bool frameBufferResized;

    public:

      explicit Environment(
          Configuration configuration,
          Initializer* initializer = nullptr
      );

      Configuration& getConfiguration();

      void drawFrame();

      void waitIdle();

      void setFrameBufferResized();

      ~Environment();

    private:

      void setEnvironmentVariables();
      void setLogger();
      void setInstance();
      void setDebugMessenger();
      void setSurface();
      void setPhysicalDevice();
      void setDevice();
      void setSwapChain();
      void setSwapChainImageViews();
      void setRenderPass();
      void setGraphicsPipeline();
      void setFrameBuffers();
      void setCommandPool();
      void setVertexBuffer();
      void setIndexBuffer();
      void setCommandBuffers();
      void setSyncObjects();

  };

}
