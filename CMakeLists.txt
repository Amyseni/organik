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

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
	message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
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
include_directories("src/Organik/JSON")
include_directories("include/imgui")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
add_library(${PROJ_NAME} SHARED 
	"src/Logging.h"
	"src/Logging.cpp"
	"src/Synthetik.h"
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
	"src/main.h"
	"src/main.cpp"
	"src/detours.h"
	"src/PALMemoryProtection.h"
	"src/SigScan.h"
	"src/zhl.h"
	"src/zhl_internal.h"
	"src/zhl_private.h"
	"src/hde.h"
	"include/imgui/imgui_demo.cpp"
	"include/imgui/imgui_draw.cpp"
	"include/imgui/imgui_tables.cpp"
	"include/imgui/imgui_widgets.cpp"
	"include/imgui/imgui_impl_dx11.cpp"
	"include/imgui/imgui_stdlib.cpp"
	"src/DefinitionHelpers/InstanceHelper.cpp"
	"src/DefinitionHelpers/InstanceHelper.h"
	"src/DefinitionHelpers/room_list.h"
	"src/DefinitionHelpers/sprite_list.h"
	"src/DefinitionHelpers/var_list.h"
	"src/Hooks/CInstance.cpp"
	"src/Hooks/ScriptHooks.cpp"
	"src/Hooks/EventHooks.cpp"
	"src/Hooks/ChatHooks.cpp"
	"src/Hooks/CRoom.cpp"
	"src/Utils.cpp"
	"src/DefinitionHelpers/VariableHelper.h"
	"include/imgui/imgui_impl_win32.cpp"
	"include/imgui/imgui.cpp"
	"src/Organik/UI/ItemFilter.h"
	"src/Organik/UI/VariableViewer.h"
	"src/Organik/UI/VariableViewer.cpp"
#	"src/Organik/UI/IRCChannelTab.cpp"
#	"src/Organik/UI/IRCChannelTab.h"
	"src/Organik/UI/ChatBox.cpp"
	"src/Organik/UI/ChatBox.h"
	"src/Organik/UI/ChatTab.h"
#	"src/Chat/IRCClient.cpp"
#	"src/Chat/IRCClient.h"
#	"src/Chat/IRCHandler.cpp"
#	"src/Chat/IRCHandler.h"
#	"src/Chat/IRCSocket.cpp"
#	"src/Chat/IRCSocket.h"
	"src/Organik/UI/PartyChatTab.h"
	"src/Organik/UI/GlobalChatTab.h"
	"src/Organik/UI/PartyChatTab.cpp"
#	"src/Organik/UI/Tools/ActionEditor.cpp"
#	"src/Organik/UI/Tools/ActionEditor.h"
	"src/Organik/UI/GlobalChatTab.cpp"
	"src/Organik/UI/ChatBox.cpp"
	"src/Organik/UI/ChatTab.cpp"
	"src/Organik/UI/MainMenu.h"
	"src/Organik/UI/MainMenu.cpp"
	"src/Organik/UI/UIManager.cpp"
	"src/SigScan.cpp"
	"src/Events.h"
	"src/Globals.h"
	"src/Globals.cpp"
	"src/Organik/JSON/Action.cpp"
	"src/Organik/JSON/Action.h"
	"src/Organik/JSON/Variant.h"
	"src/Organik/JSON/Variant.cpp"
	"src/RValue.cpp"
	"src/Synthetik.cpp"
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



set_target_properties(${PROJ_NAME} PROPERTIES
	INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
)

################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJ_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)

string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"

		MultiThreadedDLL
	
)

################################################################################
# Include directories
################################################################################
target_include_directories(${PROJ_NAME} PRIVATE
	"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJ_NAME} PRIVATE
	"NDEBUG;"
	"_CRT_SECURE_NO_WARNINGS;"
	"_CRT_NONSTDC_NO_DEPRECATE;"
	"LIBZHL_EXPORTS;"
	"SECURITY_WIN32;"
	"__i386__;"
	"_WIN32;"
	"WIN32;"
	"_CONSOLE;"
	"UNICODE;"
	"_UNICODE;"
)

target_compile_definitions(${PROJ_NAME} PRIVATE _REENTRANT)
target_compile_definitions(${PROJ_NAME} PRIVATE LIBZHL_EXPORTS)
################################################################################
# Compile and link options`
################################################################################


target_compile_options(${PROJ_NAME} PRIVATE
	#! if you change ANY of these (but especially the #! marked flags), you are doing an undefined behavior unless you explicitly know what you are changing and why.
	#! DEFINE the broken behavior and the reason a flag you are changing defines the behavior, otherwise LEAVE THESE ALONE
    
    /permissive-;           #! prevent non-conformance to the C++ standard
    /Zc:rvalueCast;         #* msvc docs: causes the compiler to follow section 5.4 of the C++11 standard and treats only cast expressions that result in non-reference types 
							#* and cast expressions that result in rvalue references to non-function types as rvalue types.
							#* by default: the compiler is non-conforming, and treats all cast expressions that result in rvalue references as rvalues.
    /Zc:referenceBinding;   #! doesn't allow expressions that bind a user-defined type temporary to a non-const lvalue reference. 
							#! By default, or if /Zc:referenceBinding- is specified, the compiler allows such expressions as a Microsoft extension
	/O2;					#! optimize speed with virtually no safeguards
	/W3;                    #* warning level 3
    /Zc:wchar_t;            #* enable wchar_t as a built-in type (default, no other options disable this, listed for the sake of completeness of implied flag usage)
	/Ox;					#* MORE SPEED (implied by /O2)
    /arch:SSE2;             #! use the x86-SSE2 instruction set. (includes floating point opcodes)
    /Gr;                    #! use __fastcall for all functions except member functions
    /Gm-;                   #! disable minimum rebuild because it KEEPS MESSING UP ALL MY FREAKIN BUILDS
    /Oi-;                   #! disable intrinsic functions because THEY SUCK
	/Oy-;
	/Ob1;
    /MD;                    #* use the multithreaded DLL version of the C runtime library (msvcrt.dll)
    /std:c++23preview;      #* use latest (preview) C++ std 23
	/Zc:inline-;			#! do not remove unreferenced data or functions
    /Ot;                    #* EVEN MORE SPEED (implied by /O2)
	/Zc:nrvo-;				#! disable optional elision (skipping) of copy / move operations on returning newly instantiated variables from functions.
    /Zc:tlsGuards;			#! enables checks for initialization of thread-local variables in DLLs. Previously, thread-local variables in DLLs weren't correctly initialized. 
							#! Other than on the thread that loaded the DLL, they weren't initialized before first use on threads that existed before the DLL was loaded.
)

target_link_options(${PROJ_NAME} PRIVATE
	/NDEBUG;				#! implies certain things to the linker about symbol removal / combining things it thinks are identical. these flags you should basically never touch
							#! if you do mess with them, the linker will mess with you and you don't win that fight
	/OPT:NOREF;				#! leave unreferenced data and functions alone (but the linker this time) (implicit by /DEBUG)
	/OPT:NOICF;				#! do not iteratively fold identical COMDATs (redundant COMDATs are removed by the linker if true) (also implicit by /DEBUG)
	/LTCG:OFF;				#! NO link-time code generation optimizations. if you aren't sure why this matters DO NOT TOUCH IT
	/SUBSYSTEM:CONSOLE;
)