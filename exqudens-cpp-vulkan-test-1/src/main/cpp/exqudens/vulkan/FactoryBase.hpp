#pragma once

#include "exqudens/vulkan/Factory.hpp"
#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/UtilityFactoryBase.hpp"
#include "exqudens/vulkan/factory/InstanceFactoryBase.hpp"
#include "exqudens/vulkan/factory/DebugUtilsMessengerFactoryBase.hpp"
#include "exqudens/vulkan/factory/PhysicalDeviceFactoryBase.hpp"
#include "exqudens/vulkan/factory/DeviceFactoryBase.hpp"
#include "exqudens/vulkan/factory/BufferFactoryBase.hpp"
#include "exqudens/vulkan/factory/ImageFactoryBase.hpp"
#include "exqudens/vulkan/factory/ImageViewFactoryBase.hpp"
#include "exqudens/vulkan/factory/DescriptorSetLayoutFactoryBase.hpp"
#include "exqudens/vulkan/factory/RenderPassFactoryBase.hpp"
#include "exqudens/vulkan/factory/PipelineFactoryBase.hpp"
#include "exqudens/vulkan/factory/SamplerFactoryBase.hpp"
#include "exqudens/vulkan/factory/DescriptorPoolFactoryBase.hpp"
#include "exqudens/vulkan/factory/DescriptorSetFactoryBase.hpp"
#include "exqudens/vulkan/factory/FrameBufferFactoryBase.hpp"
#include "exqudens/vulkan/factory/QueueFactoryBase.hpp"
#include "exqudens/vulkan/factory/CommandPoolFactoryBase.hpp"
#include "exqudens/vulkan/factory/CommandBufferFactoryBase.hpp"
#include "exqudens/vulkan/factory/SemaphoreFactoryBase.hpp"
#include "exqudens/vulkan/factory/FenceFactoryBase.hpp"
#include "exqudens/vulkan/factory/SurfaceFactoryBase.hpp"
#include "exqudens/vulkan/factory/SwapChainFactoryBase.hpp"

namespace exqudens::vulkan {

  class FactoryBase:
      virtual public Factory,
      virtual public UtilityBase,
      virtual public UtilityFactoryBase,
      virtual public InstanceFactoryBase,
      virtual public DebugUtilsMessengerFactoryBase,
      virtual public PhysicalDeviceFactoryBase,
      virtual public DeviceFactoryBase,
      virtual public BufferFactoryBase,
      virtual public ImageFactoryBase,
      virtual public ImageViewFactoryBase,
      virtual public DescriptorSetLayoutFactoryBase,
      virtual public RenderPassFactoryBase,
      virtual public PipelineFactoryBase,
      virtual public SamplerFactoryBase,
      virtual public DescriptorPoolFactoryBase,
      virtual public DescriptorSetFactoryBase,
      virtual public FrameBufferFactoryBase,
      virtual public QueueFactoryBase,
      virtual public CommandPoolFactoryBase,
      virtual public CommandBufferFactoryBase,
      virtual public SemaphoreFactoryBase,
      virtual public FenceFactoryBase,
      virtual public SurfaceFactoryBase,
      virtual public SwapChainFactoryBase
  {
  };

}
