#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/FenceFactory.hpp"

namespace exqudens::vulkan {

  class FenceFactoryBase:
      virtual public FenceFactory,
      virtual public UtilityBase
  {

    public:

      Fence createFence(VkDevice& device) override {
        try {
          return createFence(device, VK_FENCE_CREATE_SIGNALED_BIT);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Fence createFence(VkDevice& device, VkFenceCreateFlags flags) override {
        try {
          VkFence fence = nullptr;

          VkFenceCreateInfo fenceInfo{};
          fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
          fenceInfo.flags = flags;

          if (
              functions().createFence(device, &fenceInfo, nullptr, &fence) != VK_SUCCESS
              || fence == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create fence!");
          }

          return {
              .device = device,
              .value = fence
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<Fence> createFences(VkDevice& device, std::size_t size) override {
        try {
          std::vector<Fence> fences;
          fences.resize(size);
          for (std::size_t i = 0; i < size; i++) {
            fences[i] = createFence(device);
          }
          return fences;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<Fence> createFences(VkDevice& device, VkFenceCreateFlags flags, std::size_t size) override {
        try {
          std::vector<Fence> fences;
          fences.resize(size);
          for (std::size_t i = 0; i < size; i++) {
            fences[i] = createFence(device, flags);
          }
          return fences;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyFence(Fence& fence) override {
        try {
          if (fence.value != nullptr) {
            functions().destroyFence(fence.device, fence.value, nullptr);
            fence.device = nullptr;
            fence.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyFences(std::vector<Fence>& fences) override {
        try {
          for (Fence& fence : fences) {
            destroyFence(fence);
          }
          fences.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
