#pragma once

#include <optional>

namespace exqudens::vulkan::model {

  class QueueFamilyIndices {

    private:

      std::optional<uint32_t> graphicsFamily;
      std::optional<uint32_t> presentFamily;

    public:

      QueueFamilyIndices(
          std::optional<uint32_t> graphicsFamily,
          std::optional<uint32_t> presentFamily
      );

      QueueFamilyIndices();

      void setGraphicsFamily(std::optional<uint32_t> value);
      void setGraphicsFamily(uint32_t value);

      void setPresentFamily(std::optional<uint32_t> value);
      void setPresentFamily(uint32_t value);

      std::optional<uint32_t>& getGraphicsFamily();

      std::optional<uint32_t>& getPresentFamily();

      bool isComplete();

      /*bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
      }*/

  };

}
