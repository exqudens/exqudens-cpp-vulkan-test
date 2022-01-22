cmake_minimum_required(VERSION 3.21 FATAL_ERROR)
get_filename_component(RAW_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
string(REPLACE " " "_" NAME "${RAW_NAME}")
project("${NAME}" VERSION "${CMAKE_PROJECT_VERSION}" LANGUAGES "C" "CXX")

message("project: '${PROJECT_NAME}-${PROJECT_VERSION}'")

if(NOT "" STREQUAL "${CONAN_COMMAND}")
    set_conan_settings(CONAN_SETTINGS
        "${CMAKE_SYSTEM_NAME}"
        "${CXX_TARGET_ARCH}"
        "${CMAKE_CXX_COMPILER_ID}"
        "${CMAKE_CXX_COMPILER_VERSION}"
        "${CMAKE_MSVC_RUNTIME_LIBRARY}"
        "${CMAKE_CXX_STANDARD}"
        "${CMAKE_BUILD_TYPE}"
        "glm:build_type=Release"
        "vulkan:build_type=Release"
        "vulkan:compiler.runtime=${CONAN_RELEASE_COMPILER_RUNTIME}"
        "glfw:build_type=Release"
        "glfw:compiler.runtime=${CONAN_RELEASE_COMPILER_RUNTIME}"
    )

    set_conan_options(CONAN_OPTIONS
        "glm:type=interface"
        "vulkan:type=${CONAN_PACKAGE_TYPE}"
        "glfw:type=${CONAN_PACKAGE_TYPE}"
        "type=${CONAN_PACKAGE_TYPE}"
    )

    set("${PROJECT_NAME}_CONAN_INSTALL_PREFIX" "${PROJECT_BINARY_DIR}/conan-install")

    if(NOT EXISTS "${${PROJECT_NAME}_CONAN_INSTALL_PREFIX}")
        execute_process(
            COMMAND "${CONAN_COMMAND}"
            install
            "${PROJECT_SOURCE_DIR}/conanfile.py"
            --install-folder
            "${${PROJECT_NAME}_CONAN_INSTALL_PREFIX}"
            ${CONAN_SETTINGS}
            ${CONAN_OPTIONS}
            COMMAND_ECHO "STDERR"
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ENCODING "UTF-8"
            COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    if(NOT EXISTS "${${PROJECT_NAME}_CONAN_INSTALL_PREFIX}/conan-packages.cmake")
        message(FATAL_ERROR "Not exists: '${${PROJECT_NAME}_CONAN_INSTALL_PREFIX}/conan-packages.cmake'!")
    endif()

    include("${${PROJECT_NAME}_CONAN_INSTALL_PREFIX}/conan-packages.cmake")

    set(CMAKE_MODULE_PATH "${${PROJECT_NAME}_CONAN_INSTALL_PREFIX}")

    foreach(
        conanPackageName
        cmakePackageName
        cmakePackageVersion
        cmakePackageFindType
        cmakePackagePaths
        IN ZIP_LISTS
        "${PROJECT_NAME}_CONAN_PACKAGE_NAMES"
        "${PROJECT_NAME}_CMAKE_PACKAGE_NAMES"
        "${PROJECT_NAME}_CMAKE_PACKAGE_VERSIONS"
        "${PROJECT_NAME}_CMAKE_PACKAGE_FIND_TYPES"
        "${PROJECT_NAME}_CMAKE_PACKAGE_PATHS"
    )
        if(
            "" STREQUAL "${conanPackageName}"
            OR "" STREQUAL "${cmakePackageName}"
            OR "" STREQUAL "${cmakePackageVersion}"
            OR "" STREQUAL "${cmakePackageFindType}"
            OR "" STREQUAL "${cmakePackagePaths}"
        )
            string(JOIN " " errorMessage
                "Empty value 'conanPackageName': '${conanPackageName}'"
                "or 'cmakePackageName': '${cmakePackageName}'"
                "or 'cmakePackageVersion': '${cmakePackageVersion}'"
                "or 'cmakePackageFindType': '${cmakePackageFindType}'"
                "or 'cmakePackagePaths': '${cmakePackagePaths}'!"
            )
            message(FATAL_ERROR "${errorMessage}")
        endif()

        string(REPLACE "<sep>" ";" tmpCmakePackagePaths "${cmakePackagePaths}")

        foreach(cmakePackagePath ${tmpCmakePackagePaths})
            if(NOT IS_DIRECTORY "${cmakePackagePath}")
                message(FATAL_ERROR "Not is directory 'cmakePackagePath': '${cmakePackagePath}'!")
            endif()
            list(APPEND CMAKE_PREFIX_PATH "${cmakePackagePath}")
        endforeach()

        if("<ignore>" STREQUAL "${cmakePackageVersion}" AND "MODULE" STREQUAL "${cmakePackageFindType}")
            find_package("${cmakePackageName}" QUIET)
        elseif(NOT "<ignore>" STREQUAL "${cmakePackageVersion}" AND "MODULE" STREQUAL "${cmakePackageFindType}")
            find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET)
        elseif("<ignore>" STREQUAL "${cmakePackageVersion}" AND NOT "MODULE" STREQUAL "${cmakePackageFindType}")
            find_package("${cmakePackageName}" QUIET CONFIG)
        elseif(NOT "<ignore>" STREQUAL "${cmakePackageVersion}" AND NOT "MODULE" STREQUAL "${cmakePackageFindType}")
            find_package("${cmakePackageName}" "${cmakePackageVersion}" EXACT QUIET CONFIG)
        endif()

        if("vulkan" STREQUAL "${cmakePackageName}")
            find_program(GLSLC_COMMAND NAMES "glslc.exe" "glslc" PATHS "${cmakePackagePath}/bin" NO_CACHE)
        endif()

    endforeach()

    set_not_found_package_names(NOT_FOUND_PACKAGE_NAMES ${${PROJECT_NAME}_CMAKE_PACKAGE_NAMES})

    if(NOT "" STREQUAL "${NOT_FOUND_PACKAGE_NAMES}")
        foreach(package ${NOT_FOUND_PACKAGE_NAMES})
            message(WARNING "${package}_FOUND: '${${package}_FOUND}'")
        endforeach()
        return()
    endif()

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

    "src/main/cpp/exqudens/vulkan/model/Configuration.hpp"
    "src/main/cpp/exqudens/vulkan/model/Configuration.cpp"
    "src/main/cpp/exqudens/vulkan/model/QueueFamilyIndices.hpp"
    "src/main/cpp/exqudens/vulkan/model/QueueFamilyIndices.cpp"
    "src/main/cpp/exqudens/vulkan/model/SwapChainSupportDetails.hpp"
    "src/main/cpp/exqudens/vulkan/model/SwapChainSupportDetails.cpp"
    "src/main/cpp/exqudens/vulkan/model/Vertex.hpp"

    "src/main/cpp/exqudens/vulkan/Logger.hpp"
    "src/main/cpp/exqudens/vulkan/Logger.cpp"
    "src/main/cpp/exqudens/vulkan/Initializer.hpp"
    "src/main/cpp/exqudens/vulkan/Initializer.cpp"
    "src/main/cpp/exqudens/vulkan/Environment.hpp"
    "src/main/cpp/exqudens/vulkan/Environment.cpp"
)
target_include_directories("${PROJECT_NAME}" PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/main/cpp>"
    "$<INSTALL_INTERFACE:include>"
)
target_link_libraries("${PROJECT_NAME}" PUBLIC
    "vulkan::vulkan"
    "glm::glm"
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
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${${PROJECT_NAME}_CONAN_INSTALL_PREFIX}/bin" "$<TARGET_PROPERTY:${PROJECT_NAME}-app,RUNTIME_OUTPUT_DIRECTORY>"
        VERBATIM
    )
endif()
