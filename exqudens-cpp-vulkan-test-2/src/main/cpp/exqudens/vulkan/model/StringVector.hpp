#pragma once

#include <string>
#include <vector>
#include <iterator>
#include <algorithm>

namespace exqudens::vulkan::model {

  struct StringVector {

    std::vector<std::string> values;
    std::vector<const char*> pointers;

    static std::vector<const char*> toPointers(const std::vector<std::string>& values) {
      std::vector<const char*> result;
      std::transform(
        values.begin(),
        values.end(),
        std::back_inserter(result),
        [](const std::string& s) -> const char* { return s.c_str(); }
      );
      return result;
    }

    StringVector() = default;

    explicit StringVector(
      const std::vector<std::string>& values
    ):
      values(values),
      pointers(toPointers(values))
    {
    }

    StringVector& add(const std::string& value) {
      size_t index = values.size();
      values.push_back(value);
      const std::string& storedValue = values.at(index);
      pointers.emplace_back(storedValue.c_str());
      return *this;
    }

  };

}
