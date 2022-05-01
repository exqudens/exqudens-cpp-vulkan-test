#pragma once

#include "exqudens/vulkan/model/ImageCreateInfo.hpp"
#include "exqudens/vulkan/model/Image.hpp"

namespace exqudens::vulkan {

  class ImageFactory {

    public:

      virtual Image createImage(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          const ImageCreateInfo& createInfo,
          VkMemoryPropertyFlags properties
      ) = 0;
      virtual Image createImage(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          uint32_t width,
          uint32_t height,
          VkFormat format,
          VkImageTiling tiling,
          VkImageUsageFlags usage,
          VkMemoryPropertyFlags properties
      ) = 0;
      virtual std::vector<Image> createImages(
          VkPhysicalDevice& physicalDevice,
          VkDevice& device,
          uint32_t width,
          uint32_t height,
          VkFormat format,
          VkImageTiling tiling,
          VkImageUsageFlags usage,
          VkMemoryPropertyFlags properties,
          std::size_t size
      ) = 0;

      virtual void destroyImage(Image& image) = 0;
      virtual void destroyImage(Image& image, bool unmapMemory) = 0;
      virtual void destroyImages(std::vector<Image>& images) = 0;
      virtual void destroyImages(std::vector<Image>& images, bool unmapMemory) = 0;

  };

}
