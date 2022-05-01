#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/FrameBufferFactory.hpp"

namespace exqudens::vulkan {

  class FrameBufferFactoryBase:
      virtual public FrameBufferFactory,
      virtual public UtilityBase
  {

    public:

      FrameBuffer createFrameBuffer(VkDevice& device, const FrameBufferCreateInfo& createInfo) override {
        try {
          VkFramebuffer frameBuffer = nullptr;

          VkFramebufferCreateInfo frameBufferInfo = {
              .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
              .flags = createInfo.flags,
              .renderPass = createInfo.renderPass,
              .attachmentCount = static_cast<uint32_t>(createInfo.attachments.size()),
              .pAttachments = createInfo.attachments.data(),
              .width = createInfo.width,
              .height = createInfo.height,
              .layers = createInfo.layers
          };

          if (
              functions().createFramebuffer(device, &frameBufferInfo, nullptr, &frameBuffer) != VK_SUCCESS
              || frameBuffer == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create frame buffer!");
          }

          return {
              .device = device,
              .value = frameBuffer
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<FrameBuffer> createFrameBuffers(
          VkDevice& device,
          const std::vector<FrameBufferCreateInfo>& createInfo
      ) override {
        try {
          std::vector<FrameBuffer> frameBuffers;
          frameBuffers.resize(createInfo.size());

          for (std::size_t i = 0; i < frameBuffers.size(); i++) {
            frameBuffers[i] = createFrameBuffer(device, createInfo[i]);
          }

          return frameBuffers;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyFrameBuffer(FrameBuffer& frameBuffer) override {
        try {
          if (frameBuffer.value != nullptr) {
            functions().destroyFramebuffer(frameBuffer.device, frameBuffer.value, nullptr);
            frameBuffer.device = nullptr;
            frameBuffer.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyFrameBuffers(std::vector<FrameBuffer>& frameBuffers) override {
        try {
          for (FrameBuffer& frameBuffer : frameBuffers) {
            destroyFrameBuffer(frameBuffer);
          }
          frameBuffers.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
