#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/ImageFactory.hpp"

namespace exqudens::vulkan {

  class ImageFactoryBase:
      virtual public ImageFactory,
      virtual public UtilityBase
  {

    public:

      Image createImage(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const ImageCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      ) override {
        try {
          VkImageCreateInfo imageInfo = {
              .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
              .flags = createInfo.flags,
              .imageType = createInfo.imageType,
              .format = createInfo.format,
              .extent = createInfo.extent,
              .mipLevels = createInfo.mipLevels,
              .arrayLayers = createInfo.arrayLayers,
              .samples = createInfo.samples,
              .tiling = createInfo.tiling,
              .usage = createInfo.usage,
              .sharingMode = createInfo.sharingMode,
              .queueFamilyIndexCount = static_cast<uint32_t>(createInfo.queueFamilyIndices.size()),
              .pQueueFamilyIndices = createInfo.queueFamilyIndices.empty() ? nullptr : createInfo.queueFamilyIndices.data(),
              .initialLayout = createInfo.initialLayout
          };

          VkImage image = nullptr;

          if (
              functions().createImage(device, &imageInfo, nullptr, &image) != VK_SUCCESS
              || image == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create image!");
          }

          VkDeviceMemory imageMemory = nullptr;

          VkMemoryRequirements memRequirements;
          functions().getImageMemoryRequirements(device, image, &memRequirements);

          VkMemoryAllocateInfo allocInfo = {};
          allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
          allocInfo.allocationSize = memRequirements.size;
          allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

          if (functions().allocateMemory(device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate image memory!");
          }

          if (functions().bindImageMemory(device, image, imageMemory, 0) != VK_SUCCESS) {
            throw std::runtime_error("failed to bind image memory!");
          }

          return {
              .device = device,
              .width = createInfo.extent.width,
              .height = createInfo.extent.height,
              .format = createInfo.format,
              .memory = imageMemory,
              .memorySize = memRequirements.size,
              .memoryProperties = properties,
              .value = image
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Image createImage(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          uint32_t width,
          uint32_t height,
          VkFormat format,
          VkImageTiling tiling,
          VkImageUsageFlags usage,
          VkMemoryPropertyFlags properties
      ) override {
        try {
          return createImage(
              physicalDevice,
              device,
              ImageCreateInfo {
                  .flags = 0,
                  .imageType = VK_IMAGE_TYPE_2D,
                  .format = format,
                  .extent = VkExtent3D {
                      .width = width,
                      .height = height,
                      .depth = 1
                  },
                  .mipLevels = 1,
                  .arrayLayers = 1,
                  .samples = VK_SAMPLE_COUNT_1_BIT,
                  .tiling = tiling,
                  .usage = usage,
                  .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
                  .queueFamilyIndices = {},
                  .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED
              },
              properties
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<Image> createImages(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          uint32_t width,
          uint32_t height,
          VkFormat format,
          VkImageTiling tiling,
          VkImageUsageFlags usage,
          VkMemoryPropertyFlags properties,
          std::size_t size
      ) override {
        try {
          std::vector<Image> images;
          images.resize(size);
          for (std::size_t i = 0; i < size; i++) {
            images[i] = createImage(physicalDevice, device, width, height, format, tiling, usage, properties);
          }
          return images;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImage(Image& image) override {
        try {
          destroyImage(image, false);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImage(Image& image, bool unmapMemory) override {
        try {
          if (image.device == nullptr) {
            throw std::runtime_error(CALL_INFO() + ": image device is null!");
          }
          if (image.memory != nullptr) {
            if (
                unmapMemory
                && (image.memoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) == VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
            ) {
              functions().unmapMemory(image.device, image.memory);
            }
            functions().freeMemory(image.device, image.memory, nullptr);
            image.memory = nullptr;
          }
          if (image.value != nullptr) {
            functions().destroyImage(image.device, image.value, nullptr);
            image.device = nullptr;
            image.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImages(std::vector<Image>& images) override {
        try {
          for (Image& image : images) {
            destroyImage(image);
          }
          images.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImages(std::vector<Image>& images, bool unmapMemory) override {
        try {
          for (Image& image : images) {
            destroyImage(image, unmapMemory);
          }
          images.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
