#pragma once

#include "exqudens/vulkan/model/ImageView.hpp"

namespace exqudens::vulkan {

  class ImageViewFactory {

    public:

      virtual ImageView createImageView(VkDevice& device, VkImage& image, VkFormat format) = 0;
      virtual ImageView createImageView(
          VkDevice& device,
          VkImage& image,
          VkFormat format,
          VkImageAspectFlags aspectMask
      ) = 0;
      virtual std::vector<ImageView> createImageViews(
          VkDevice& device,
          std::vector<Image>& images,
          VkFormat format
      ) = 0;
      virtual std::vector<ImageView> createImageViews(
          VkDevice& device,
          std::vector<VkImage>& images,
          VkFormat format,
          VkImageAspectFlags aspectMask
      ) = 0;

      virtual void destroyImageView(ImageView& imageView) = 0;
      virtual void destroyImageViews(std::vector<ImageView>& imageViews) = 0;

  };

}
