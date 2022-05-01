#pragma once

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <format>

#include <gtest/gtest.h>

#include "exqudens/TestUtils.hpp"
#include "exqudens/vulkan/raii/Utility.hpp"

namespace exqudens::vulkan {

  class MyClassA {

    private:

      inline static int currentId = 1;
      int id = -1;

    public:

      MyClassA() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
        id = currentId;
        currentId++;
      }

      int getId() {
        return id;
      }

      ~MyClassA() {
        std::cout << std::format("{}", CALL_INFO()) << std::endl;
      }

  };

  class OtherTests : public testing::Test {

    protected:

      void SetUp() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

      void TearDown() override {
        try {
          std::cout << std::format("{}", CALL_INFO()) << std::endl;
        } catch (...) {
          std::throw_with_nested(std::runtime_error(CALL_INFO()));
        }
      }

  };

  TEST_F(OtherTests, test1) {
    try {
      std::shared_ptr<MyClassA> obj1(new MyClassA);
      std::shared_ptr<MyClassA> obj2 = obj1;

      ASSERT_EQ(1, obj1->getId());
      ASSERT_EQ(1, obj2->getId());
    } catch (const std::exception& e) {
      FAIL() << TestUtils::toString(e);
    }
  }

}
