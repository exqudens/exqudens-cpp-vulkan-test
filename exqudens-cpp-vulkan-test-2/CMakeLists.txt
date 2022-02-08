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
    "vulkan:compiler.runtime=MD"
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

    if("vulkan" STREQUAL "${conanPackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
        find_program(GLSLC_COMMAND NAMES "glslc.exe" "glslc" PATHS "${cmakePackagePath}/bin")
    elseif("glm" STREQUAL "${conanPackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
    elseif("lodepng" STREQUAL "${conanPackageName}")
        find_package("${cmakePackageName}" QUIET)
    elseif("glfw" STREQUAL "${conanPackageName}")
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
    else()
        find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET CONFIG)
    endif()

endforeach()

set_not_found_package_names(NOT_FOUND_PACKAGE_NAMES ${${PROJECT_NAME}_CMAKE_PACKAGE_NAMES})

if(NOT "" STREQUAL "${NOT_FOUND_PACKAGE_NAMES}")
    foreach(package ${NOT_FOUND_PACKAGE_NAMES})
        message(WARNING "${package}_FOUND: '${${package}_FOUND}'")
    endforeach()
    return()
endif()

add_custom_command(
    OUTPUT "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders/shader.vert.spv"
           "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders/shader.frag.spv"
    COMMAND "${CMAKE_COMMAND}" "-E" "rm" "-rf" "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders"
    COMMAND "${CMAKE_COMMAND}" "-E" "make_directory" "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders"
    COMMAND "${GLSLC_COMMAND}" "${PROJECT_SOURCE_DIR}/src/main/glsl/exqudens/vulkan/shader.vert" -o "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders/shader.vert.spv"
    COMMAND "${GLSLC_COMMAND}" "${PROJECT_SOURCE_DIR}/src/main/glsl/exqudens/vulkan/shader.frag" -o "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders/shader.frag.spv"
    VERBATIM
)
add_library("${PROJECT_NAME}"
    "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders/shader.vert.spv"
    "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin/shaders/shader.frag.spv"

    "src/main/cpp/exqudens/vulkan/model/StringVector.hpp"
    "src/main/cpp/exqudens/vulkan/model/QueueFamilyIndices.hpp"
    "src/main/cpp/exqudens/vulkan/model/SwapChainSupportDetails.hpp"
    "src/main/cpp/exqudens/vulkan/model/Vertex.hpp"
    "src/main/cpp/exqudens/vulkan/model/UniformBufferObject.hpp"

    "src/main/cpp/exqudens/vulkan/Environment.hpp"
    "src/main/cpp/exqudens/vulkan/Environment.cpp"
    "src/main/cpp/exqudens/vulkan/Logger.hpp"
    "src/main/cpp/exqudens/vulkan/Logger.cpp"
)
target_include_directories("${PROJECT_NAME}" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries("${PROJECT_NAME}" PUBLIC
    "Vulkan::Vulkan"
    "glm::glm"
    "lodepng::lodepng"
)
set_target_properties("${PROJECT_NAME}" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}/lib"
)

add_library("${PROJECT_NAME}-glfw"
    "src/main/cpp/exqudens/glfw/Application.hpp"
    "src/main/cpp/exqudens/glfw/Application.cpp"
)
target_include_directories("${PROJECT_NAME}-glfw" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries("${PROJECT_NAME}-glfw" PUBLIC
    "${PROJECT_NAME}"
    "glfw::glfw"
)
set_target_properties("${PROJECT_NAME}-glfw" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-glfw/lib"
)

add_executable("${PROJECT_NAME}-app" "src/main/cpp/main.cpp")
target_link_libraries("${PROJECT_NAME}-app" PUBLIC
    "${PROJECT_NAME}-glfw"
)
set_target_properties("${PROJECT_NAME}-app" PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/bin"
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/bin"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/bin"

    ARCHIVE_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"

    LIBRARY_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELEASE        "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL     "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
    LIBRARY_OUTPUT_DIRECTORY_DEBUG          "${PROJECT_BINARY_DIR}/main/${PROJECT_NAME}-app/lib"
)
if("${BUILD_SHARED_LIBS}")
    add_custom_command(TARGET "${PROJECT_NAME}-app"
        PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E rm -f "$<TARGET_PROPERTY:${PROJECT_NAME}-app,RUNTIME_OUTPUT_DIRECTORY>/*"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:${PROJECT_NAME},RUNTIME_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:${PROJECT_NAME}-app,RUNTIME_OUTPUT_DIRECTORY>"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "$<TARGET_PROPERTY:${PROJECT_NAME}-glfw,RUNTIME_OUTPUT_DIRECTORY>" "$<TARGET_PROPERTY:${PROJECT_NAME}-app,RUNTIME_OUTPUT_DIRECTORY>"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CONAN_INSTALL_PREFIX}/bin" "$<TARGET_PROPERTY:${PROJECT_NAME}-app,RUNTIME_OUTPUT_DIRECTORY>"
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${PROJECT_SOURCE_DIR}/src/main/resources" "$<TARGET_PROPERTY:${PROJECT_NAME}-app,RUNTIME_OUTPUT_DIRECTORY>/resources"
        VERBATIM
    )
endif()
