#include <stdexcept>
#include <utility>
#include <iostream>

#include "exqudens/vulkan/Environment.hpp"

#define TO_EXCEPTION_MESSAGE(message) std::string(__FUNCTION__) + "(" + __FILE__ + ":" + std::to_string(__LINE__) + "): " + message
#define THROW_RUNTIME_ERROR(message) throw std::runtime_error(TO_EXCEPTION_MESSAGE(message))

namespace exqudens::vulkan {

  Environment::Environment(
      Configuration configuration,
      Initializer* initializer
  ):
      configuration(std::move(configuration)),
      initializer(initializer != nullptr ? initializer : new Initializer()),
      initializerDelete(initializer == nullptr),

      logger(nullptr),
      loggerDelete(false),

      instance(nullptr),

      debugMessenger(nullptr),

      surface(nullptr),

      physicalDevice(nullptr),

      device(nullptr),
      graphicsQueue(nullptr),
      presentQueue(nullptr),

      swapChain(nullptr),
      swapChainImages(std::vector<VkImage>()),
      swapChainImageFormat(VK_FORMAT_UNDEFINED),
      swapChainExtent(VkExtent2D()),
      swapChainImageViews(std::vector<VkImageView>()),

      renderPass(nullptr),
      graphicsPipelineLayout(nullptr),
      graphicsPipeline(nullptr),

      swapChainFrameBuffers(std::vector<VkFramebuffer>()),

      commandPool(nullptr),

      vertices({
          {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
          {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
          {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
          {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
      }),
      indices({
          0, 1, 2, 2, 3, 0
      }),
      vertexBuffer(nullptr),
      vertexBufferMemory(nullptr),
      indexBuffer(nullptr),
      indexBufferMemory(nullptr),

      commandBuffers(std::vector<VkCommandBuffer>()),

      maxFramesInFlight(2),
      imageAvailableSemaphores(std::vector<VkSemaphore>()),
      renderFinishedSemaphores(std::vector<VkSemaphore>()),
      currentFrame(0),
      inFlightFences(std::vector<VkFence>()),
      imagesInFlight(std::vector<VkFence>()),

      frameBufferResized(false)
  {
    setEnvironmentVariables();

    setLogger();

    setInstance();

    setDebugMessenger();

    setSurface();

    setPhysicalDevice();

    setDevice();

    setSwapChain();
    setSwapChainImageViews();

    setRenderPass();
    setGraphicsPipeline();

    setFrameBuffers();

    setCommandPool();

    setVertexBuffer();
    setIndexBuffer();
    setCommandBuffers();

    setSyncObjects();
  }

  Configuration& Environment::getConfiguration() {
    return configuration;
  }

  void Environment::drawFrame() {
    vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      initializer->resetSwapChain(
          configuration.getFrameBufferWidth(),
          configuration.getFrameBufferWidth(),
          surface,
          physicalDevice,
          device,
          swapChain,
          swapChainImages,
          swapChainImageFormat,
          swapChainExtent,
          swapChainImageViews,
          renderPass,
          graphicsPipelineLayout,
          graphicsPipeline,
          swapChainFrameBuffers,
          commandPool,
          vertexBuffer,
          indexBuffer,
          indices,
          commandBuffers,
          imagesInFlight
      );
      return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      THROW_RUNTIME_ERROR("failed to acquire swap chain image!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
      vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(device, 1, &inFlightFences[currentFrame]);

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
      THROW_RUNTIME_ERROR("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    result = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || frameBufferResized) {
      frameBufferResized = false;
      initializer->resetSwapChain(
          configuration.getFrameBufferWidth(),
          configuration.getFrameBufferWidth(),
          surface,
          physicalDevice,
          device,
          swapChain,
          swapChainImages,
          swapChainImageFormat,
          swapChainExtent,
          swapChainImageViews,
          renderPass,
          graphicsPipelineLayout,
          graphicsPipeline,
          swapChainFrameBuffers,
          commandPool,
          vertexBuffer,
          indexBuffer,
          indices,
          commandBuffers,
          imagesInFlight
      );
    } else if (result != VK_SUCCESS) {
      THROW_RUNTIME_ERROR("failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % maxFramesInFlight;
  }

  void Environment::waitIdle() {
    vkDeviceWaitIdle(device);
  }

  void Environment::setFrameBufferResized() {
    frameBufferResized = true;
  }

  Environment::~Environment() {
    try {
      initializer->unset(
          configuration.isValidationLayersEnabled(),
          environmentVariables,
          environmentVariablesOld,
          instance,
          debugMessenger,
          surface,
          device,
          swapChain,
          swapChainImageViews,
          renderPass,
          graphicsPipelineLayout,
          graphicsPipeline,
          swapChainFrameBuffers,
          commandPool,
          vertexBuffer,
          vertexBufferMemory,
          indexBuffer,
          indexBufferMemory,
          commandBuffers,
          maxFramesInFlight,
          imageAvailableSemaphores,
          renderFinishedSemaphores,
          inFlightFences
      );

      if (loggerDelete) {
        delete logger;
      }

      if (initializerDelete) {
        delete initializer;
      }
    } catch (const std::exception& e) {
      std::cerr << TO_EXCEPTION_MESSAGE(e.what()) << std::endl;
    } catch (...) {
      std::cerr << TO_EXCEPTION_MESSAGE("Unhandled exception!") << std::endl;
    }
  }

  void Environment::setEnvironmentVariables() {
    initializer->setEnvironmentVariables(
        environmentVariables,
        environmentVariablesOld,
        configuration.getCommandLineArguments()
    );
  }

  void Environment::setLogger() {
    initializer->setLogger(logger, loggerDelete, configuration.getLogFunction());
  }

  void Environment::setInstance() {
    initializer->setInstance(
        instance,
        configuration.isValidationLayersEnabled(),
        configuration.getValidationLayersPointers(),
        configuration.getExtensionsPointers(),
        logger
    );
  }

  void Environment::setDebugMessenger() {
    if (configuration.isValidationLayersEnabled()) {
      initializer->setDebugMessenger(debugMessenger, instance, logger);
    }
  }

  void Environment::setSurface() {
    initializer->setSurface(surface, instance, configuration.getCreateSurfaceFunction());
  }

  void Environment::setPhysicalDevice() {
    initializer->setPhysicalDevice(physicalDevice, instance, surface, configuration.getDeviceExtensionsPointers());
  }

  void Environment::setDevice() {
    initializer->setDevice(
        device,
        graphicsQueue,
        presentQueue,
        surface,
        physicalDevice,
        configuration.getDeviceExtensionsPointers(),
        configuration.isValidationLayersEnabled(),
        configuration.getValidationLayersPointers()
    );
  }

  void Environment::setSwapChain() {
    initializer->setSwapChain(
        swapChain,
        swapChainImages,
        swapChainImageFormat,
        swapChainExtent,
        surface,
        physicalDevice,
        device,
        configuration.getFrameBufferWidth(),
        configuration.getFrameBufferHeight()
    );
  }

  void Environment::setSwapChainImageViews() {
    initializer->setImageViews(
        swapChainImageViews,
        device,
        swapChainImages,
        swapChainImageFormat
    );
  }

  void Environment::setRenderPass() {
    initializer->setRenderPass(
        renderPass,
        device,
        swapChainImageFormat
    );
  }

  void Environment::setGraphicsPipeline() {
    initializer->setGraphicsPipeline(
        graphicsPipeline,
        graphicsPipelineLayout,
        device,
        swapChainExtent,
        renderPass
    );
  }

  void Environment::setFrameBuffers() {
    initializer->setFrameBuffers(
        swapChainFrameBuffers,
        device,
        swapChainImageViews,
        renderPass,
        swapChainExtent
    );
  }

  void Environment::setCommandPool() {
    initializer->setCommandPool(
        commandPool,
        surface,
        physicalDevice,
        device
    );
  }

  void Environment::setVertexBuffer() {
    initializer->setVertexBuffer(
        vertexBuffer,
        vertexBufferMemory,
        vertices,
        physicalDevice,
        device,
        commandPool,
        graphicsQueue
    );
  }

  void Environment::setIndexBuffer() {
    initializer->setIndexBuffer(
        indexBuffer,
        indexBufferMemory,
        indices,
        physicalDevice,
        device,
        commandPool,
        graphicsQueue
    );
  }

  void Environment::setCommandBuffers() {
    initializer->setCommandBuffers(
        commandBuffers,
        device,
        swapChainFrameBuffers,
        commandPool,
        renderPass,
        swapChainExtent,
        graphicsPipeline,
        vertexBuffer,
        indexBuffer,
        indices
    );
  }

  void Environment::setSyncObjects() {
    initializer->setSyncObjects(
        imageAvailableSemaphores,
        renderFinishedSemaphores,
        inFlightFences,
        imagesInFlight,
        device,
        maxFramesInFlight,
        swapChainImages
    );
  }
}
