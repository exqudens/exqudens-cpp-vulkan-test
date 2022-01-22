#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

#include "vulkan/vulkan.h"

#include "exqudens/vulkan/model/Configuration.hpp"
#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"
#include "exqudens/vulkan/model/SwapChainSupportDetails.hpp"
#include "exqudens/vulkan/model/Vertex.hpp"
#include "exqudens/vulkan/Logger.hpp"

namespace exqudens::vulkan {

  using Configuration = model::Configuration;
  using QueueFamilyIndices = model::QueueFamilyIndices;
  using SwapChainSupportDetails = model::SwapChainSupportDetails;
  using Vertex = model::Vertex;

  class Initializer {

    public:

      virtual void setEnvironmentVariables(
          std::map<std::string, std::string>& environmentVariables,
          std::map<std::string, std::string>& environmentVariablesOld,
          std::vector<std::string>& commandLineArguments
      );

      virtual void setLogger(
          Logger*& logger,
          bool& loggerDelete,
          std::function<void(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, std::string)>& logFunction
      );

      virtual void setInstance(
          VkInstance& instance,
          bool& validationLayersEnabled,
          std::vector<const char*>& validationLayers,
          std::vector<const char*>& extensions,
          Logger*& logger
      );

      virtual void setDebugMessenger(
          VkDebugUtilsMessengerEXT& debugMessenger,
          VkInstance& instance,
          Logger*& logger
      );

      virtual void setSurface(
          VkSurfaceKHR& surface,
          VkInstance& instance,
          std::function<VkSurfaceKHR(VkInstance)>& createSurfaceFunction
      );

      virtual void setPhysicalDevice(
          VkPhysicalDevice& physicalDevice,
          VkInstance& instance,
          VkSurfaceKHR& surface,
          std::vector<const char*>& deviceExtensions
      );

      virtual void setDevice(
          VkDevice& device,
          VkQueue& deviceGraphicsQueue,
          VkQueue& devicePresentQueue,
          VkSurfaceKHR& surface,
          VkPhysicalDevice& physicalDevice,
          std::vector<const char*>& deviceExtensions,
          bool& validationLayersEnabled,
          std::vector<const char*>& validationLayers
      );

      virtual void setSwapChain(
          VkSwapchainKHR& swapChain,
          std::vector<VkImage>& swapChainImages,
          VkFormat& swapChainImageFormat,
          VkExtent2D& swapChainExtent,
          VkSurfaceKHR& surface,
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          int& frameBufferWidth,
          int& frameBufferHeight
      );

      virtual void setImageViews(
          std::vector<VkImageView>& swapChainImageViews,
          VkDevice& device,
          const std::vector<VkImage>& swapChainImages,
          VkFormat& swapChainImageFormat
      );

      virtual void setRenderPass(
          VkRenderPass& renderPass,
          VkDevice& device,
          VkFormat& swapChainImageFormat
      );

      virtual void setGraphicsPipeline(
          VkPipeline& graphicsPipeline,
          VkPipelineLayout& graphicsPipelineLayout,
          VkDevice& device,
          VkExtent2D& swapChainExtent,
          VkRenderPass& renderPass
      );

      virtual void setFrameBuffers(
          std::vector<VkFramebuffer>& swapChainFrameBuffers,
          VkDevice& device,
          std::vector<VkImageView>& swapChainImageViews,
          VkRenderPass& renderPass,
          VkExtent2D& swapChainExtent
      );

      virtual void setCommandPool(
          VkCommandPool& commandPool,
          VkSurfaceKHR& surface,
          VkPhysicalDevice& physicalDevice,
          VkDevice& device
      );

      virtual void setVertexBuffer(
          VkBuffer& vertexBuffer,
          VkDeviceMemory& vertexBufferMemory,
          std::vector<Vertex>& vertices,
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkCommandPool& commandPool,
          VkQueue& graphicsQueue
      );

      virtual void setIndexBuffer(
          VkBuffer& indexBuffer,
          VkDeviceMemory& indexBufferMemory,
          std::vector<uint16_t>& indices,
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          VkCommandPool& commandPool,
          VkQueue& graphicsQueue
      );

      virtual void setCommandBuffers(
          std::vector<VkCommandBuffer>& commandBuffers,
          VkDevice& device,
          std::vector<VkFramebuffer>& swapChainFrameBuffers,
          VkCommandPool& commandPool,
          VkRenderPass& renderPass,
          VkExtent2D& swapChainExtent,
          VkPipeline& graphicsPipeline,
          VkBuffer& vertexBuffer,
          VkBuffer& indexBuffer,
          std::vector<uint16_t>& indices
      );

      virtual void setSyncObjects(
          std::vector<VkSemaphore>& imageAvailableSemaphores,
          std::vector<VkSemaphore>& renderFinishedSemaphores,
          std::vector<VkFence>& inFlightFences,
          std::vector<VkFence>& imagesInFlight,
          VkDevice& device,
          int& maxFramesInFlight,
          std::vector<VkImage>& swapChainImages
      );

      virtual void resetSwapChain(
          int& frameBufferWidth,
          int& frameBufferHeight,

          VkSurfaceKHR& surface,

          VkPhysicalDevice& physicalDevice,

          VkDevice& device,

          VkSwapchainKHR& swapChain,
          std::vector<VkImage>& swapChainImages,
          VkFormat& swapChainImageFormat,
          VkExtent2D& swapChainExtent,
          std::vector<VkImageView>& swapChainImageViews,

          VkRenderPass& renderPass,
          VkPipelineLayout& graphicsPipelineLayout,
          VkPipeline& graphicsPipeline,

          std::vector<VkFramebuffer>& swapChainFrameBuffers,

          VkCommandPool& commandPool,

          VkBuffer& vertexBuffer,
          VkBuffer& indexBuffer,
          std::vector<uint16_t>& indices,

          std::vector<VkCommandBuffer>& commandBuffers,

          std::vector<VkFence>& imagesInFlight
      );

      virtual void unset(
          bool& validationLayersEnabled,

          std::map<std::string, std::string>& environmentVariables,
          std::map<std::string, std::string>& environmentVariablesOld,

          VkInstance& instance,
          VkDebugUtilsMessengerEXT& debugMessenger,
          VkSurfaceKHR& surface,

          VkDevice& device,

          VkSwapchainKHR& swapChain,
          std::vector<VkImageView>& swapChainImageViews,

          VkRenderPass& renderPass,
          VkPipelineLayout& graphicsPipelineLayout,
          VkPipeline& graphicsPipeline,

          std::vector<VkFramebuffer>& swapChainFrameBuffers,

          VkCommandPool& commandPool,

          VkBuffer& vertexBuffer,
          VkDeviceMemory& vertexBufferMemory,
          VkBuffer& indexBuffer,
          VkDeviceMemory& indexBufferMemory,

          std::vector<VkCommandBuffer>& commandBuffers,

          int& maxFramesInFlight,
          std::vector<VkSemaphore>& imageAvailableSemaphores,
          std::vector<VkSemaphore>& renderFinishedSemaphores,
          std::vector<VkFence>& inFlightFences
      );

      virtual ~Initializer();

    private:

      virtual void setEnvironmentVariable(std::string& name, std::string& value);

      virtual std::optional<std::string> getEnvironmentVariable(std::string& name);

      virtual bool checkValidationLayerSupport(std::vector<const char*>& validationLayers);

      virtual void setVkDebugUtilsMessengerCreateInfoEXT(VkDebugUtilsMessengerCreateInfoEXT& object, Logger* logger);

      virtual bool isDeviceSuitable(VkSurfaceKHR& surface, VkPhysicalDevice& device, std::vector<const char*>& deviceExtensions);

      virtual QueueFamilyIndices findQueueFamilies(VkSurfaceKHR& surface, VkPhysicalDevice& device);

      virtual bool checkDeviceExtensionSupport(VkPhysicalDevice& device, std::vector<const char*>& deviceExtensions);

      virtual SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR& surface, VkPhysicalDevice& device);

      virtual VkSurfaceFormatKHR chooseSwapSurfaceFormat(std::vector<VkSurfaceFormatKHR>& availableFormats);

      virtual VkPresentModeKHR chooseSwapPresentMode(std::vector<VkPresentModeKHR>& availablePresentModes);

      virtual VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR& capabilities, int& frameBufferWidth, int& frameBufferHeight);

      virtual std::vector<char> readFile(const std::string& path);

      virtual VkShaderModule createShaderModule(VkDevice& device, const std::vector<char>& code);

      virtual uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties, VkPhysicalDevice& physicalDevice);

      virtual void setBuffer(
          VkBuffer& buffer,
          VkDeviceMemory& bufferMemory,
          VkDeviceSize size,
          VkBufferUsageFlags usage,
          VkMemoryPropertyFlags properties,
          VkPhysicalDevice& physicalDevice,
          VkDevice& device
      );

      virtual void copyBuffer(
          VkBuffer& srcBuffer,
          VkBuffer& dstBuffer,
          VkDeviceSize size,
          VkDevice& device,
          VkCommandPool& commandPool,
          VkQueue& graphicsQueue
      );

      virtual void unsetSwapChain(
          VkDevice& device,

          VkSwapchainKHR& swapChain,
          std::vector<VkImageView>& swapChainImageViews,

          VkRenderPass& renderPass,
          VkPipelineLayout& graphicsPipelineLayout,
          VkPipeline& graphicsPipeline,

          std::vector<VkFramebuffer>& swapChainFrameBuffers,

          VkCommandPool& commandPool,
          std::vector<VkCommandBuffer>& commandBuffers
      );

      virtual void unsetDebugMessenger(
          VkDebugUtilsMessengerEXT& debugMessenger,
          bool& validationLayersEnabled,
          VkInstance& instance
      );

  };

}
