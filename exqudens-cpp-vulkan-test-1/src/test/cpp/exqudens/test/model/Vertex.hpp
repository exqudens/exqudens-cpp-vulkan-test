#pragma once

#include <vector>
#include <cstddef>

#include <glm/glm.hpp>
#include <vulkan/vulkan.h>

namespace exqudens::vulkan {

  struct Vertex {

    static VkVertexInputBindingDescription getBindingDescription() {
      return {
        .binding = 0,
        .stride = sizeof(Vertex),
        .inputRate = VK_VERTEX_INPUT_RATE_VERTEX
      };
    }

    static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
      uint32_t posOffset = offsetof(Vertex, pos);
      uint32_t colorOffset = offsetof(Vertex, color);
      uint32_t texCoordOffset = offsetof(Vertex, texCoord);

      return {
          VkVertexInputAttributeDescription {
              .location = 0,
              .binding = 0,
              .format = VK_FORMAT_R32G32B32_SFLOAT,
              .offset = posOffset
          },
          VkVertexInputAttributeDescription {
              .location = 1,
              .binding = 0,
              .format = VK_FORMAT_R32G32B32_SFLOAT,
              .offset = colorOffset
          },
          VkVertexInputAttributeDescription {
              .location = 2,
              .binding = 0,
              .format = VK_FORMAT_R32G32_SFLOAT,
              .offset = texCoordOffset
          }
      };
    }

    glm::vec3 pos;
    glm::vec3 color;
    glm::vec2 texCoord;

  };

}
