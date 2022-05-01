#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/SurfaceFactory.hpp"

namespace exqudens::vulkan {

  class SurfaceFactoryBase:
      virtual public SurfaceFactory,
      virtual public UtilityBase
  {

    public:

      Surface createSurface(VkInstance& instance) override {
        try {
          throw std::runtime_error("Not implemented!!!");
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroySurface(Surface& surface) override {
        try {
          if (surface.value != nullptr) {
            functions().destroySurfaceKHR(surface.instance, surface.value, nullptr);
            surface.instance = nullptr;
            surface.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
