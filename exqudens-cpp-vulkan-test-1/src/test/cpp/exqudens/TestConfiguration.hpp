#pragma once

#include <string>
#include <optional>
#include <filesystem>

namespace exqudens::vulkan {

  class TestConfiguration {

    friend class TestApplication;

    private:

      inline static std::optional<std::string> executableFile;
      inline static std::optional<std::string> executableDir;

    public:

      static std::string getExecutableFile() {
        return executableFile.value_or("");
      }

      static std::string getExecutableDir() {
        return executableDir.value_or("");
      }

    private:

      static void setExecutableFile(const char* value) {
        std::filesystem::path path = std::filesystem::path(std::string(value)).make_preferred();
        TestConfiguration::executableFile = path.string();
        TestConfiguration::executableDir = path.parent_path().string();
      }

  };

}
