#pragma once

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "exqudens/TestConfiguration.hpp"
#include "exqudens/test/TestUtilsTests.hpp"
#include "exqudens/test/OtherTests.hpp"
#include "exqudens/test/ConfigurationTests.hpp"
#include "exqudens/test/ShaderTests.hpp"
#include "exqudens/test/FactoryTests.hpp"
#include "exqudens/test/UiTestsA.hpp"
#include "exqudens/test/UiTestsB.hpp"
//#include "exqudens/test/UiTestsC.hpp"
//#include "exqudens/test/UiTestsD.hpp"
//#include "exqudens/test/UiTestsE.hpp"

namespace exqudens::vulkan {

  class TestApplication {

    public:

      static int run(int* argc, char** argv) {
        TestConfiguration::setExecutableFile(argv[0]);
        testing::InitGoogleMock(argc, argv);
        testing::InitGoogleTest(argc, argv);
        return RUN_ALL_TESTS();
      }

  };

}
