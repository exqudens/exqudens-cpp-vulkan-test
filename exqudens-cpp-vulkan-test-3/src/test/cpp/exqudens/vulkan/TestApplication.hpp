#pragma once

#include <gtest/gtest.h>

#include "exqudens/vulkan/UtilsTests.hpp"
#include "exqudens/vulkan/ApplicationTests.hpp"

namespace exqudens::vulkan {

  class TestApplication {

    public:

      static int run(int* argc, char** argv) {
        testing::InitGoogleTest(argc, argv);
        return RUN_ALL_TESTS();
      }

  };

}
