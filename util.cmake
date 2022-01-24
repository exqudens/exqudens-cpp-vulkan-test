function(set_if_not_defined var)
    if("" STREQUAL "${${var}}" AND NOT "" STREQUAL "${ARGN}")
        set("${var}" "${ARGN}" PARENT_SCOPE)
    endif()
endfunction()

function(string_substring_from var input fromExclusive)
    if("" STREQUAL "${fromExclusive}")
        message(FATAL_ERROR "Empty value not supported for 'fromExclusive'.")
    endif()
    string(FIND "${input}" "${fromExclusive}" fromStartIndex)
    if("-1" STREQUAL "${fromStartIndex}")
        message(FATAL_ERROR "Can't find 'fromExclusive' in 'input'")
    endif()
    string(LENGTH "${input}" inputLength)
    string(LENGTH "${fromExclusive}" "fromLength")
    math(EXPR fromEndIndex "${fromStartIndex} + ${fromLength}")
    math(EXPR substringLength "${inputLength} - ${fromEndIndex}")
    string(SUBSTRING "${input}" "${fromEndIndex}" "${substringLength}" result)
    set("${var}" "${result}" PARENT_SCOPE)
endfunction()

function(string_substring_to var input toExclusive)
    if("" STREQUAL "${toExclusive}")
        message(FATAL_ERROR "Empty value not supported for 'toExclusive'.")
    endif()
    string(FIND "${input}" "${toExclusive}" toStartIndex)
    if("-1" STREQUAL "${toStartIndex}")
        message(FATAL_ERROR "Can't find 'toExclusive' in 'input'")
    endif()
    string(LENGTH "${input}" inputLength)
    string(SUBSTRING "${input}" 0 "${toStartIndex}" result)
    set("${var}" "${result}" PARENT_SCOPE)
endfunction()

function(string_substring_between var input fromExclusive toExclusive)
    string_substring_from(result "${input}" "${fromExclusive}")
    string_substring_to(result "${result}" "${toExclusive}")
    set("${var}" "${result}" PARENT_SCOPE)
endfunction()

function(set_msvc_path var vswhereCommand compilerVersion)
    foreach(name var compilerVersion)
        if("" STREQUAL "${${name}}")
            message(FATAL_ERROR "Empty value not supported for '${name}'.")
        endif()
    endforeach()

    if(NOT "" STREQUAL "${vswhereCommand}")
        cmake_path(CONVERT "${vswhereCommand}" TO_CMAKE_PATH_LIST vswhereCommand NORMALIZE)
    else()
        set(vswhereCommand "C:/Program Files (x86)/Microsoft Visual Studio/Installer/vswhere.exe")
    endif()

    if("17" STREQUAL "${compilerVersion}" OR "2022" STREQUAL "${compilerVersion}")
        set(vswhereVersionArgs "-version" "[17.0, 18.0)")
    elseif("16" STREQUAL "${compilerVersion}" OR "2019" STREQUAL "${compilerVersion}")
        set(vswhereVersionArgs "-version" "[16.0, 17.0)")
    elseif("15" STREQUAL "${compilerVersion}" OR "2017" STREQUAL "${compilerVersion}")
        set(vswhereVersionArgs "-version" "[15.0, 16.0)")
    else()
        #set(vswhereVersionArgs "-latest")
        string(JOIN " " errorMessage
            "Unsupported or not specified 'compilerVersion': '${compilerVersion}'."
            "Supported values ['15', '16', '2017', '2019']."
        )
        message(FATAL_ERROR "${errorMessage}")
    endif()

    execute_process(
        COMMAND "${vswhereCommand}" ${vswhereVersionArgs} "-property" "installationPath"
        OUTPUT_VARIABLE "result"
        COMMAND_ECHO "STDERR"
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ENCODING "UTF-8"
        COMMAND_ERROR_IS_FATAL ANY
    )

    if("" STREQUAL "${result}")
        execute_process(
            COMMAND "${vswhereCommand}" ${vswhereVersionArgs} "-products" "Microsoft.VisualStudio.Product.BuildTools" "-property" "installationPath"
            OUTPUT_VARIABLE "result"
            COMMAND_ECHO "STDERR"
            OUTPUT_STRIP_TRAILING_WHITESPACE
            ENCODING "UTF-8"
            COMMAND_ERROR_IS_FATAL ANY
        )
    endif()

    if("" STREQUAL "${result}")
        message(FATAL_ERROR "Empty result from: '${vswhereCommand}'.")
    endif()

    cmake_path(CONVERT "${result}" TO_CMAKE_PATH_LIST result NORMALIZE)
    string(REGEX REPLACE "[\r]" "" result "${result}")
    string(REGEX REPLACE "[\n]" ";" result "${result}")
    list(GET result 0 result)

    set("${var}" "${result}" PARENT_SCOPE)
endfunction()

function(set_msvc_env prefix vswhereCommand compilerVersion compilerArch targetArch)
    foreach(name prefix compilerVersion compilerArch targetArch)
        if("" STREQUAL "${${name}}")
            message(FATAL_ERROR "Empty value not supported for '${name}'.")
        endif()
    endforeach()

    if(NOT "x86" STREQUAL "${compilerArch}" AND NOT "x64" STREQUAL "${compilerArch}")
        string(JOIN " " errorMessage
            "Unsupported or not specified 'compilerArch': '${compilerArch}'."
            "Supported values ['x86', 'x64']."
        )
        message(FATAL_ERROR "${errorMessage}")
    endif()

    if(NOT "x86" STREQUAL "${targetArch}" AND NOT "x64" STREQUAL "${targetArch}")
        string(JOIN " " errorMessage
            "Unsupported or not specified 'targetArch': '${targetArch}'."
            "Supported values [ 'x86', 'x64' ]."
        )
        message(FATAL_ERROR "${errorMessage}")
    endif()

    if("${compilerArch}" STREQUAL "${targetArch}")
        set(vcvarsallBatConf "${compilerArch}")
    else()
        set(vcvarsallBatConf "${compilerArch}_${targetArch}")
    endif()

    set_msvc_path(msvcPath "${vswhereCommand}" "${compilerVersion}")

    file(GLOB_RECURSE msvcFiles "${msvcPath}/*")
    set(vcvarsallBatName "vcvarsall.bat")
    foreach(file IN LISTS msvcFiles)
        get_filename_component(fileName "${file}" NAME)
        get_filename_component(path "${file}" DIRECTORY)
        if("${fileName}" STREQUAL "${vcvarsallBatName}")
            set(vcvarsallBatFile "${file}")
            set(vcvarsallBatPath "${path}")
            break()
        endif()
    endforeach()

    set(vcvarsallBatArgs
        "&&" "call" "echo" "INCLUDE_START"
        "&&" "call" "echo" "%INCLUDE%"
        "&&" "call" "echo" "INCLUDE_STOP"
        "&&" "call" "echo" "LIBPATH_START"
        "&&" "call" "echo" "%LIBPATH%"
        "&&" "call" "echo" "LIBPATH_STOP"
        "&&" "call" "echo" "LIB_START"
        "&&" "call" "echo" "%LIB%"
        "&&" "call" "echo" "LIB_STOP"
        "&&" "call" "echo" "CLPATH_START"
        "&&" "call" "where" "cl"
        "&&" "call" "echo" "CLPATH_STOP"
        "&&" "call" "echo" "RCPATH_START"
        "&&" "call" "where" "rc"
        "&&" "call" "echo" "RCPATH_STOP"
    )
    execute_process(
        COMMAND "cmd" "/c" "${vcvarsallBatName}" "${vcvarsallBatConf}" ${vcvarsallBatArgs}
        WORKING_DIRECTORY "${vcvarsallBatPath}"
        OUTPUT_VARIABLE "msvcEnv"
        COMMAND_ECHO "STDERR"
        OUTPUT_STRIP_TRAILING_WHITESPACE
        ENCODING "UTF-8"
        COMMAND_ERROR_IS_FATAL ANY
    )

    string_substring_between(msvcInclude "${msvcEnv}" "INCLUDE_START" "INCLUDE_STOP")
    string(STRIP "${msvcInclude}" msvcInclude)
    string(REGEX REPLACE "[\r]" "" msvcInclude "${msvcInclude}")
    string(REGEX REPLACE "[\n]" "" msvcInclude "${msvcInclude}")

    string_substring_between(msvcLibPath "${msvcEnv}" "LIBPATH_START" "LIBPATH_STOP")
    string(STRIP "${msvcLibPath}" msvcLibPath)
    string(REGEX REPLACE "[\r]" "" msvcLibPath "${msvcLibPath}")
    string(REGEX REPLACE "[\n]" "" msvcLibPath "${msvcLibPath}")

    string_substring_between(msvcLib "${msvcEnv}" "LIB_START" "LIB_STOP")
    string(STRIP "${msvcLib}" msvcLib)
    string(REGEX REPLACE "[\r]" "" msvcLib "${msvcLib}")
    string(REGEX REPLACE "[\n]" "" msvcLib "${msvcLib}")

    string_substring_between(msvcClPath "${msvcEnv}" "CLPATH_START" "CLPATH_STOP")
    string(STRIP "${msvcClPath}" msvcClPath)
    string(REGEX REPLACE "[\r]" "" msvcClPath "${msvcClPath}")
    string(REGEX REPLACE "[\n]" ";" msvcClPath "${msvcClPath}")
    list(GET msvcClPath 0 msvcClPath)
    get_filename_component(msvcClPath "${msvcClPath}" DIRECTORY)
    cmake_path(CONVERT "${msvcClPath}" TO_NATIVE_PATH_LIST msvcClPath NORMALIZE)

    string_substring_between(msvcRcPath "${msvcEnv}" "RCPATH_START" "RCPATH_STOP")
    string(STRIP "${msvcRcPath}" msvcRcPath)
    string(REGEX REPLACE "[\r]" "" msvcRcPath "${msvcRcPath}")
    string(REGEX REPLACE "[\n]" ";" msvcRcPath "${msvcRcPath}")
    list(GET msvcRcPath 0 msvcRcPath)
    get_filename_component(msvcRcPath "${msvcRcPath}" DIRECTORY)
    cmake_path(CONVERT "${msvcRcPath}" TO_NATIVE_PATH_LIST msvcRcPath NORMALIZE)

    set("${prefix}_MSVC_INCLUDE" "${msvcInclude}" PARENT_SCOPE)
    set("${prefix}_MSVC_LIBPATH" "${msvcLibPath}" PARENT_SCOPE)
    set("${prefix}_MSVC_LIB" "${msvcLib}" PARENT_SCOPE)
    set("${prefix}_MSVC_PATH" "${msvcClPath}" "${msvcRcPath}" PARENT_SCOPE)
endfunction()

function(set_conan_msvc_compiler_runtime var cmakeMsvcRuntimeLibrary)
    if("" STREQUAL "${var}")
        message(FATAL_ERROR "Empty value not supported for 'var'.")
    endif()

    if("${cmakeMsvcRuntimeLibrary}" STREQUAL "MultiThreaded")
        set(value "MT")
    elseif("${cmakeMsvcRuntimeLibrary}" STREQUAL "MultiThreadedDLL")
        set(value "MD")
    elseif("${cmakeMsvcRuntimeLibrary}" STREQUAL "MultiThreadedDebug")
        set(value "MTd")
    elseif("${cmakeMsvcRuntimeLibrary}" STREQUAL "MultiThreadedDebugDLL")
        set(value "MDd")
    else()
        message(FATAL_ERROR "Unsupported 'cmakeMsvcRuntimeLibrary': '${cmakeMsvcRuntimeLibrary}'")
    endif()

    set("${var}" "${value}" PARENT_SCOPE)
endfunction()

function(
    set_conan_settings
    var
    cmakeSystemName
    cxxTargetArch
    cmakeCxxCompilerId
    cmakeCxxCompilerVersion
    cmakeMsvcRuntimeLibrary
    cmakeCxxStandard
    cmakeBuildType
)
    if("" STREQUAL "${var}")
        message(FATAL_ERROR "Empty value not supported for 'var'.")
    endif()

    # os
    if("Windows" STREQUAL "${cmakeSystemName}")
        set(value "--settings" "os=${cmakeSystemName}")

        # arch
        if("x64" STREQUAL "${cxxTargetArch}" OR "AMD64" STREQUAL "${cxxTargetArch}" OR "IA64" STREQUAL "${cxxTargetArch}")
            set(value "${value}" "--settings" "arch=x86_64")
        elseif("x86" STREQUAL "${cxxTargetArch}")
            set(value "${value}" "--settings" "arch=x86")
        else()
            message(FATAL_ERROR "Unsupported 'cxxTargetArch': '${cxxTargetArch}'")
        endif()
    else()
        message(FATAL_ERROR "Unsupported 'cmakeSystemName': '${cmakeSystemName}'")
    endif()

    # compiler
    if("MSVC" STREQUAL "${cmakeCxxCompilerId}")
        set(value "${value}" "--settings" "compiler=Visual Studio")

        # compiler.version
        if("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "19.30" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "19.40")
            set(value "${value}" "--settings" "compiler.version=17")
        elseif("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "19.20" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "19.30")
            set(value "${value}" "--settings" "compiler.version=16")
        elseif("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "19.10" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "19.20")
            set(value "${value}" "--settings" "compiler.version=15")
        else()
            message(FATAL_ERROR "Unsupported 'cmakeCxxCompilerVersion': '${cmakeCxxCompilerVersion}'")
        endif()

        # compiler.runtime
        set_conan_msvc_compiler_runtime(conanCompilerRuntime "${cmakeMsvcRuntimeLibrary}")
        set(value "${value}" "--settings" "compiler.runtime=${conanCompilerRuntime}")
    elseif("GNU" STREQUAL "${cmakeCxxCompilerId}")
        set(value "${value}" "--settings" "compiler=gcc")

        # compiler.version
        if("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "13" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "14")
            set(value "${value}" "--settings" "compiler.version=13")
        elseif("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "12" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "13")
            set(value "${value}" "--settings" "compiler.version=12")
        elseif("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "11" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "12")
            set(value "${value}" "--settings" "compiler.version=11")
        elseif("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "10" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "11")
            set(value "${value}" "--settings" "compiler.version=10")
        elseif("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "9" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "10")
            set(value "${value}" "--settings" "compiler.version=9")
        elseif("${cmakeCxxCompilerVersion}" VERSION_GREATER_EQUAL "8" AND "${cmakeCxxCompilerVersion}" VERSION_LESS "9")
            set(value "${value}" "--settings" "compiler.version=8")
        else()
            message(FATAL_ERROR "Unsupported 'cmakeCxxCompilerVersion': '${cmakeCxxCompilerVersion}'")
        endif()

        # compiler.libcxx
        if("${cmakeCxxStandard}" VERSION_GREATER_EQUAL "11")
            set(value "${value}" "--settings" "compiler.libcxx=libstdc++11")
        else()
            set(value "${value}" "--settings" "compiler.libcxx=libstdc++")
        endif()
    else()
        message(FATAL_ERROR "Unsupported 'cmakeCxxStandard': '${cmakeCxxStandard}'")
    endif()

    # build_type
    if("MinSizeRel" STREQUAL "${cmakeBuildType}")
        set(value "${value}" "--settings" "build_type=${cmakeBuildType}")
    elseif("Release" STREQUAL "${cmakeBuildType}")
        set(value "${value}" "--settings" "build_type=${cmakeBuildType}")
    elseif("RelWithDebInfo" STREQUAL "${cmakeBuildType}")
        set(value "${value}" "--settings" "build_type=${cmakeBuildType}")
    elseif("Debug" STREQUAL "${cmakeBuildType}")
        set(value "${value}" "--settings" "build_type=${cmakeBuildType}")
    else()
        message(FATAL_ERROR "Unsupported 'cmakeBuildType': '${cmakeBuildType}'")
    endif()

    # additional
    if(NOT "" STREQUAL "${ARGN}")
        foreach(arg ${ARGN})
            list(APPEND value "--settings" "${arg}")
        endforeach()
    endif()

    set("${var}" "${value}" PARENT_SCOPE)
endfunction()

function(set_python_boolean var cmakeBoolean)
    if("" STREQUAL "${var}")
        message(FATAL_ERROR "Empty value not supported for 'var'.")
    endif()

    if("${cmakeBoolean}")
        set(value "True")
    else()
        set(value "False")
    endif()

    set("${var}" "${value}" PARENT_SCOPE)
endfunction()

function(set_conan_options var)
    if("" STREQUAL "${var}")
        message(FATAL_ERROR "Empty value not supported for 'var'.")
    endif()

    # all options
    if(NOT "" STREQUAL "${ARGN}")
        foreach(arg ${ARGN})
            if(NOT "" STREQUAL "${arg}" AND NOT "${arg}" IN_LIST "value")
                list(APPEND value "--options" "${arg}")
            endif()
        endforeach()
    endif()

    set("${var}" "${value}" PARENT_SCOPE)
endfunction()

function(set_not_found_package_names var)
    if("" STREQUAL "${var}")
        message(FATAL_ERROR "Empty value not supported for 'var'.")
    endif()

    if(NOT "" STREQUAL "${ARGN}")
        foreach(arg ${ARGN})
            if(NOT "${${arg}_FOUND}")
                list(APPEND value "${arg}")
            endif()
        endforeach()
    endif()

    set("${var}" "${value}" PARENT_SCOPE)
endfunction()

function(
    set_target_names
    var
    dir
)
    get_property(subdirectories DIRECTORY "${dir}" PROPERTY SUBDIRECTORIES)
    foreach(subdir ${subdirectories})
        set_target_names(subTargets "${subdir}")
    endforeach()
    if(NOT "${subTargets}" STREQUAL "")
        list(APPEND targets "${subTargets}")
    endif()
    get_property(currentTargets DIRECTORY "${dir}" PROPERTY BUILDSYSTEM_TARGETS)
    if(NOT "${currentTargets}" STREQUAL "")
        list(APPEND targets "${currentTargets}")
    endif()
    set(${var} "${targets}" PARENT_SCOPE)
endfunction()

#[[function(print_variables messageMode prefix)
    foreach(n ${ARGN})
        if("" STREQUAL "${messageMode}")
            message("${prefix}${n}: '${${n}}'")
        else()
            message("${messageMode}" "${prefix}${n}: '${${n}}'")
        endif()
    endforeach()
endfunction()]]

#[[function(print_environment_variables messageMode prefix)
    foreach(n ${ARGN})
        if("" STREQUAL "${messageMode}")
            message("${prefix}${n}: '$ENV{${n}}'")
        else()
            message("${messageMode}" "${prefix}ENV{${n}}: '$ENV{${n}}'")
        endif()
    endforeach()
endfunction()]]

#[[macro(print_all_variables projectName messageMode prefix)
    if("" STREQUAL "${projectName}")
        message(FATAL_ERROR "Empty value not supported for 'projectName'.")
    endif()

    if("" STREQUAL "${ARGN}")
        get_cmake_property("${projectName}_VARIABLES" VARIABLES)
        foreach(n ${${PROJECT_NAME}_VARIABLES})
            if("" STREQUAL "${messageMode}")
                message("${prefix}${n}: '${${n}}'")
            else()
                message("${messageMode}" "${prefix}${n}: '${${n}}'")
            endif()
        endforeach()
    else()
        foreach(n ${ARGN})
            if("<nl>" STREQUAL "${n}")
                if("" STREQUAL "${messageMode}")
                    message("${prefix}")
                else()
                    message("${messageMode}" "${prefix}")
                endif()
            else()
                if("" STREQUAL "${messageMode}")
                    message("${prefix}${n}: '${${n}}'")
                else()
                    message("${messageMode}" "${prefix}${n}: '${${n}}'")
                endif()
            endif()
        endforeach()
    endif()
endmacro()]]
