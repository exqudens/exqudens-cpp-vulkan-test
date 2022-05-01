#pragma once

#include "exqudens/vulkan/model/FrameBufferCreateInfo.hpp"
#include "exqudens/vulkan/model/FrameBuffer.hpp"

namespace exqudens::vulkan {

  class FrameBufferFactory {

    public:

      virtual FrameBuffer createFrameBuffer(VkDevice& device, const FrameBufferCreateInfo& createInfo) = 0;
      virtual std::vector<FrameBuffer> createFrameBuffers(
          VkDevice& device,
          const std::vector<FrameBufferCreateInfo>& createInfo
      ) = 0;

      virtual void destroyFrameBuffer(FrameBuffer& frameBuffer) = 0;
      virtual void destroyFrameBuffers(std::vector<FrameBuffer>& frameBuffers) = 0;

  };

}
