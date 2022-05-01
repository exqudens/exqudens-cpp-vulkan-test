#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/ImageViewFactory.hpp"

namespace exqudens::vulkan {

  class ImageViewFactoryBase:
      virtual public ImageViewFactory,
      virtual public UtilityBase
  {

    public:

      ImageView createImageView(VkDevice& device, VkImage& image, VkFormat format) override {
        try {
          return createImageView(device, image, format, VK_IMAGE_ASPECT_COLOR_BIT);
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      ImageView createImageView(
          VkDevice& device,
          VkImage& image,
          VkFormat format,
          VkImageAspectFlags aspectMask
      ) override {
        try {
          VkImageViewCreateInfo viewInfo = {};
          viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
          viewInfo.image = image;
          viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
          viewInfo.format = format;
          //viewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
          //viewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
          //viewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
          //viewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
          viewInfo.subresourceRange.aspectMask = aspectMask;
          viewInfo.subresourceRange.baseMipLevel = 0;
          viewInfo.subresourceRange.levelCount = 1;
          viewInfo.subresourceRange.baseArrayLayer = 0;
          viewInfo.subresourceRange.layerCount = 1;

          VkImageView imageView = nullptr;

          if (
              functions().createImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS
              || imageView == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create texture image view!");
          }

          return {
              .device = device,
              .value = imageView
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<ImageView> createImageViews(
          VkDevice& device,
          std::vector<Image>& images,
          VkFormat format
      ) override {
        try {
          std::vector<ImageView> imageViews;
          imageViews.resize(images.size());

          for (uint32_t i = 0; i < imageViews.size(); i++) {
            imageViews[i] = createImageView(device, images[i].value, format);
          }

          return imageViews;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      std::vector<ImageView> createImageViews(
          VkDevice& device,
          std::vector<VkImage>& images,
          VkFormat format,
          VkImageAspectFlags aspectMask
      ) override {
        try {
          std::vector<ImageView> imageViews;
          imageViews.resize(images.size());

          for (uint32_t i = 0; i < imageViews.size(); i++) {
            imageViews[i] = createImageView(device, images[i], format, aspectMask);
          }

          return imageViews;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImageView(ImageView& imageView) override {
        try {
          if (imageView.value != nullptr) {
            functions().destroyImageView(imageView.device, imageView.value, nullptr);
            imageView.device = nullptr;
            imageView.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyImageViews(std::vector<ImageView>& imageViews) override {
        try {
          for (ImageView& imageView : imageViews) {
            destroyImageView(imageView);
          }
          imageViews.clear();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
