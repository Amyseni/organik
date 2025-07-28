cmake_minimum_required(VERSION 3.25.0 FATAL_ERROR)
set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(${PROJ_NAME} CXX)

message("Project version: ${PROJECT_VERSION}")

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x86")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################
set(PROJ_NAME ${PROJ_NAME})


################################################################################
# Source groups
################################################################################
include_directories("include")
include_directories("src")
include_directories("src/DefinitionHelpers")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_library(${PROJ_NAME} SHARED 
    "src/Logging.h"
    "src/Logging.cpp"
    "include/GameMaker_Defs.h"
    "include/imgui/imconfig.h"
    "include/imgui/imgui_internal.h"
    "include/imgui/imgui.h"
    "include/imgui/imstb_rectpack.h"
    "include/imgui/imstb_textedit.h"
    "include/imgui/imstb_truetype.h"
    "include/imgui/imgui_impl_dx11.h"
    "include/imgui/imgui_stdlib.h"
    "include/imgui/imgui_impl_win32.h"
    "src/detours.h"
    "src/PALMemoryProtection.h"
    "src/SigScan.h"
    "src/zhl.h"
    "src/zhl_internal.h"
    "src/zhl_private.h"
    "src/hde.h"
    "src/Synthetik.h"
    "include/imgui/imgui_demo.cpp"
    "include/imgui/imgui_draw.cpp"
    "include/imgui/imgui_tables.cpp"
    "include/imgui/imgui_widgets.cpp"
    "include/imgui/imgui_impl_dx11.cpp"
    "include/imgui/imgui_stdlib.cpp"
    "include/imgui/imgui_impl_win32.cpp"
    "include/imgui/imgui.cpp"
    "src/Organik/UI/UIManager.h"
    "src/Organik/UI/VariableViewer.h"
    "src/Organik/UI/VariableViewer.cpp"
    "src/Organik/UI/UIElement.h"
    "src/Organik/UI/MainMenu.h"
    "src/Organik/UI/MainMenu.cpp"
    "src/Organik/UI/UIManager.cpp"
    "src/SigScan.cpp"
    "src/RValue.cpp"
    "src/Synthetik.cpp"
    "src/main.cpp"
    "src/hde.cpp"
    "src/zhl.cpp"
    )

set(CMAKE_SHARED_LIBRARY_PREFIX_CXX "")

target_compile_features(${PROJ_NAME} PRIVATE cxx_std_23)

find_path(DIRECTX_INCLUDE_DIR d3d11.h
    PATHS "/nvme2/organik/MSVC_Hackaround/msvc-downl/kits/10/Lib/10.0.26100.0/um" # Example path, adjust SDK version
    NO_DEFAULT_PATH
)
find_library(DIRECTX_LIB d3d11.lib
    PATHS "/nvme2/organik/MSVC_Hackaround/msvc-downl/kits/10/Lib/10.0.26100.0/um/x86" # Example path, adjust SDK version and architecture
    NO_DEFAULT_PATH
)
if(DIRECTX_INCLUDE_DIR)
    include_directories(${DIRECTX_INCLUDE_DIR})
endif()
include_directories("/usr/local/include/luajit-2.1")
################################################################################
# Target
################################################################################

use_props(${PROJ_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Src)
if(DIRECTX_LIB)
    target_link_libraries(${PROJ_NAME} PRIVATE ${DIRECTX_LIB} dxgi.lib d3dcompiler.lib dxguid.lib)
endif()
set_target_properties(${PROJ_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)


if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJ_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJ_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()

################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJ_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:Release>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Debug>:
            MultiThreadedDebugDLL
        >
        $<$<CONFIG:Release>:
            MultiThreadedDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Debug>,$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJ_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJ_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJ_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG;"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
        ">"
        "_CRT_SECURE_NO_WARNINGS;"
        "LIBZHL_EXPORTS;"
        "_WIN32;"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJ_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG;"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG;"
        ">"
        "_CRT_SECURE_NO_WARNINGS;"
        "LIBZHL_EXPORTS;"
        "__i386__;"
        "_WIN32;"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()

target_compile_definitions(${PROJ_NAME} PRIVATE _REENTRANT)
target_compile_definitions(${PROJ_NAME} PRIVATE LIBZHL_EXPORTS)
################################################################################
# Compile and link options
################################################################################

if(MSVC)
#    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
#        target_compile_options(${PROJ_NAME} PRIVATE
#            $<$<CONFIG:Debug>:
#                /W4;
#                /WX;
#                /Z7
#            >
#            $<$<CONFIG:Release>:
#                /Oi;
#                /Gy;
#                /W3
#            >
#            /permissive-;
#            /std:c++23;
#            /sdl;
#            ${DEFAULT_CXX_EXCEPTION_HANDLING}
#        )
#    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
#        target_compile_options(${PROJ_NAME} PRIVATE
#            $<$<CONFIG:Debug>:
#                /W4;
#                /WX;
#                /Z7
#            >
#            $<$<CONFIG:Release>:
#                /Oi;
#                /Gy;
#                /W3
#            >
#            /permissive-;
#            /std:c++23;
#            /sdl;
#            ${DEFAULT_CXX_EXCEPTION_HANDLING}
#        )
#    endif()
#    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
#        target_link_options(${PROJ_NAME} PRIVATE
#            $<$<CONFIG:Release>:
#                /OPT:REF;
#                /OPT:ICF
#            >
#            /NDEBUG;
#            /SUBSYSTEM:CONSOLE
#        )
#    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
#        target_link_options(${PROJ_NAME} PRIVATE
#            $<$<CONFIG:Release>:
#                /OPT:REF;
#                /OPT:ICF
#            >
#            /NDEBUG;
#            /SUBSYSTEM:CONSOLE
#        )
#    endif()
endif()

