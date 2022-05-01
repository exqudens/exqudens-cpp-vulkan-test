#pragma once

#include <stdexcept>

#include "exqudens/vulkan/Macros.hpp"
#include "exqudens/vulkan/FunctionsProvider.hpp"

namespace exqudens::vulkan {

  class FunctionsProviderBase:
      virtual public FunctionsProvider
  {

    public:

      Functions functions() override {
        try {
          return {
              .enumerateInstanceLayerProperties = vkEnumerateInstanceLayerProperties,
              .enumerateDeviceExtensionProperties = vkEnumerateDeviceExtensionProperties,
              .enumeratePhysicalDevices = vkEnumeratePhysicalDevices,
              .getPhysicalDeviceProperties = vkGetPhysicalDeviceProperties,
              .getPhysicalDeviceQueueFamilyProperties = vkGetPhysicalDeviceQueueFamilyProperties,
              .getPhysicalDeviceSurfaceSupportKHR = vkGetPhysicalDeviceSurfaceSupportKHR,
              .getPhysicalDeviceFeatures = vkGetPhysicalDeviceFeatures,
              .getPhysicalDeviceSurfaceCapabilitiesKHR = vkGetPhysicalDeviceSurfaceCapabilitiesKHR,
              .getPhysicalDeviceSurfaceFormatsKHR = vkGetPhysicalDeviceSurfaceFormatsKHR,
              .getPhysicalDeviceSurfacePresentModesKHR = vkGetPhysicalDeviceSurfacePresentModesKHR,
              .getPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties,
              .getPhysicalDeviceFormatProperties = vkGetPhysicalDeviceFormatProperties,
              .getInstanceProcAddr = vkGetInstanceProcAddr,
              .getDeviceQueue = vkGetDeviceQueue,
              .getBufferMemoryRequirements = vkGetBufferMemoryRequirements,
              .getImageMemoryRequirements = vkGetImageMemoryRequirements,
              .getSwapchainImagesKHR = vkGetSwapchainImagesKHR,
              .updateDescriptorSets = vkUpdateDescriptorSets,
              .createInstance = vkCreateInstance,
              .createDevice = vkCreateDevice,
              .createCommandPool = vkCreateCommandPool,
              .createSwapchainKHR = vkCreateSwapchainKHR,
              .createBuffer = vkCreateBuffer,
              .createImage = vkCreateImage,
              .createImageView = vkCreateImageView,
              .createRenderPass = vkCreateRenderPass,
              .createDescriptorSetLayout = vkCreateDescriptorSetLayout,
              .createShaderModule = vkCreateShaderModule,
              .createPipelineLayout = vkCreatePipelineLayout,
              .createGraphicsPipelines = vkCreateGraphicsPipelines,
              .createFramebuffer = vkCreateFramebuffer,
              .createSampler = vkCreateSampler,
              .createDescriptorPool = vkCreateDescriptorPool,
              .createSemaphore = vkCreateSemaphore,
              .createFence = vkCreateFence,
              .allocateMemory = vkAllocateMemory,
              .allocateDescriptorSets = vkAllocateDescriptorSets,
              .allocateCommandBuffers = vkAllocateCommandBuffers,
              .bindBufferMemory = vkBindBufferMemory,
              .bindImageMemory = vkBindImageMemory,
              .mapMemory = vkMapMemory,
              .unmapMemory = vkUnmapMemory,
              .freeMemory = vkFreeMemory,
              .freeCommandBuffers = vkFreeCommandBuffers,
              .destroyFence = vkDestroyFence,
              .destroySemaphore = vkDestroySemaphore,
              .destroyDescriptorPool = vkDestroyDescriptorPool,
              .destroySampler = vkDestroySampler,
              .destroyFramebuffer = vkDestroyFramebuffer,
              .destroyPipeline = vkDestroyPipeline,
              .destroyShaderModule = vkDestroyShaderModule,
              .destroyDescriptorSetLayout = vkDestroyDescriptorSetLayout,
              .destroyRenderPass = vkDestroyRenderPass,
              .destroyImageView = vkDestroyImageView,
              .destroyImage = vkDestroyImage,
              .destroyBuffer = vkDestroyBuffer,
              .destroySwapchainKHR = vkDestroySwapchainKHR,
              .destroyCommandPool = vkDestroyCommandPool,
              .destroyDevice = vkDestroyDevice,
              .destroySurfaceKHR = vkDestroySurfaceKHR,
              .destroyInstance = vkDestroyInstance,
              .cmdCopyBuffer = vkCmdCopyBuffer,
              .cmdCopyBufferToImage = vkCmdCopyBufferToImage,
              .cmdPipelineBarrier = vkCmdPipelineBarrier
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
