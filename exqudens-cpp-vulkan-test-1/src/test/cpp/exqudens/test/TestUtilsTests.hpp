#pragma once

#include <string>
#include <sstream>
#include <filesystem>
#include <stdexcept>

#include <gtest/gtest.h>

#include "exqudens/TestMacros.hpp"
#include "exqudens/TestConfiguration.hpp"
#include "exqudens/TestUtils.hpp"

namespace exqudens::vulkan {

  class TestUtilsTests : public testing::Test {

    protected:

      void func3() {
        try {
          throw std::invalid_argument(CALL_INFO() + ": Test error message!");
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void func2() {
        try {
          func3();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void func1() {
        try {
          func2();
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  TEST_F(TestUtilsTests, test1) {
    try {
      RecordProperty("TestUtilsTests.test1.info1", "toString(const std::exception& e)");
      std::string thisFilePath = std::filesystem::path(__FILE__).make_preferred().string();
      std::ostringstream out;
      out << "exqudens::vulkan::TestUtilsTests::func3(" + thisFilePath + ":22): Test error message!" << std::endl;
      out << "exqudens::vulkan::TestUtilsTests::func3(" + thisFilePath + ":24)" << std::endl;
      out << "exqudens::vulkan::TestUtilsTests::func2(" + thisFilePath + ":32)" << std::endl;
      out << "exqudens::vulkan::TestUtilsTests::func1(" + thisFilePath + ":40)";
      std::string expected = out.str();
      try {
        func1();
        FAIL() << "(const std::exception& e) not thrown!";
      } catch (const std::exception& e) {
        std::string result = TestUtils::toString(e);
        ASSERT_EQ(expected, result);
      }
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

  TEST_F(TestUtilsTests, test2) {
    try {
      RecordProperty("TestUtilsTests.test2.info1", "readPng(const std::string& path)");
      RecordProperty(
          "TestUtilsTests.test2.info2",
          "writePng(const std::vector<std::vector<std::vector<unsigned char>>>& image, const std::string& path)"
      );
      std::cout << std::format("executableFile: '{}'", TestConfiguration::getExecutableFile()) << std::endl;
      std::cout << std::format("executableDir: '{}'", TestConfiguration::getExecutableDir()) << std::endl;
      std::vector<std::vector<std::vector<unsigned char>>> image1 = TestUtils::readPng(
          std::filesystem::path(TestConfiguration::getExecutableDir())
              .append("resources")
              .append("png")
              .append("texture.png")
              .make_preferred()
              .string()
      );
      ASSERT_EQ(480, image1.size());
      ASSERT_EQ(640, image1[0].size());
      ASSERT_EQ(4, image1[0][0].size());
      std::cout << "image1.size OK!" << std::endl;
      TestUtils::writePng(
          image1,
          std::filesystem::path(TestConfiguration::getExecutableDir())
              .append("resources")
              .append("png")
              .append("texture1.png")
              .make_preferred()
              .string()
      );
      std::vector<std::vector<std::vector<unsigned char>>> image2 = TestUtils::readPng(
          std::filesystem::path(TestConfiguration::getExecutableDir())
              .append("resources")
              .append("png")
              .append("texture1.png")
              .make_preferred()
              .string()
      );
      ASSERT_EQ(480, image2.size());
      ASSERT_EQ(640, image2[0].size());
      ASSERT_EQ(4, image2[0][0].size());
      std::cout << "image2.size OK!" << std::endl;
      ASSERT_EQ(image1, image2);
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
