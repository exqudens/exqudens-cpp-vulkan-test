#pragma once

#include "exqudens/vulkan/Utility.hpp"
#include "exqudens/vulkan/model/Shader.hpp"
#include "exqudens/vulkan/model/PipelineLayoutCreateInfo.hpp"
#include "exqudens/vulkan/model/GraphicsPipelineCreateInfo.hpp"
#include "exqudens/vulkan/model/Pipeline.hpp"

namespace exqudens::vulkan {

  class PipelineFactory {

    public:

      virtual Shader createShader(VkDevice& device, const std::string& path) = 0;

      virtual Pipeline createPipeline(
          VkDevice& device,
          const std::vector<std::string>& shaderPaths,
          const std::vector<VkVertexInputBindingDescription>& vertexBindingDescriptions,
          const std::vector<VkVertexInputAttributeDescription>& vertexAttributeDescriptions,
          const PipelineLayoutCreateInfo& layoutCreateInfo,
          const GraphicsPipelineCreateInfo& createInfo
      ) = 0;

      virtual Pipeline createGraphicsPipeline(
          VkDevice& device,
          const VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass
      ) = 0;
      virtual Pipeline createGraphicsPipeline(
          VkDevice& device,
          const VkExtent2D& extent,
          const std::vector<std::string>& shaderPaths,
          VkRenderPass& renderPass,
          VkFrontFace frontFace,
          std::vector<VkDescriptorSetLayout> descriptorSetLayouts,
          std::vector<VkVertexInputBindingDescription> vertexBindingDescriptions,
          std::vector<VkVertexInputAttributeDescription> vertexAttributeDescriptions
      ) = 0;

      virtual void destroyShader(Shader& shader) = 0;

      virtual void destroyPipeline(Pipeline& pipeline) = 0;

  };

}
