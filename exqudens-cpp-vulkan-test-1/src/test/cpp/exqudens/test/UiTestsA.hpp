#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gtest/gtest.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/ContextBase.hpp"
#include "exqudens/vulkan/RendererBase.hpp"
#include "exqudens/test/model/Vertex.hpp"
#include "exqudens/test/model/UniformBufferObject.hpp"

namespace exqudens::vulkan {

  class UiTestsA : public testing::Test {

    protected:

      class Environment:
          virtual public RendererBase
      {

        public:

          bool resized = false;

        private:

          const int MAX_FRAMES_IN_FLIGHT = 2;

          GLFWwindow* window = nullptr;

          std::vector<Vertex> vertices = {};
          std::vector<uint16_t> indices = {};

          Context* context = nullptr;

          std::map<std::string, std::string> environmentVariables = {};
          Configuration configuration = {};
          Logger logger = {};

          Instance instance = {};
          DebugUtilsMessenger debugUtilsMessenger = {};
          Surface surface = {};
          PhysicalDevice physicalDevice = {};
          Device device = {};
          SwapChain swapChain = {};

          Buffer vertexStagingBuffer = {};
          Buffer vertexBuffer = {};

          Buffer indexStagingBuffer = {};
          Buffer indexBuffer = {};

          std::vector<Buffer> uniformBuffers = {};

          Buffer samplerImageStaging = {};
          Image samplerImage = {};
          ImageView samplerImageView = {};
          Sampler sampler = {};

          Image depthImage = {};
          ImageView depthImageView = {};

          DescriptorSetLayout descriptorSetLayout = {};
          DescriptorPool descriptorPool = {};
          std::vector<DescriptorSet> descriptorSets = {};

          RenderPass renderPass = {};

          Pipeline graphicsPipeline = {};

          std::vector<Image> swapChainImages = {};
          std::vector<ImageView> swapChainImageViews = {};
          std::vector<FrameBuffer> swapChainFrameBuffers = {};

          Queue transferQueue = {};
          CommandPool transferCommandPool = {};
          CommandBuffer transferCommandBuffer = {};

          Queue graphicsQueue = {};
          CommandPool graphicsCommandPool = {};
          std::vector<CommandBuffer> graphicsCommandBuffers = {};

          Queue presentQueue = {};

          std::vector<Semaphore> imageAvailableSemaphores = {};
          std::vector<Semaphore> renderFinishedSemaphores = {};
          std::vector<Fence> inFlightFences = {};

          std::size_t currentFrame = 0;

        public:

          void create(const std::vector<std::string>& arguments, GLFWwindow* window) {
            try {
              this->window = window;

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

              context = new ContextBase;

              environmentVariables = context->createEnvironmentVariables(arguments.front());
              for (auto const& [name, value] : environmentVariables) {
                context->setEnvironmentVariable(name, value);
              }

              configuration = context->createConfiguration();
              uint32_t glfwExtensionCount = 0;
              const char** glfwExtensions;
              glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
              std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
              for (const char*& extension : extensions) {
                configuration.extensions.emplace_back(extension);
              }

              logger = context->createLogger(
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
              );

              instance = context->createInstance(configuration, logger);
              debugUtilsMessenger = context->createDebugUtilsMessenger(instance.value, logger);
              surface = context->add(createSurface(instance.value));
              physicalDevice = context->createPhysicalDevice(instance.value, configuration, surface.value);
              device = context->createDevice(physicalDevice.value, configuration, physicalDevice.queueFamilyIndexInfo);
              int width = 0;
              int height = 0;
              glfwGetFramebufferSize(window, &width, &height);
              swapChain = context->createSwapChain(
                  physicalDevice.swapChainSupportDetails.value(),
                  physicalDevice.queueFamilyIndexInfo,
                  surface.value,
                  device.value,
                  (uint32_t) width,
                  (uint32_t) height
              );

              vertexStagingBuffer = context->createBuffer(
                  physicalDevice.value,
                  device.value,
                  sizeof(vertices[0]) * vertices.size(),
                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
              );
              copyToMemory(device.value, vertexStagingBuffer.memory, 0, vertexStagingBuffer.memorySize, vertices.data());
              vertexBuffer = context->createBuffer(
                  physicalDevice.value,
                  device.value,
                  sizeof(vertices[0]) * vertices.size(),
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
              );

              indexStagingBuffer = context->createBuffer(
                  physicalDevice.value,
                  device.value,
                  sizeof(indices[0]) * indices.size(),
                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
              );
              copyToMemory(device.value, indexStagingBuffer.memory, 0, indexStagingBuffer.memorySize, indices.data());
              indexBuffer = context->createBuffer(
                  physicalDevice.value,
                  device.value,
                  sizeof(vertices[0]) * vertices.size(),
                  VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
              );

              uniformBuffers = context->createBuffers(
                  physicalDevice.value,
                  device.value,
                  sizeof(UniformBufferObject),
                  VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                  MAX_FRAMES_IN_FLIGHT
              );

              unsigned int imageWidth, imageHeight, imageDepth;
              std::vector<unsigned char> imageData;
              TestUtils::readPng(
                  std::filesystem::path().append("resources").append("png").append("texture.png").make_preferred().string(),
                  imageWidth,
                  imageHeight,
                  imageDepth,
                  imageData
              );
              samplerImageStaging = context->createBuffer(
                  physicalDevice.value,
                  device.value,
                  imageWidth * imageHeight * imageDepth,
                  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
              );
              copyToMemory(device.value, samplerImageStaging.memory, 0, samplerImageStaging.memorySize, imageData.data());
              samplerImage = context->createImage(
                  physicalDevice.value,
                  device.value,
                  imageWidth,
                  imageHeight,
                  VK_FORMAT_R8G8B8A8_SRGB,
                  VK_IMAGE_TILING_OPTIMAL,
                  VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
              );
              samplerImageView = context->createImageView(device.value, samplerImage.value, VK_FORMAT_R8G8B8A8_SRGB);
              sampler = context->createSampler(physicalDevice.value, device.value, true);

              depthImage = context->createImage(
                  physicalDevice.value,
                  device.value,
                  swapChain.width,
                  swapChain.height,
                  context->findDepthFormat(physicalDevice.value),
                  VK_IMAGE_TILING_OPTIMAL,
                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
              );
              depthImageView = context->createImageView(
                  device.value,
                  depthImage.value,
                  depthImage.format,
                  VK_IMAGE_ASPECT_DEPTH_BIT
              );

              descriptorSetLayout = context->createDescriptorSetLayout(
                  device.value,
                  DescriptorSetLayoutCreateInfo {
                      .flags = 0,
                      .bindings = {
                          VkDescriptorSetLayoutBinding {
                              .binding = 0,
                              .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                              .descriptorCount = 1,
                              .stageFlags = VK_SHADER_STAGE_VERTEX_BIT,
                              .pImmutableSamplers = nullptr
                          },
                          VkDescriptorSetLayoutBinding {
                              .binding = 1,
                              .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                              .descriptorCount = 1,
                              .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
                              .pImmutableSamplers = nullptr
                          }
                      }
                  }
              );
              descriptorPool = context->createDescriptorPool(
                  device.value,
                  DescriptorPoolCreateInfo {
                      .maxSets = (uint32_t) MAX_FRAMES_IN_FLIGHT,
                      .poolSizes = {
                          VkDescriptorPoolSize {
                              .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                              .descriptorCount = (uint32_t) MAX_FRAMES_IN_FLIGHT
                          },
                          VkDescriptorPoolSize {
                              .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                              .descriptorCount = (uint32_t) MAX_FRAMES_IN_FLIGHT
                          }
                      }
                  }
              );
              descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
              for (std::size_t i = 0; i < descriptorSets.size(); i++) {
                descriptorSets[i] = context->createDescriptorSet(
                    device.value,
                    descriptorPool.value,
                    descriptorSetLayout.value,
                    {
                        WriteDescriptorSet {
                            .dstBinding = 0,
                            .dstArrayElement = 0,
                            .descriptorCount = 1,
                            .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                            .imageInfo = {},
                            .bufferInfo = {
                                VkDescriptorBufferInfo {
                                    .buffer = uniformBuffers[i].value,
                                    .offset = 0,
                                    .range = sizeof(UniformBufferObject)
                                }
                            },
                            .texelBufferView = {}
                        },
                        WriteDescriptorSet {
                            .dstBinding = 1,
                            .dstArrayElement = 0,
                            .descriptorCount = 1,
                            .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                            .imageInfo = {
                                VkDescriptorImageInfo {
                                    .sampler = sampler.value,
                                    .imageView = samplerImageView.value,
                                    .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
                                }
                            },
                            .bufferInfo = {},
                            .texelBufferView = {}
                        }
                    }
                );
              }

              RenderPassCreateInfo renderPassCreateInfo = {
                  .attachments = {
                      VkAttachmentDescription {
                          .format = swapChain.format,
                          .samples = VK_SAMPLE_COUNT_1_BIT,
                          .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                          .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                          .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                          .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                          .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                          .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                      },
                      VkAttachmentDescription {
                          .format = depthImage.format,
                          .samples = VK_SAMPLE_COUNT_1_BIT,
                          .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                          .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                          .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                          .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                          .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                          .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                      }
                  },
                  .subPasses = {
                      SubPassDescription {
                          .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                          .colorAttachments = {
                              VkAttachmentReference {
                                  .attachment = 0,
                                  .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                              }
                          },
                          .depthStencilAttachment = VkAttachmentReference {
                              .attachment = 1,
                              .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                          }
                      }
                  },
                  .dependencies = {
                      VkSubpassDependency {
                          .srcSubpass = VK_SUBPASS_EXTERNAL,
                          .dstSubpass = 0,
                          .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                          .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                          .srcAccessMask = 0,
                          .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
                      }
                  }
              };
              renderPass = context->createRenderPass(
                  device.value,
                  renderPassCreateInfo
              );

              graphicsPipeline = context->createGraphicsPipeline(
                  device.value,
                  swapChain.extent,
                  {"resources/shader/shader-4.vert.spv", "resources/shader/shader-4.frag.spv"},
                  renderPass.value,
                  VK_FRONT_FACE_COUNTER_CLOCKWISE,
                  {descriptorSetLayout.value},
                  {Vertex::getBindingDescription()},
                  Vertex::getAttributeDescriptions()
              );

              swapChainImages = context->createSwapChainImages(device.value, swapChain.value);
              swapChainImageViews = context->createImageViews(device.value, swapChainImages, swapChain.format);
              std::vector<FrameBufferCreateInfo> frameBufferCreateInfoVector;
              frameBufferCreateInfoVector.resize(swapChainImageViews.size());
              for (std::size_t i = 0; i < frameBufferCreateInfoVector.size(); i++) {
                frameBufferCreateInfoVector[i] = FrameBufferCreateInfo {
                    .flags = 0,
                    .renderPass = renderPass.value,
                    .attachments = {
                        swapChainImageViews[i].value,
                        depthImageView.value
                    },
                    .width = swapChain.extent.width,
                    .height = swapChain.extent.height,
                    .layers = 1
                };
              }
              swapChainFrameBuffers = context->createFrameBuffers(device.value, frameBufferCreateInfoVector);

              transferQueue = context->createQueue(
                  device.value,
                  physicalDevice.queueFamilyIndexInfo.transferFamily.value(),
                  0
              );
              transferCommandPool = context->createCommandPool(device.value, transferQueue.familyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
              transferCommandBuffer = context->createCommandBuffer(device.value, transferCommandPool.value);

              graphicsQueue = context->createQueue(
                  device.value,
                  physicalDevice.queueFamilyIndexInfo.graphicsFamily.value(),
                  0
              );
              graphicsCommandPool = context->createCommandPool(device.value, graphicsQueue.familyIndex, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
              graphicsCommandBuffers = context->createCommandBuffers(device.value, graphicsCommandPool.value, MAX_FRAMES_IN_FLIGHT);

              presentQueue = context->createQueue(
                  device.value,
                  physicalDevice.queueFamilyIndexInfo.presentFamily.value(),
                  0
              );

              imageAvailableSemaphores = context->createSemaphores(device.value, MAX_FRAMES_IN_FLIGHT);
              renderFinishedSemaphores = context->createSemaphores(device.value, MAX_FRAMES_IN_FLIGHT);
              inFlightFences = context->createFences(device.value, MAX_FRAMES_IN_FLIGHT);

              VkCommandBufferBeginInfo beginInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

              if (vkBeginCommandBuffer(transferCommandBuffer.value, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to begin command buffer!");
              }

              cmdCopyBuffer(
                  transferCommandBuffer.value,
                  vertexStagingBuffer.value,
                  vertexBuffer.value,
                  vertexStagingBuffer.memorySize
              );

              cmdCopyBuffer(
                  transferCommandBuffer.value,
                  indexStagingBuffer.value,
                  indexBuffer.value,
                  indexStagingBuffer.memorySize
              );

              cmdPipelineBarrier(
                  transferCommandBuffer.value,
                  samplerImage.value,
                  samplerImage.format,
                  VK_IMAGE_LAYOUT_UNDEFINED,
                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
              );
              cmdCopyBufferToImage(
                  transferCommandBuffer.value,
                  samplerImageStaging.value,
                  samplerImage.value,
                  (uint32_t) samplerImage.width,
                  (uint32_t) samplerImage.height
              );
              cmdPipelineBarrier(
                  transferCommandBuffer.value,
                  samplerImage.value,
                  samplerImage.format,
                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                  VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
              );

              cmdPipelineBarrier(
                  transferCommandBuffer.value,
                  depthImage.value,
                  depthImage.format,
                  VK_IMAGE_LAYOUT_UNDEFINED,
                  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
              );

              if (vkEndCommandBuffer(transferCommandBuffer.value) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to end command buffer!");
              }

              VkSubmitInfo submitInfo {
                  .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                  .commandBufferCount = 1,
                  .pCommandBuffers = &transferCommandBuffer.value
              };
              vkQueueSubmit(transferQueue.value, 1, &submitInfo, VK_NULL_HANDLE);
              vkQueueWaitIdle(transferQueue.value);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void drawFrame(int width, int height) {
            try {
              vkWaitForFences(device.value, 1, &inFlightFences[currentFrame].value, VK_TRUE, UINT64_MAX);

              uint32_t imageIndex;
              VkResult result = vkAcquireNextImageKHR(device.value, swapChain.value, UINT64_MAX, imageAvailableSemaphores[currentFrame].value, VK_NULL_HANDLE, &imageIndex);

              if (result == VK_ERROR_OUT_OF_DATE_KHR) {
                reCreateSwapChain(width, height);
                return;
              } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
                throw std::runtime_error("failed to acquire swap chain image!");
              }

              updateUniformBuffer(currentFrame);

              vkResetFences(device.value, 1, &inFlightFences[currentFrame].value);

              vkResetCommandBuffer(graphicsCommandBuffers[currentFrame].value, 0);
              recordCommandBuffer(
                  graphicsCommandBuffers[currentFrame].value,
                  imageIndex,
                  renderPass.value,
                  swapChainFrameBuffers,
                  swapChain.extent,
                  graphicsPipeline.value,
                  vertexBuffer.value,
                  indexBuffer.value,
                  graphicsPipeline.layout,
                  descriptorSets,
                  currentFrame
              );

              VkSubmitInfo submitInfo{};
              submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

              VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame].value};
              VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
              submitInfo.waitSemaphoreCount = 1;
              submitInfo.pWaitSemaphores = waitSemaphores;
              submitInfo.pWaitDstStageMask = waitStages;

              submitInfo.commandBufferCount = 1;
              submitInfo.pCommandBuffers = &graphicsCommandBuffers[currentFrame].value;

              VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame].value};
              submitInfo.signalSemaphoreCount = 1;
              submitInfo.pSignalSemaphores = signalSemaphores;

              if (vkQueueSubmit(graphicsQueue.value, 1, &submitInfo, inFlightFences[currentFrame].value) != VK_SUCCESS) {
                throw std::runtime_error("failed to submit draw command buffer!");
              }

              VkPresentInfoKHR presentInfo{};
              presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

              presentInfo.waitSemaphoreCount = 1;
              presentInfo.pWaitSemaphores = signalSemaphores;

              VkSwapchainKHR swapChains[] = {swapChain.value};
              presentInfo.swapchainCount = 1;
              presentInfo.pSwapchains = swapChains;

              presentInfo.pImageIndices = &imageIndex;

              result = vkQueuePresentKHR(presentQueue.value, &presentInfo);

              if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || resized) {
                resized = false;
                reCreateSwapChain(width, height);
              } else if (result != VK_SUCCESS) {
                throw std::runtime_error("failed to present swap chain image!");
              }

              currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void waitIdle() {
            try {
              vkDeviceWaitIdle(device.value);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void destroy() {
            try {
              context->destroy();
              delete context;
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

        private:

          Surface createSurface(VkInstance& instance) {
            try {
              VkSurfaceKHR result;
              if (glfwCreateWindowSurface(instance, window, nullptr, &result) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to create surface!");
              }
              return {
                  .instance = instance,
                  .value = result
              };
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void reCreateSwapChain(int width, int height) {
            try {
              std::cout << __FUNCTION__ << " width: " << width << " height: " << height << std::endl;

              vkDeviceWaitIdle(device.value);

              context->destroyFrameBuffers(swapChainFrameBuffers);
              context->destroyPipeline(graphicsPipeline);
              context->destroyRenderPass(renderPass);
              context->destroyImageView(depthImageView);
              context->destroyImage(depthImage);
              context->destroyImageViews(swapChainImageViews);
              context->destroySwapChain(swapChain);

              physicalDevice.swapChainSupportDetails = context->querySwapChainSupport(physicalDevice.value, surface.value);

              swapChain = context->createSwapChain(
                  physicalDevice.swapChainSupportDetails.value(),
                  physicalDevice.queueFamilyIndexInfo,
                  surface.value,
                  device.value,
                  (uint32_t) width,
                  (uint32_t) height
              );
              swapChainImages = context->createSwapChainImages(device.value, swapChain.value);
              swapChainImageViews = context->createImageViews(device.value, swapChainImages, swapChain.format);

              depthImage = context->createImage(
                  physicalDevice.value,
                  device.value,
                  swapChain.width,
                  swapChain.height,
                  context->findDepthFormat(physicalDevice.value),
                  VK_IMAGE_TILING_OPTIMAL,
                  VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
                  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
              );
              depthImageView = context->createImageView(device.value, depthImage.value, depthImage.format, VK_IMAGE_ASPECT_DEPTH_BIT);

              renderPass = context->createRenderPass(
                  device.value,
                  RenderPassCreateInfo {
                      .attachments = {
                          VkAttachmentDescription {
                              .format = swapChain.format,
                              .samples = VK_SAMPLE_COUNT_1_BIT,
                              .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                              .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                              .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                              .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                              .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                              .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                          },
                          VkAttachmentDescription {
                              .format = depthImage.format,
                              .samples = VK_SAMPLE_COUNT_1_BIT,
                              .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                              .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                              .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                              .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                              .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                              .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                          }
                      },
                      .subPasses = {
                          SubPassDescription {
                              .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                              .colorAttachments = {
                                  VkAttachmentReference {
                                      .attachment = 0,
                                      .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                  }
                              },
                              .depthStencilAttachment = VkAttachmentReference {
                                  .attachment = 1,
                                  .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                              }
                          }
                      },
                      .dependencies = {
                          VkSubpassDependency {
                              .srcSubpass = VK_SUBPASS_EXTERNAL,
                              .dstSubpass = 0,
                              .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                              .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
                              .srcAccessMask = 0,
                              .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
                          }
                      }
                  }
              );
              graphicsPipeline = context->createGraphicsPipeline(
                  device.value,
                  swapChain.extent,
                  {"resources/shader/shader-4.vert.spv", "resources/shader/shader-4.frag.spv"},
                  renderPass.value,
                  VK_FRONT_FACE_COUNTER_CLOCKWISE,
                  {descriptorSetLayout.value},
                  {Vertex::getBindingDescription()},
                  Vertex::getAttributeDescriptions()
              );

              std::vector<FrameBufferCreateInfo> frameBufferCreateInfoVector;
              frameBufferCreateInfoVector.resize(swapChainImageViews.size());
              for (std::size_t i = 0; i < frameBufferCreateInfoVector.size(); i++) {
                frameBufferCreateInfoVector[i] = FrameBufferCreateInfo {
                    .flags = 0,
                    .renderPass = renderPass.value,
                    .attachments = {
                        swapChainImageViews[i].value,
                        depthImageView.value
                    },
                    .width = swapChain.extent.width,
                    .height = swapChain.extent.height,
                    .layers = 1
                };
              }
              swapChainFrameBuffers = context->createFrameBuffers(device.value, frameBufferCreateInfoVector);

              VkCommandBufferBeginInfo beginInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

              if (vkBeginCommandBuffer(transferCommandBuffer.value, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to begin command buffer!");
              }

              cmdPipelineBarrier(
                  transferCommandBuffer.value,
                  depthImage.value,
                  depthImage.format,
                  VK_IMAGE_LAYOUT_UNDEFINED,
                  VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
              );

              if (vkEndCommandBuffer(transferCommandBuffer.value) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to end command buffer!");
              }

              VkSubmitInfo submitInfo {
                  .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
                  .commandBufferCount = 1,
                  .pCommandBuffers = &transferCommandBuffer.value
              };
              vkQueueSubmit(transferQueue.value, 1, &submitInfo, nullptr);
              vkQueueWaitIdle(transferQueue.value);
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void updateUniformBuffer(uint32_t currentImage) {
            try {
              static auto startTime = std::chrono::high_resolution_clock::now();

              auto currentTime = std::chrono::high_resolution_clock::now();
              float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

              UniformBufferObject ubo = {};
              ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
              ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
              ubo.proj = glm::perspective(glm::radians(45.0f), (float) swapChain.extent.width / (float) swapChain.extent.height, 0.1f, 10.0f);
              ubo.proj[1][1] *= -1;

              copyToMemory(
                  device.value,
                  uniformBuffers[currentImage].memory,
                  0,
                  uniformBuffers[currentImage].memorySize,
                  &ubo
              );
            } catch (...) {
              std::throw_with_nested(std::runtime_error(CALL_INFO()));
            }
          }

          void recordCommandBuffer(
              VkCommandBuffer& commandBuffer,
              uint32_t imageIndex,
              VkRenderPass& renderPass,
              std::vector<FrameBuffer>& swapChainFramebuffers,
              VkExtent2D& swapChainExtent,
              VkPipeline& graphicsPipeline,
              VkBuffer& vertexBuffer,
              VkBuffer& indexBuffer,
              VkPipelineLayout& pipelineLayout,
              std::vector<DescriptorSet>& descriptorSets,
              std::size_t currentFrame
          ) {
            try {
              VkCommandBufferBeginInfo beginInfo = { .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };

              if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to begin recording command buffer!");
              }

              VkRenderPassBeginInfo renderPassInfo = {};
              renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
              renderPassInfo.renderPass = renderPass;
              renderPassInfo.framebuffer = swapChainFramebuffers[imageIndex].value;
              renderPassInfo.renderArea.offset = {0, 0};
              renderPassInfo.renderArea.extent = swapChainExtent;

              std::vector<VkClearValue> clearValues = {
                  VkClearValue {
                      .color = {0.0f, 0.0f, 0.0f, 1.0f}
                  },
                  VkClearValue {
                      .depthStencil = {1.0f, 0}
                  }
              };

              renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
              renderPassInfo.pClearValues = clearValues.data();

              vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

              vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

              VkBuffer vertexBuffers[] = {vertexBuffer};
              VkDeviceSize offsets[] = {0};
              vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

              vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

              vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame].value, 0, nullptr);

              vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

              vkCmdEndRenderPass(commandBuffer);

              if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
                throw std::runtime_error(CALL_INFO() + ": failed to record command buffer!");
              }
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
              environment->create(arguments, window);

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

  };

  TEST_F(UiTestsA, test1) {
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
