#pragma once

#include <cstdlib>

#include <gtest/gtest.h>

#include "exqudens/glfw/Application.hpp"

namespace exqudens::vulkan {

  TEST(ApplicationTests, test1) {
    std::vector<std::string> arguments = { "C:\\aaa\\bbb\\test-app.exe" };
    std::vector<char*> cArguments;
    cArguments.reserve(arguments.size());
    for (size_t i = 0; i < arguments.size(); i++) {
      cArguments.push_back(const_cast<char*>(arguments[i].c_str()));
    }
    exqudens::glfw::Application app(cArguments.size(), &cArguments[0]);
    int result = app.run();
    ASSERT_EQ(EXIT_SUCCESS, result);
  }

}
