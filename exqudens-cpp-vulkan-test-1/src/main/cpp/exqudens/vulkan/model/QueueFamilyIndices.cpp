#include "exqudens/vulkan/model/QueueFamilyIndices.hpp"

namespace exqudens::vulkan::model {

  QueueFamilyIndices::QueueFamilyIndices(
      std::optional<uint32_t> graphicsFamily,
      std::optional<uint32_t> presentFamily
  ):
      graphicsFamily(graphicsFamily),
      presentFamily(presentFamily)
  {
  }

  QueueFamilyIndices::QueueFamilyIndices(
  ):
      QueueFamilyIndices(
          std::nullopt,
          std::nullopt
      )
  {
  }

  void QueueFamilyIndices::setGraphicsFamily(std::optional<uint32_t> value) {
    graphicsFamily = value;
  }

  void QueueFamilyIndices::setGraphicsFamily(uint32_t value) {
    graphicsFamily.emplace(value);
  }

  void QueueFamilyIndices::setPresentFamily(std::optional<uint32_t> value) {
    presentFamily = value;
  }

  void QueueFamilyIndices::setPresentFamily(uint32_t value) {
    presentFamily.emplace(value);
  }

  std::optional<uint32_t>& QueueFamilyIndices::getGraphicsFamily() {
    return graphicsFamily;
  }

  std::optional<uint32_t>& QueueFamilyIndices::getPresentFamily() {
    return presentFamily;
  }

  bool QueueFamilyIndices::isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
}
