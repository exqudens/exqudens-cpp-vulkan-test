#pragma once

#include <cstdint>
#include <string>
#include <optional>
#include <vector>
#include <map>
#include <tuple>
#include <exqudens/vulkan/model/UniformBufferObject.hpp>

#include "vulkan/vulkan.h"

#include "exqudens/vulkan/model/StringVector.hpp"
#include "exqudens/vulkan/Logger.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"
#include "exqudens/vulkan/model/Vertex.hpp"

namespace exqudens::vulkan {

  using StringVector = model::StringVector;
  using QueueFamilyIndices = model::QueueFamilyIndices;
  using SwapChainSupportDetails = model::SwapChainSupportDetails;
  using Vertex = model::Vertex;
  using UniformBufferObject = model::UniformBufferObject;

  class Environment {

    private:
      const int maxFramesInFlight;

      bool validationLayersEnabled;

      int frameBufferWidth;
      int frameBufferHeight;

      StringVector commandLineArguments;
      StringVector validationLayers;
      StringVector extensions;
      StringVector deviceExtensions;

      std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction;
      std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction;

      std::map<std::string, std::string> environmentVariables;
      std::map<std::string, std::string> environmentVariablesOld;

      Logger* logger;

      VkInstance instance;

      VkDebugUtilsMessengerEXT debugMessenger;

      VkSurfaceKHR surface;

      VkPhysicalDevice physicalDevice;

      VkDevice device;
      VkQueue transferQueue;
      VkQueue graphicsQueue;
      VkQueue presentQueue;

      VkSwapchainKHR swapChain;

      std::vector<VkImage> swapChainImages;
      VkFormat swapChainImageFormat;
      VkExtent2D swapChainExtent;
      std::vector<VkImageView> swapChainImageViews;

      VkRenderPass renderPass;

      VkDescriptorSetLayout descriptorSetLayout;
      VkPipelineLayout pipelineLayout;
      VkPipeline graphicsPipeline;

      std::vector<VkFramebuffer> swapChainFrameBuffers;

      VkCommandPool transferCommandPool;
      VkCommandPool commandPool;

      VkFence transferFence;
      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;
      bool frameBufferResized;
      size_t currentFrame;

      std::vector<Vertex> vertices;
      VkBuffer stagingVertexBuffer;
      VkDeviceMemory stagingVertexBufferMemory;
      VkBuffer vertexBuffer;
      VkDeviceMemory vertexBufferMemory;

      std::vector<uint16_t> indices;
      VkBuffer stagingIndexBuffer;
      VkDeviceMemory stagingIndexBufferMemory;
      VkBuffer indexBuffer;
      VkDeviceMemory indexBufferMemory;

      std::vector<VkBuffer> uniformBuffers;
      std::vector<VkDeviceMemory> uniformBuffersMemory;

      VkDescriptorPool descriptorPool;
      std::vector<VkDescriptorSet> descriptorSets;

      VkImage textureImage;
      VkDeviceMemory textureImageMemory;
      VkImageView textureImageView;
      VkSampler textureSampler;

      std::vector<VkCommandBuffer> commandBuffers;

    public:

      Environment(
          bool validationLayersEnabled,

          int frameBufferWidth,
          int frameBufferHeight,

          const std::vector<std::string>& commandLineArguments,
          const std::vector<std::string>& validationLayers,
          const std::vector<std::string>& extensions,
          const std::vector<std::string>& deviceExtensions,

          std::function<VkSurfaceKHR(VkInstance)> createSurfaceFunction,
          std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)> logFunction
      );

      virtual Environment& setFrameBufferSize(const int& width, const int& height);

      virtual Environment& setFrameBufferResized();

      virtual Environment& update(const std::vector<Vertex>& newVertices, const std::vector<uint16_t>& newIndices);

      virtual Environment& updateUniformBuffer(uint32_t currentImage);

      virtual Environment& drawFrame();

      virtual Environment& waitIdle();

      virtual ~Environment();

    private:

      void set();

      virtual void setConfiguration();
      virtual void setEnvironmentVariables();
      virtual void setLogger();
      virtual void setInstance();
      virtual void setDebugMessenger();
      virtual void setSurface();
      virtual void setPhysicalDevice();
      virtual void setDevice();
      virtual void setSwapChain();
      virtual void setImageViews();
      virtual void setRenderPass();
      virtual void setDescriptorSetLayout();
      virtual void setGraphicsPipeline();
      virtual void setFrameBuffers();
      virtual void setCommandPools();
      virtual void setSyncObjects();
      virtual void setVertexBuffers();
      virtual void setIndexBuffers();
      virtual void setUniformBuffers();
      virtual void setTextureImage();
      virtual void setTextureImageView();
      virtual void setTextureSampler();
      virtual void setDescriptorPool();
      virtual void setDescriptorSets();
      virtual void setCommandBuffers();

      virtual void unsetSwapChain();

      void unset();

      virtual void resetSwapChain();

      virtual std::optional<std::string> getEnvironmentVariable(std::string name);
      virtual void setEnvironmentVariable(std::string name, std::string value);
      virtual bool checkValidationLayerSupport();
      virtual void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& object);
      virtual bool isDeviceSuitable(VkPhysicalDevice& object);
      virtual QueueFamilyIndices findQueueFamilies(VkPhysicalDevice& object);
      virtual bool checkDeviceExtensionSupport(VkPhysicalDevice& object);
      virtual SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice& object);
      virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);
      virtual VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);
      virtual VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities);
      virtual std::vector<char> readFile(const std::string& path);
      virtual VkImageView createImageView(VkImage image, VkFormat format);
      virtual VkShaderModule createShaderModule(const std::vector<char>& code);
      virtual std::tuple<VkBuffer, VkDeviceMemory> createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
      virtual std::tuple<VkImage, VkDeviceMemory> createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
      virtual uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
      virtual VkCommandBuffer beginSingleTimeCommands();
      virtual void endSingleTimeCommands(VkCommandBuffer commandBuffer);
      virtual void copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize size, bool useVkMemoryBarrier);
      virtual void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
      virtual void copyVerticesToStagingBufferMemory(const std::vector<Vertex>& newVertices);
      virtual void copyIndicesToStagingBufferMemory(const std::vector<uint16_t>& newIndices);
      virtual void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);

  };

}
