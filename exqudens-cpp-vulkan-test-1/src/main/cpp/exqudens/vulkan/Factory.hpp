#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/factory/UtilityFactory.hpp"
#include "exqudens/vulkan/factory/InstanceFactory.hpp"
#include "exqudens/vulkan/factory/DebugUtilsMessengerFactory.hpp"
#include "exqudens/vulkan/factory/PhysicalDeviceFactory.hpp"
#include "exqudens/vulkan/factory/DeviceFactory.hpp"
#include "exqudens/vulkan/factory/BufferFactory.hpp"
#include "exqudens/vulkan/factory/ImageFactory.hpp"
#include "exqudens/vulkan/factory/ImageViewFactory.hpp"
#include "exqudens/vulkan/factory/DescriptorSetLayoutFactory.hpp"
#include "exqudens/vulkan/factory/RenderPassFactory.hpp"
#include "exqudens/vulkan/factory/PipelineFactory.hpp"
#include "exqudens/vulkan/factory/SamplerFactory.hpp"
#include "exqudens/vulkan/factory/DescriptorPoolFactory.hpp"
#include "exqudens/vulkan/factory/DescriptorSetFactory.hpp"
#include "exqudens/vulkan/factory/FrameBufferFactory.hpp"
#include "exqudens/vulkan/factory/QueueFactory.hpp"
#include "exqudens/vulkan/factory/CommandPoolFactory.hpp"
#include "exqudens/vulkan/factory/CommandBufferFactory.hpp"
#include "exqudens/vulkan/factory/SemaphoreFactory.hpp"
#include "exqudens/vulkan/factory/FenceFactory.hpp"
#include "exqudens/vulkan/factory/SurfaceFactory.hpp"
#include "exqudens/vulkan/factory/SwapChainFactory.hpp"

namespace exqudens::vulkan {

  class Factory:
      virtual public Utility,
      virtual public UtilityFactory,
      virtual public InstanceFactory,
      virtual public DebugUtilsMessengerFactory,
      virtual public PhysicalDeviceFactory,
      virtual public DeviceFactory,
      virtual public BufferFactory,
      virtual public ImageFactory,
      virtual public ImageViewFactory,
      virtual public DescriptorSetLayoutFactory,
      virtual public RenderPassFactory,
      virtual public PipelineFactory,
      virtual public SamplerFactory,
      virtual public DescriptorPoolFactory,
      virtual public DescriptorSetFactory,
      virtual public FrameBufferFactory,
      virtual public QueueFactory,
      virtual public CommandPoolFactory,
      virtual public CommandBufferFactory,
      virtual public SemaphoreFactory,
      virtual public FenceFactory,
      virtual public SurfaceFactory,
      virtual public SwapChainFactory
  {

    public:

      ~Factory() override = default;

  };

}
