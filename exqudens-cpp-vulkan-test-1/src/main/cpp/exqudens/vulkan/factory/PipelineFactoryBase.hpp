#pragma once

#include "exqudens/vulkan/UtilityBase.hpp"
#include "exqudens/vulkan/factory/PipelineFactory.hpp"

namespace exqudens::vulkan {

  class PipelineFactoryBase:
      virtual public PipelineFactory,
      virtual public UtilityBase
  {

    public:

      Shader createShader(VkDevice& device, const std::string& path) override {
        try {
          VkShaderModule shaderModule = nullptr;

          const std::vector<char>& code = readFile(path);

          if (code.empty()) {
            throw std::runtime_error(CALL_INFO() + ": failed to create shader module code is empty!");
          }

          VkShaderModuleCreateInfo createInfo{};
          createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
          createInfo.codeSize = code.size();
          createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

          if (
              functions().createShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS
              || shaderModule == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create shader module!");
          }

          VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
          pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;

          if (path.ends_with(".vert.spv")) {
            pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
          } else if (path.ends_with(".frag.spv")) {
            pipelineShaderStageCreateInfo.stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
          } else {
            throw std::invalid_argument(CALL_INFO() + ": '" + path + "' failed to create shader!");
          }

          pipelineShaderStageCreateInfo.module = shaderModule;
          pipelineShaderStageCreateInfo.pName = "main";

          return {
              .device = device,
              .shaderModule = shaderModule,
              .pipelineShaderStageCreateInfo = pipelineShaderStageCreateInfo
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Pipeline createPipeline(
          VkDevice& device,
          const std::vector<std::string>& shaderPaths,
          const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions,
          const std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions,
          const PipelineLayoutCreateInfo& layoutCreateInfo,
          const GraphicsPipelineCreateInfo& createInfo
      ) override {
        try {
          VkPipelineLayout pipelineLayout = nullptr;

          VkPipelineLayoutCreateInfo vkLayoutCreateInfo = {
              .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
              .flags = layoutCreateInfo.flags,
              .setLayoutCount = static_cast<uint32_t>(layoutCreateInfo.setLayouts.size()),
              .pSetLayouts = layoutCreateInfo.setLayouts.empty() ? nullptr : layoutCreateInfo.setLayouts.data(),
              .pushConstantRangeCount = static_cast<uint32_t>(layoutCreateInfo.pushConstantRanges.size()),
              .pPushConstantRanges = layoutCreateInfo.pushConstantRanges.empty() ? nullptr : layoutCreateInfo.pushConstantRanges.data()
          };

          if (
              functions().createPipelineLayout(device, &vkLayoutCreateInfo, nullptr, &pipelineLayout) != VK_SUCCESS
              || pipelineLayout == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create pipeline layout!");
          }

          VkPipeline pipeline = nullptr;

          VkGraphicsPipelineCreateInfo iCreateInfo = {};
          iCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

          std::vector<Shader> shaders;
          shaders.resize(shaderPaths.size());
          std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
          shaderStages.resize(shaders.size());
          for (std::size_t i = 0; i < shaderPaths.size(); i++) {
            shaders[i] = createShader(device, shaderPaths[i]);
            shaderStages[i] = shaders[i].pipelineShaderStageCreateInfo;
          }

          iCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
          iCreateInfo.pStages = shaderStages.data();

          VkPipelineVertexInputStateCreateInfo vertexInputInfo = {
              .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
              .vertexBindingDescriptionCount = static_cast<uint32_t>(vertexBindingDescriptions.size()),
              .pVertexBindingDescriptions = vertexBindingDescriptions.empty() ? nullptr : vertexBindingDescriptions.data(),
              .vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexAttributeDescriptions.size()),
              .pVertexAttributeDescriptions = vertexAttributeDescriptions.empty() ? nullptr : vertexAttributeDescriptions.data()
          };

          iCreateInfo.pVertexInputState = &vertexInputInfo;

          if (createInfo.inputAssemblyState.has_value()) {
            VkPipelineInputAssemblyStateCreateInfo vkPipelineInputAssemblyStateCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
                .flags = createInfo.inputAssemblyState.value().flags,
                .topology = createInfo.inputAssemblyState.value().topology,
                .primitiveRestartEnable = createInfo.inputAssemblyState.value().primitiveRestartEnable
            };

            iCreateInfo.pInputAssemblyState = &vkPipelineInputAssemblyStateCreateInfo;
          }

          if (createInfo.tessellationState.has_value()) {
            VkPipelineTessellationStateCreateInfo vkPipelineTessellationStateCreateInfo {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO,
                .flags = createInfo.tessellationState.value().flags,
                .patchControlPoints = createInfo.tessellationState.value().patchControlPoints
            };
            iCreateInfo.pTessellationState = &vkPipelineTessellationStateCreateInfo;
          }

          if (createInfo.viewportState.has_value()) {
            VkPipelineViewportStateCreateInfo viewportState = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
                .viewportCount = static_cast<uint32_t>(createInfo.viewportState.value().viewports.size()),
                .pViewports = createInfo.viewportState.value().viewports.empty() ? nullptr : createInfo.viewportState.value().viewports.data(),
                .scissorCount = static_cast<uint32_t>(createInfo.viewportState.value().scissors.size()),
                .pScissors = createInfo.viewportState.value().scissors.empty() ? nullptr : createInfo.viewportState.value().scissors.data()
            };

            iCreateInfo.pViewportState = &viewportState;
          }

          if (createInfo.rasterizationState.has_value()) {
            VkPipelineRasterizationStateCreateInfo vkPipelineRasterizationStateCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
                .flags = createInfo.rasterizationState.value().flags,
                .depthClampEnable = createInfo.rasterizationState.value().depthClampEnable,
                .rasterizerDiscardEnable = createInfo.rasterizationState.value().rasterizerDiscardEnable,
                .polygonMode = createInfo.rasterizationState.value().polygonMode,
                .cullMode = createInfo.rasterizationState.value().cullMode,
                .frontFace = createInfo.rasterizationState.value().frontFace,
                .depthBiasEnable = createInfo.rasterizationState.value().depthBiasEnable,
                .depthBiasConstantFactor = createInfo.rasterizationState.value().depthBiasConstantFactor,
                .depthBiasClamp = createInfo.rasterizationState.value().depthBiasClamp,
                .depthBiasSlopeFactor = createInfo.rasterizationState.value().depthBiasSlopeFactor,
                .lineWidth = createInfo.rasterizationState.value().lineWidth
            };
            iCreateInfo.pRasterizationState = &vkPipelineRasterizationStateCreateInfo;
          }

          if (createInfo.multisampleState.has_value()) {
            VkPipelineMultisampleStateCreateInfo vkPipelineMultisampleStateCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
                .flags = createInfo.multisampleState.value().flags,
                .rasterizationSamples = createInfo.multisampleState.value().rasterizationSamples,
                .sampleShadingEnable = createInfo.multisampleState.value().sampleShadingEnable,
                .minSampleShading = createInfo.multisampleState.value().minSampleShading,
                .pSampleMask = createInfo.multisampleState.value().pSampleMask,
                .alphaToCoverageEnable = createInfo.multisampleState.value().alphaToCoverageEnable,
                .alphaToOneEnable = createInfo.multisampleState.value().alphaToOneEnable
            };
            iCreateInfo.pMultisampleState = &vkPipelineMultisampleStateCreateInfo;
          }

          if (createInfo.depthStencilState.has_value()) {
            VkPipelineDepthStencilStateCreateInfo vkPipelineDepthStencilStateCreateInfo = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
                .flags = createInfo.depthStencilState.value().flags,
                .depthTestEnable = createInfo.depthStencilState.value().depthTestEnable,
                .depthWriteEnable = createInfo.depthStencilState.value().depthWriteEnable,
                .depthCompareOp = createInfo.depthStencilState.value().depthCompareOp,
                .depthBoundsTestEnable = createInfo.depthStencilState.value().depthBoundsTestEnable,
                .stencilTestEnable = createInfo.depthStencilState.value().stencilTestEnable,
                .front = createInfo.depthStencilState.value().front,
                .back = createInfo.depthStencilState.value().back,
                .minDepthBounds = createInfo.depthStencilState.value().minDepthBounds,
                .maxDepthBounds = createInfo.depthStencilState.value().maxDepthBounds
            };
            iCreateInfo.pDepthStencilState = &vkPipelineDepthStencilStateCreateInfo;
          }

          if (createInfo.colorBlendState.has_value()) {
            VkPipelineColorBlendStateCreateInfo colorBlending = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
                .flags = createInfo.colorBlendState.value().flags,
                .logicOpEnable = createInfo.colorBlendState.value().logicOpEnable,
                .logicOp = createInfo.colorBlendState.value().logicOp,
                .attachmentCount = static_cast<uint32_t>(createInfo.colorBlendState.value().attachments.size()),
                .pAttachments = createInfo.colorBlendState.value().attachments.empty() ? nullptr : createInfo.colorBlendState.value().attachments.data()
            };
            colorBlending.blendConstants[0] = createInfo.colorBlendState.value().blendConstants[0];
            colorBlending.blendConstants[1] = createInfo.colorBlendState.value().blendConstants[1];
            colorBlending.blendConstants[2] = createInfo.colorBlendState.value().blendConstants[2];
            colorBlending.blendConstants[3] = createInfo.colorBlendState.value().blendConstants[3];

            iCreateInfo.pColorBlendState = &colorBlending;
          }

          if (createInfo.dynamicState.has_value()) {
            VkPipelineDynamicStateCreateInfo dynamicState {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
                .flags = createInfo.dynamicState.value().flags,
                .dynamicStateCount = static_cast<uint32_t>(createInfo.dynamicState.value().dynamicStates.size()),
                .pDynamicStates = createInfo.dynamicState.value().dynamicStates.empty() ? nullptr : createInfo.dynamicState.value().dynamicStates.data()
            };
            iCreateInfo.pDynamicState = &dynamicState;
          }

          iCreateInfo.layout = pipelineLayout;
          iCreateInfo.renderPass = createInfo.renderPass;
          iCreateInfo.subpass = createInfo.subpass;
          iCreateInfo.basePipelineHandle = createInfo.basePipelineHandle;
          iCreateInfo.basePipelineIndex = createInfo.basePipelineIndex;

          if (
              functions().createGraphicsPipelines(device, VK_NULL_HANDLE, 1, &iCreateInfo, nullptr, &pipeline) != VK_SUCCESS
              || pipeline == nullptr
          ) {
            throw std::runtime_error(CALL_INFO() + ": failed to create graphics pipeline!");
          }

          for (Shader& shader : shaders) {
            destroyShader(shader);
          }

          return {
              .device = device,
              .layout = pipelineLayout,
              .value = pipeline
          };
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Pipeline createGraphicsPipeline(
          VkDevice& device,
          const VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass
      ) override {
        try {
          VkFrontFace frontFace = VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
          std::vector<VkDescriptorSetLayout> descriptorSetLayouts = {};
          std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions = {}; //{Vertex::getBindingDescription()};
          std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions = {}; //Vertex::getAttributeDescriptions();
          return createGraphicsPipeline(
              device,
              extent,
              shaderPaths,
              renderPass,
              frontFace,
              descriptorSetLayouts,
              vertexBindingDescriptions,
              vertexAttributeDescriptions
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      Pipeline createGraphicsPipeline(
          VkDevice& device,
          const VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass,
          VkFrontFace frontFace,
          std::vector<VkDescriptorSetLayout> descriptorSetLayouts,
          std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions,
          std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions
      ) override {
        try {
          return createPipeline(
              device,
              shaderPaths,
              vertexBindingDescriptions,
              vertexAttributeDescriptions,
              PipelineLayoutCreateInfo {
                  .flags = 0,
                  .setLayouts = descriptorSetLayouts,
                  .pushConstantRanges = {}
              },
              GraphicsPipelineCreateInfo {
                  .flags = 0,
                  .inputAssemblyState = PipelineInputAssemblyStateCreateInfo {
                      .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
                      .primitiveRestartEnable = VK_FALSE
                  },
                  .tessellationState = {},
                  .viewportState = PipelineViewportStateCreateInfo {
                      .flags = 0,
                      .viewports = {
                          VkViewport {
                              .x = 0.0f,
                              .y = 0.0f,
                              .width = static_cast<float>(extent.width),
                              .height = static_cast<float>(extent.height),
                              .minDepth = 0.0f,
                              .maxDepth = 1.0f
                          }
                      },
                      .scissors = {
                          VkRect2D {
                              .offset = {0, 0},
                              .extent = extent
                          }
                      }
                  },
                  .rasterizationState = PipelineRasterizationStateCreateInfo {
                      .depthClampEnable = VK_FALSE,
                      .rasterizerDiscardEnable = VK_FALSE,
                      .polygonMode = VK_POLYGON_MODE_FILL,
                      .cullMode = VK_CULL_MODE_BACK_BIT,
                      .frontFace = frontFace, // black screen WARNING!!!
                      .depthBiasEnable = VK_FALSE,
                      .depthBiasConstantFactor = 0.0f, // Optional
                      .depthBiasClamp = 0.0f, // Optional
                      .depthBiasSlopeFactor = 0.0f, // Optional
                      .lineWidth = 1.0f
                  },
                  .multisampleState = PipelineMultisampleStateCreateInfo {
                      .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
                      .sampleShadingEnable = VK_FALSE,
                      .minSampleShading = 1.0f, // Optional
                      .pSampleMask = nullptr, // Optional
                      .alphaToCoverageEnable = VK_FALSE, // Optional
                      .alphaToOneEnable = VK_FALSE // Optional
                  },
                  .depthStencilState = PipelineDepthStencilStateCreateInfo {
                      .depthTestEnable = VK_TRUE,
                      .depthWriteEnable = VK_TRUE,
                      .depthCompareOp = VK_COMPARE_OP_LESS,
                      .depthBoundsTestEnable = VK_FALSE,
                      .stencilTestEnable = VK_FALSE,
                      .front = {}, // Optional
                      .back = {}, // Optional
                      .minDepthBounds = 0.0f, // Optional
                      .maxDepthBounds = 1.0f // Optional
                  },
                  .colorBlendState = PipelineColorBlendStateCreateInfo {
                      .flags = 0,
                      .logicOpEnable = VK_FALSE,
                      .logicOp = VK_LOGIC_OP_COPY,
                      .attachments = {
                          VkPipelineColorBlendAttachmentState {
                              .blendEnable = VK_FALSE,
                              .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                              .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                              .colorBlendOp = VK_BLEND_OP_ADD,
                              .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                              .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                              .alphaBlendOp = VK_BLEND_OP_ADD,
                              .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
                          }
                      },
                      .blendConstants = {0.0f, 0.0f, 0.0f, 0.0f}
                  },
                  .dynamicState = {},
                  .renderPass = renderPass,
                  .subpass = 0,
                  .basePipelineHandle = nullptr,
                  .basePipelineIndex = 0
              }
          );
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyPipeline(Pipeline& pipeline) override {
        try {
          if (pipeline.value != nullptr) {
            functions().destroyPipeline(pipeline.device, pipeline.value, nullptr);
            pipeline.value = nullptr;
          }
          if (pipeline.layout != nullptr) {
            vkDestroyPipelineLayout(pipeline.device, pipeline.layout, nullptr);
            pipeline.layout = nullptr;
          }
          pipeline.device = nullptr;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void destroyShader(Shader& shader) override {
        try {
          if (shader.shaderModule != nullptr) {
            functions().destroyShaderModule(shader.device, shader.shaderModule, nullptr);
            shader.device = nullptr;
            shader.shaderModule = nullptr;
          }
          shader.pipelineShaderStageCreateInfo = {};
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

}
