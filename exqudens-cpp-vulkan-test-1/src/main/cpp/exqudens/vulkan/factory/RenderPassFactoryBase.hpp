#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/RenderPassFactory.hpp"

namespace exqudens::vulkan {

  class RenderPassFactoryBase:
      virtual public RenderPassFactory,
      virtual public UtilityBase
  {

    public:

      RenderPass createRenderPass(VkDevice& device, VkFormat& format) override {
        try {
          return createRenderPass(
              device,
              RenderPassCreateInfo {
                  .attachments = {
                      VkAttachmentDescription {
                          .format = format,
                          .samples = VK_SAMPLE_COUNT_1_BIT,
                          .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                          .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
                          .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
                          .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                          .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                          .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
                      }
                  },
                  .subPasses = {
                      SubPassDescription {
                          .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
                          .colorAttachments = {
                              VkAttachmentReference {
                                  .attachment = 0,
                                  .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                              }
                          }
                      }
                  },
                  .dependencies = {
                      VkSubpassDependency {
                          .srcSubpass = VK_SUBPASS_EXTERNAL,
                          .dstSubpass = 0,
                          .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                          .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
                          .srcAccessMask = 0,
                          .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
                      }
                  }
              }
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      RenderPass createRenderPass(VkDevice& device, const RenderPassCreateInfo& createInfo) override {
        try {
          VkRenderPass renderPass = nullptr;

          std::vector<VkSubpassDescription> subPasses;

          for (const SubPassDescription& subPassDescription : createInfo.subPasses) {
            VkSubpassDescription subPass = {
                .flags = subPassDescription.flags,
                .pipelineBindPoint = subPassDescription.pipelineBindPoint,
                .inputAttachmentCount = static_cast<uint32_t>(subPassDescription.inputAttachments.size()),
                .pInputAttachments = subPassDescription.inputAttachments.data(),
                .colorAttachmentCount = static_cast<uint32_t>(subPassDescription.colorAttachments.size()),
                .pColorAttachments = subPassDescription.colorAttachments.data(),
                .pResolveAttachments = subPassDescription.resolveAttachments.data(),
                .pDepthStencilAttachment = subPassDescription.depthStencilAttachment.has_value() ? &subPassDescription.depthStencilAttachment.value() : nullptr,
                .preserveAttachmentCount = static_cast<uint32_t>(subPassDescription.preserveAttachments.size()),
                .pPreserveAttachments = subPassDescription.preserveAttachments.data()
            };

            subPasses.emplace_back(subPass);
          }

          VkRenderPassCreateInfo info = {
              .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
              .pNext = nullptr,
              .flags = createInfo.flags,
              .attachmentCount = static_cast<uint32_t>(createInfo.attachments.size()),
              .pAttachments = createInfo.attachments.data(),
              .subpassCount = static_cast<uint32_t>(subPasses.size()),
              .pSubpasses = subPasses.data(),
              .dependencyCount = static_cast<uint32_t>(createInfo.dependencies.size()),
              .pDependencies = createInfo.dependencies.data()
          };

          if (
              functions().createRenderPass(device, &info, nullptr, &renderPass) != VK_SUCCESS
              || renderPass == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create render pass!");
          }

          return {
              .device = device,
              .value = renderPass
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyRenderPass(RenderPass& renderPass) override {
        try {
          if (renderPass.value != nullptr) {
            functions().destroyRenderPass(renderPass.device, renderPass.value, nullptr);
            renderPass.device = nullptr;
            renderPass.value = nullptr;
          }
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
