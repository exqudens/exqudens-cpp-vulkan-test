cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
get_filename_component(RAW_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" NAME "${RAW_NAME}")
project("${NAME}" VERSION "${CMAKE_PROJECT_VERSION}" LANGUAGES "C" "CXX")

message("project: '${PROJECT_NAME}-${PROJECT_VERSION}'")

set_conan_settings(CONAN_SETTINGS
    "${CMAKE_SYSTEM_NAME}"
    "${CXX_TARGET_ARCH}"
    "${CMAKE_CXX_COMPILER_ID}"
    "${CMAKE_CXX_COMPILER_VERSION}"
    "${CMAKE_MSVC_RUNTIME_LIBRARY}"
    "${CMAKE_CXX_STANDARD}"
    "${CMAKE_BUILD_TYPE}"
    "vulkan:build_type=Release"
    "vulkan:compiler.runtime=${CONAN_COMPILER_RUNTIME}"
    "glfw:build_type=Release"
    "glfw:compiler.runtime=${CONAN_COMPILER_RUNTIME}"
)

set_conan_options(CONAN_OPTIONS
    "shared=${CONAN_BUILD_SHARED_LIBS}"
)

set("CONAN_INSTALL_PREFIX" "${PROJECT_BINARY_DIR}/conan-install")

if(NOT EXISTS "${CONAN_INSTALL_PREFIX}")
    execute_process(
        COMMAND "${CONAN_COMMAND}"
                install
                "${PROJECT_SOURCE_DIR}/conanfile.py"
                --install-folder
                "${CONAN_INSTALL_PREFIX}"
                ${CONAN_SETTINGS}
                ${CONAN_OPTIONS}
        COMMAND_ECHO "STDERR"
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ENCODING "UTF-8"
        COMMAND_ERROR_IS_FATAL ANY
    )
endif()

if(NOT EXISTS "${CONAN_INSTALL_PREFIX}/conan-packages.cmake")
    message(FATAL_ERROR "Not exists: '${CONAN_INSTALL_PREFIX}/conan-packages.cmake'!")
endif()

include("${CONAN_INSTALL_PREFIX}/conan-packages.cmake")

set(CMAKE_MODULE_PATH "${CONAN_INSTALL_PREFIX}")

foreach(
    conanPackageName
    cmakePackageName
    cmakePackageVersion
    cmakePackagePath
    IN ZIP_LISTS
    "${PROJECT_NAME}_CONAN_PACKAGE_NAMES"
    "${PROJECT_NAME}_CMAKE_PACKAGE_NAMES"
    "${PROJECT_NAME}_CMAKE_PACKAGE_VERSIONS"
    "${PROJECT_NAME}_CMAKE_PACKAGE_PATHS"
)
    if(
        "" STREQUAL "${conanPackageName}"
        OR "" STREQUAL "${cmakePackageName}"
        OR "" STREQUAL "${cmakePackageVersion}"
        OR "" STREQUAL "${cmakePackagePath}"
    )
        string(JOIN " " errorMessage
            "Empty value 'conanPackageName': '${conanPackageName}'"
            "or 'cmakePackageName': '${cmakePackageName}'"
            "or 'cmakePackageVersion': '${cmakePackageVersion}'"
            "or 'cmakePackagePath': '${cmakePackagePath}'!"
            )
        message(FATAL_ERROR "${errorMessage}")
    elseif(NOT IS_DIRECTORY "${cmakePackagePath}")
        message(FATAL_ERROR "Not is directory 'cmakePackagePath': '${cmakePackagePath}'!")
    endif()

    list(APPEND CMAKE_PREFIX_PATH "${cmakePackagePath}")

    if("vulkan" STREQUAL "${cmakePackageName}")
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
        find_program(GLSLC_COMMAND NAMES "glslc.exe" "glslc" PATHS "${cmakePackagePath}/bin" NO_CACHE)
    elseif("glm" STREQUAL "${conanPackageName}")
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
    elseif("lodepng" STREQUAL "${conanPackageName}")
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" EXACT QUIET)
    elseif("gtest" STREQUAL "${conanPackageName}")
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
    elseif("glfw" STREQUAL "${conanPackageName}")
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
    else()
        list(APPEND NOT_FOUND_PACKAGE_NAMES "${cmakePackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET CONFIG)
    endif()

endforeach()

set_not_found_package_names(NOT_FOUND_PACKAGE_NAMES ${NOT_FOUND_PACKAGE_NAMES})

if(NOT "" STREQUAL "${NOT_FOUND_PACKAGE_NAMES}")
    foreach(package ${NOT_FOUND_PACKAGE_NAMES})
        message(WARNING "${package}_FOUND: '${${package}_FOUND}'")
    endforeach()
    return()
endif()

add_custom_command(
    OUTPUT "${PROJECT_BINARY_DIR}/main/bin/resources/shaders/shader.vert.spv"
           "${PROJECT_BINARY_DIR}/main/bin/resources/shaders/shader.frag.spv"
    COMMAND "${CMAKE_COMMAND}" "-E" "rm" "-rf" "${PROJECT_BINARY_DIR}/main/bin/resources/shaders"
    COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${PROJECT_BINARY_DIR}/main/bin/resources/shaders"
    COMMAND "${GLSLC_COMMAND}" "${PROJECT_SOURCE_DIR}/src/main/resources/shaders/shader.vert" -o "${PROJECT_BINARY_DIR}/main/bin/resources/shaders/shader.vert.spv"
    COMMAND "${GLSLC_COMMAND}" "${PROJECT_SOURCE_DIR}/src/main/resources/shaders/shader.frag" -o "${PROJECT_BINARY_DIR}/main/bin/resources/shaders/shader.frag.spv"
    VERBATIM
)
add_library("${PROJECT_NAME}"
    "${PROJECT_BINARY_DIR}/main/bin/resources/shaders/shader.vert.spv"
    "${PROJECT_BINARY_DIR}/main/bin/resources/shaders/shader.frag.spv"

    "src/main/cpp/exqudens/vulkan/Macros.hpp"

    "src/main/cpp/exqudens/vulkan/model/StringVector.hpp"
    "src/main/cpp/exqudens/vulkan/model/QueueFamilyIndices.hpp"
    "src/main/cpp/exqudens/vulkan/model/SwapChainSupportDetails.hpp"
    "src/main/cpp/exqudens/vulkan/model/Vertex.hpp"
    "src/main/cpp/exqudens/vulkan/model/UniformBufferObject.hpp"

    "src/main/cpp/exqudens/vulkan/Utils.hpp"
    "src/main/cpp/exqudens/vulkan/Utils.cpp"
    "src/main/cpp/exqudens/vulkan/Logger.hpp"
    "src/main/cpp/exqudens/vulkan/Logger.cpp"
)
target_include_directories("${PROJECT_NAME}" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries("${PROJECT_NAME}" PUBLIC
    "vulkan::vulkan"
    "glm::glm"
    "lodepng::lodepng"
)
set_target_properties("${PROJECT_NAME}" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/lib"
)

add_library("${PROJECT_NAME}-test-lib" INTERFACE
    "src/test/cpp/exqudens/glfw/Application.hpp"

    "src/test/cpp/exqudens/vulkan/TestApplication.hpp"
    "src/test/cpp/exqudens/vulkan/TestConfiguration.hpp"
    "src/test/cpp/exqudens/vulkan/UtilsTests.hpp"
    "src/test/cpp/exqudens/vulkan/ApplicationTests.hpp"
)
target_include_directories("${PROJECT_NAME}-test-lib" INTERFACE
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/test/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries("${PROJECT_NAME}-test-lib" INTERFACE
    "${PROJECT_NAME}"
    "GTest::gtest"
    "GTest::gtest_main"
    "glfw::glfw"
)
set_target_properties("${PROJECT_NAME}-test-lib" PROPERTIES
    CXX_STANDARD 23
)

add_executable("${PROJECT_NAME}-test-app" "src/test/cpp/main.cpp")
target_link_libraries("${PROJECT_NAME}-test-app" PRIVATE
    "${PROJECT_NAME}-test-lib"
)
set_target_properties("${PROJECT_NAME}-test-app" PROPERTIES
    CXX_STANDARD 23

    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/test/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/test/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/test/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/test/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/test/lib"
)
add_custom_command(TARGET "${PROJECT_NAME}-test-app"
    PRE_BUILD
    COMMAND "${CMAKE_COMMAND}" -E rm -f "$<TARGET_PROPERTY:${PROJECT_NAME}-test-app,RUNTIME_OUTPUT_DIRECTORY>/*"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:${PROJECT_NAME},RUNTIME_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:${PROJECT_NAME}-test-app,RUNTIME_OUTPUT_DIRECTORY>"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CONAN_INSTALL_PREFIX}/bin" "$<TARGET_PROPERTY:${PROJECT_NAME}-test-app,RUNTIME_OUTPUT_DIRECTORY>"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PROJECT_SOURCE_DIR}/src/main/resources" "$<TARGET_PROPERTY:${PROJECT_NAME}-test-app,RUNTIME_OUTPUT_DIRECTORY>/resources"
    VERBATIM
)
