cmake_minimum_required(VERSION 3.21)

# Static MSVC runtime, matching the x64-windows-static vcpkg triplet (set before project()).
if(NOT DEFINED CMAKE_MSVC_RUNTIME_LIBRARY)
	set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()
if(NOT DEFINED VCPKG_TARGET_TRIPLET)
	set(VCPKG_TARGET_TRIPLET "x64-windows-static" CACHE STRING "")
endif()

project(CommonLibSSENGTemplate VERSION 2.0.0 LANGUAGES CXX)

set(PLUGIN_NAME "ExamplePlugin" CACHE STRING "Name of the generated SKSE plugin DLL")
set(PLUGIN_AUTHOR "YourName" CACHE STRING "Plugin author shown to SKSE")
# UNIVERSAL: one DLL for SE 1.5.97, AE 1.6/1.7 and VR 1.4.15. Class layouts that differ are selected at runtime
# (GetRuntimeData(), RelocateMember, RuntimeMember). SE or AE limit the build to one runtime.
set(SKYRIM_VARIANT "UNIVERSAL" CACHE STRING "Skyrim runtime variant: UNIVERSAL, SE, AE, or VR")
set_property(CACHE SKYRIM_VARIANT PROPERTY STRINGS UNIVERSAL SE AE VR)
option(COPY_BUILD "Copy the DLL and PDB to Skyrim's SKSE plugin directory." OFF)
option(BUILD_SKYRIMVR "Same as SKYRIM_VARIANT=VR." OFF)

if(NOT DEFINED SKYRIM_PATH AND DEFINED ENV{Skyrim64Path})
	set(SKYRIM_PATH "$ENV{Skyrim64Path}" CACHE PATH "Skyrim Special Edition game directory")
endif()

string(TOUPPER "${SKYRIM_VARIANT}" SKYRIM_VARIANT)
if(BUILD_SKYRIMVR)
	set(SKYRIM_VARIANT "VR")
endif()

set(COMMONLIB_NAME "CommonLibSSE")
set(COMMONLIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/extern/CommonLibSSE" CACHE PATH "MIT CommonLibSSE NG checkout")
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
if(SKYRIM_VARIANT STREQUAL "UNIVERSAL")
	set(ENABLE_SKYRIM_SE ON CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_AE ON CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_VR ON CACHE BOOL "" FORCE)
elseif(SKYRIM_VARIANT STREQUAL "SE")
	set(ENABLE_SKYRIM_SE ON CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_AE OFF CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_VR OFF CACHE BOOL "" FORCE)
elseif(SKYRIM_VARIANT STREQUAL "AE")
	set(ENABLE_SKYRIM_SE OFF CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_AE ON CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_VR OFF CACHE BOOL "" FORCE)
elseif(SKYRIM_VARIANT STREQUAL "VR")
	set(ENABLE_SKYRIM_SE OFF CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_AE OFF CACHE BOOL "" FORCE)
	set(ENABLE_SKYRIM_VR ON CACHE BOOL "" FORCE)
else()
	message(FATAL_ERROR "SKYRIM_VARIANT must be UNIVERSAL, SE, AE, or VR.")
endif()

if(NOT EXISTS "${COMMONLIB_PATH}/include/REL/Relocation.h")
	message(FATAL_ERROR "COMMONLIB_PATH must point to the selected CommonLib checkout.")
endif()

add_subdirectory("${COMMONLIB_PATH}" ${COMMONLIB_NAME})

add_library(${PLUGIN_NAME} SHARED src/TemplatePlugin.cpp)
target_compile_features(${PLUGIN_NAME} PRIVATE cxx_std_23)
target_compile_definitions(${PLUGIN_NAME} PRIVATE
	TEMPLATE_PLUGIN_NAME="${PLUGIN_NAME}"
	TEMPLATE_PLUGIN_AUTHOR="${PLUGIN_AUTHOR}"
)
target_link_libraries(${PLUGIN_NAME} PRIVATE ${COMMONLIB_NAME}::${COMMONLIB_NAME})

if(MSVC)
	target_compile_options(${PLUGIN_NAME} PRIVATE /permissive- /utf-8 /W4)
endif()

if(COPY_BUILD)
	if(NOT SKYRIM_PATH)
		message(FATAL_ERROR "COPY_BUILD requires SKYRIM_PATH or the Skyrim64Path environment variable.")
	endif()
	add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${SKYRIM_PATH}/Data/SKSE/Plugins"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PLUGIN_NAME}>" "${SKYRIM_PATH}/Data/SKSE/Plugins/"
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_PDB_FILE:${PLUGIN_NAME}>" "${SKYRIM_PATH}/Data/SKSE/Plugins/"
	)
endif()
