#pragma once

#include <cstdint>
#include <functional>

#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Functions {

    std::function<void(
        uint32_t*                                   pPropertyCount,
        VkLayerProperties*                          pProperties
    )> enumerateInstanceLayerProperties;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        const char*                                 pLayerName,
        uint32_t*                                   pPropertyCount,
        VkExtensionProperties*                      pProperties
    )> enumerateDeviceExtensionProperties;

    std::function<void(
        VkInstance                                  instance,
        uint32_t*                                   pPhysicalDeviceCount,
        VkPhysicalDevice*                           pPhysicalDevices
    )> enumeratePhysicalDevices;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkPhysicalDeviceProperties*                 pProperties
    )> getPhysicalDeviceProperties;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        uint32_t*                                   pQueueFamilyPropertyCount,
        VkQueueFamilyProperties*                    pQueueFamilyProperties
    )> getPhysicalDeviceQueueFamilyProperties;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        uint32_t                                    queueFamilyIndex,
        VkSurfaceKHR                                surface,
        VkBool32*                                   pSupported
    )> getPhysicalDeviceSurfaceSupportKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkPhysicalDeviceFeatures*                   pFeatures
    )> getPhysicalDeviceFeatures;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkSurfaceKHR                                surface,
        VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities
    )> getPhysicalDeviceSurfaceCapabilitiesKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkSurfaceKHR                                surface,
        uint32_t*                                   pSurfaceFormatCount,
        VkSurfaceFormatKHR*                         pSurfaceFormats
    )> getPhysicalDeviceSurfaceFormatsKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkSurfaceKHR                                surface,
        uint32_t*                                   pPresentModeCount,
        VkPresentModeKHR*                           pPresentModes
    )> getPhysicalDeviceSurfacePresentModesKHR;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkPhysicalDeviceMemoryProperties*           pMemoryProperties
    )> getPhysicalDeviceMemoryProperties;

    std::function<void(
        VkPhysicalDevice                            physicalDevice,
        VkFormat                                    format,
        VkFormatProperties*                         pFormatProperties
    )> getPhysicalDeviceFormatProperties;

    std::function<PFN_vkVoidFunction(
        VkInstance                                  instance,
        const char*                                 pName
    )> getInstanceProcAddr;

    std::function<void(
        VkDevice                                    device,
        uint32_t                                    queueFamilyIndex,
        uint32_t                                    queueIndex,
        VkQueue*                                    pQueue
    )> getDeviceQueue;

    std::function<void(
        VkDevice                                    device,
        VkBuffer                                    buffer,
        VkMemoryRequirements*                       pMemoryRequirements
    )> getBufferMemoryRequirements;

    std::function<void(
        VkDevice                                    device,
        VkImage                                     image,
        VkMemoryRequirements*                       pMemoryRequirements
    )> getImageMemoryRequirements;

    std::function<VkResult(
        VkDevice                                    device,
        VkSwapchainKHR                              swapchain,
        uint32_t*                                   pSwapchainImageCount,
        VkImage*                                    pSwapchainImages
    )> getSwapchainImagesKHR;

    std::function<void(
        VkDevice                                    device,
        uint32_t                                    descriptorWriteCount,
        const VkWriteDescriptorSet*                 pDescriptorWrites,
        uint32_t                                    descriptorCopyCount,
        const VkCopyDescriptorSet*                  pDescriptorCopies
    )> updateDescriptorSets;

    std::function<VkResult(
        const VkInstanceCreateInfo*                 pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkInstance*                                 pInstance
    )> createInstance;

    std::function<VkResult(
        VkPhysicalDevice                            physicalDevice,
        const VkDeviceCreateInfo*                   pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDevice*                                   pDevice
    )> createDevice;

    std::function<VkResult(
        VkDevice                                    device,
        const VkCommandPoolCreateInfo*              pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkCommandPool*                              pCommandPool
    )> createCommandPool;

    std::function<VkSurfaceKHR(
        VkInstance                                  instance
    )> createSurfaceKHR = {};

    std::function<VkResult(
        VkDevice                                    device,
        const VkSwapchainCreateInfoKHR*             pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkSwapchainKHR*                             pSwapchain
    )> createSwapchainKHR;

    std::function<VkResult(
        VkDevice                                    device,
        const VkBufferCreateInfo*                   pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkBuffer*                                   pBuffer
    )> createBuffer;

    std::function<VkResult(
        VkDevice                                    device,
        const VkImageCreateInfo*                    pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkImage*                                    pImage
    )> createImage;

    std::function<VkResult(
        VkDevice                                    device,
        const VkImageViewCreateInfo*                pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkImageView*                                pView
    )> createImageView;

    std::function<VkResult(
        VkDevice                                    device,
        const VkRenderPassCreateInfo*               pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkRenderPass*                               pRenderPass
    )> createRenderPass;

    std::function<VkResult(
        VkDevice                                    device,
        const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDescriptorSetLayout*                      pSetLayout
    )> createDescriptorSetLayout;

    std::function<VkResult(
        VkDevice                                    device,
        const VkShaderModuleCreateInfo*             pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkShaderModule*                             pShaderModule
    )> createShaderModule;

    std::function<VkResult(
        VkDevice                                    device,
        const VkPipelineLayoutCreateInfo*           pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkPipelineLayout*                           pPipelineLayout
    )> createPipelineLayout;

    std::function<VkResult(
        VkDevice                                    device,
        VkPipelineCache                             pipelineCache,
        uint32_t                                    createInfoCount,
        const VkGraphicsPipelineCreateInfo*         pCreateInfos,
        const VkAllocationCallbacks*                pAllocator,
        VkPipeline*                                 pPipelines
    )> createGraphicsPipelines;

    std::function<VkResult(
        VkDevice                                    device,
        const VkFramebufferCreateInfo*              pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkFramebuffer*                              pFramebuffer
    )> createFramebuffer;

    std::function<VkResult(
        VkDevice                                    device,
        const VkSamplerCreateInfo*                  pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkSampler*                                  pSampler
    )> createSampler;

    std::function<VkResult(
        VkDevice                                    device,
        const VkDescriptorPoolCreateInfo*           pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDescriptorPool*                           pDescriptorPool
    )> createDescriptorPool;

    std::function<VkResult(
        VkDevice                                    device,
        const VkSemaphoreCreateInfo*                pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkSemaphore*                                pSemaphore
    )> createSemaphore;

    std::function<VkResult(
        VkDevice                                    device,
        const VkFenceCreateInfo*                    pCreateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkFence*                                    pFence
    )> createFence;

    std::function<VkResult(
        VkDevice                                    device,
        const VkMemoryAllocateInfo*                 pAllocateInfo,
        const VkAllocationCallbacks*                pAllocator,
        VkDeviceMemory*                             pMemory
    )> allocateMemory;

    std::function<VkResult(
        VkDevice                                    device,
        const VkDescriptorSetAllocateInfo*          pAllocateInfo,
        VkDescriptorSet*                            pDescriptorSets
    )> allocateDescriptorSets;

    std::function<VkResult(
        VkDevice                                    device,
        const VkCommandBufferAllocateInfo*          pAllocateInfo,
        VkCommandBuffer*                            pCommandBuffers
    )> allocateCommandBuffers;

    std::function<VkResult(
        VkDevice                                    device,
        VkBuffer                                    buffer,
        VkDeviceMemory                              memory,
        VkDeviceSize                                memoryOffset
    )> bindBufferMemory;

    std::function<VkResult(
        VkDevice                                    device,
        VkImage                                     image,
        VkDeviceMemory                              memory,
        VkDeviceSize                                memoryOffset
    )> bindImageMemory;

    std::function<VkResult(
        VkDevice                                    device,
        VkDeviceMemory                              memory,
        VkDeviceSize                                offset,
        VkDeviceSize                                size,
        VkMemoryMapFlags                            flags,
        void**                                      ppData
    )> mapMemory;

    std::function<void(
        VkDevice                                    device,
        VkDeviceMemory                              memory
    )> unmapMemory;

    std::function<void(
        VkDevice                                    device,
        VkDeviceMemory                              memory,
        const VkAllocationCallbacks*                pAllocator
    )> freeMemory;

    std::function<void(
        VkDevice                                    device,
        VkCommandPool                               commandPool,
        uint32_t                                    commandBufferCount,
        const VkCommandBuffer*                      pCommandBuffers
    )> freeCommandBuffers;

    std::function<void(
        VkDevice                                    device,
        VkFence                                     fence,
        const VkAllocationCallbacks*                pAllocator
    )> destroyFence;

    std::function<void(
        VkDevice                                    device,
        VkSemaphore                                 semaphore,
        const VkAllocationCallbacks*                pAllocator
    )> destroySemaphore;

    std::function<void(
        VkDevice                                    device,
        VkDescriptorPool                            descriptorPool,
        const VkAllocationCallbacks*                pAllocator
    )> destroyDescriptorPool;

    std::function<void(
        VkDevice                                    device,
        VkSampler                                   sampler,
        const VkAllocationCallbacks*                pAllocator
    )> destroySampler;

    std::function<void(
        VkDevice                                    device,
        VkFramebuffer                               framebuffer,
        const VkAllocationCallbacks*                pAllocator
    )> destroyFramebuffer;

    std::function<void(
        VkDevice                                    device,
        VkPipeline                                  pipeline,
        const VkAllocationCallbacks*                pAllocator
    )> destroyPipeline;

    std::function<void(
        VkDevice                                    device,
        VkShaderModule                              shaderModule,
        const VkAllocationCallbacks*                pAllocator
    )> destroyShaderModule;

    std::function<void(
        VkDevice                                    device,
        VkDescriptorSetLayout                       descriptorSetLayout,
        const VkAllocationCallbacks*                pAllocator
    )> destroyDescriptorSetLayout;

    std::function<void(
        VkDevice                                    device,
        VkRenderPass                                renderPass,
        const VkAllocationCallbacks*                pAllocator
    )> destroyRenderPass;

    std::function<void(
        VkDevice                                    device,
        VkImageView                                 imageView,
        const VkAllocationCallbacks*                pAllocator
    )> destroyImageView;

    std::function<void(
        VkDevice                                    device,
        VkImage                                     image,
        const VkAllocationCallbacks*                pAllocator
    )> destroyImage;

    std::function<void(
        VkDevice                                    device,
        VkBuffer                                    buffer,
        const VkAllocationCallbacks*                pAllocator
    )> destroyBuffer;

    std::function<void(
        VkDevice                                    device,
        VkSwapchainKHR                              swapchain,
        const VkAllocationCallbacks*                pAllocator
    )> destroySwapchainKHR;

    std::function<void(
        VkDevice                                    device,
        VkCommandPool                               commandPool,
        const VkAllocationCallbacks*                pAllocator
    )> destroyCommandPool;

    std::function<void(
        VkDevice                                    device,
        const VkAllocationCallbacks*                pAllocator
    )> destroyDevice;

    std::function<void(
        VkInstance                                  instance,
        VkSurfaceKHR                                surface,
        const VkAllocationCallbacks*                pAllocator
    )> destroySurfaceKHR;

    std::function<void(
        VkInstance                                  instance,
        const VkAllocationCallbacks*                pAllocator
    )> destroyInstance;

    std::function<void(
        VkCommandBuffer                             commandBuffer,
        VkBuffer                                    srcBuffer,
        VkBuffer                                    dstBuffer,
        uint32_t                                    regionCount,
        const VkBufferCopy*                         pRegions
    )> cmdCopyBuffer;

    std::function<void(
        VkCommandBuffer                             commandBuffer,
        VkBuffer                                    srcBuffer,
        VkImage                                     dstImage,
        VkImageLayout                               dstImageLayout,
        uint32_t                                    regionCount,
        const VkBufferImageCopy*                    pRegions
    )> cmdCopyBufferToImage;

    std::function<void(
        VkCommandBuffer                             commandBuffer,
        VkPipelineStageFlags                        srcStageMask,
        VkPipelineStageFlags                        dstStageMask,
        VkDependencyFlags                           dependencyFlags,
        uint32_t                                    memoryBarrierCount,
        const VkMemoryBarrier*                      pMemoryBarriers,
        uint32_t                                    bufferMemoryBarrierCount,
        const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
        uint32_t                                    imageMemoryBarrierCount,
        const VkImageMemoryBarrier*                 pImageMemoryBarriers
    )> cmdPipelineBarrier;

  };

}
