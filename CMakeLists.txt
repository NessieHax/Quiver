cmake_minimum_required(VERSION 3.30.0)
project(PCKPP
    VERSION 1.0.0.0
    DESCRIPTION "A C++ Rewrite of PCK Studio, a tool to edit Minecraft Legacy Console Edition file formats!"
    LANGUAGES CXX
)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
enable_language(CXX)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
set(SRC ${SOURCE_FILES})

# add_definitions(-DUSE_VIEWPORTS)
string(LENGTH "${CMAKE_SOURCE_DIR}/" SOURCE_PATH_SIZE)
add_definitions("-DSOURCE_PATH_SIZE=${SOURCE_PATH_SIZE}")
add_definitions("-D__FILENAME__=(__FILE__ + SOURCE_PATH_SIZE)")

# TODO: Add icons for other platforms
# Compilation step for static resources
function(add_resources out_var)
    set(result)

    FOREACH(in_f ${ARGN})
        FILE(RELATIVE_PATH src_f ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/${in_f})
        SET(out_f "${PROJECT_BINARY_DIR}/${in_f}.o")
        ADD_CUSTOM_COMMAND(PRE_BUILD OUTPUT ${out_f}
            COMMAND ld -r -b binary -o ${out_f} ${src_f}
            DEPENDS ${in_f}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMENT "Building object ${out_f}"
            VERBATIM
        )
        LIST(APPEND result ${out_f})
    endforeach()

    set(${out_var} "${result}" PARENT_SCOPE)
endfunction()

# Windows embedded resources
# if(WIN32)
# set(RESOURCE_RC "${CMAKE_CURRENT_SOURCE_DIR}/assets/platforms/windows/resources.rc")

# if(EXISTS ${RESOURCE_RC})
# message(STATUS "Including Windows resource file: ${RESOURCE_RC}")
# set(RESOURCE_FILES ${RESOURCE_RC})
# else()
# message(WARNING "Windows resource file not found: ${RESOURCE_RC}")
# endif()
# endif()
add_resources(RESOURCE_FILES "assets/platforms/windows/pckpp.png")

add_executable(PCKPP ${SRC} ${RESOURCE_FILES})

if(WIN32)
    set_target_properties(PCKPP PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
    set_target_properties(PCKPP PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS")
endif()

find_package(OpenGL REQUIRED)
target_link_libraries(PCKPP PRIVATE OpenGL::GL)
target_link_libraries(PCKPP PRIVATE OpenGL::GLU)

add_subdirectory(vendor/glad)
target_link_libraries(PCKPP PRIVATE glad)

add_subdirectory(vendor/sdl)
target_link_libraries(PCKPP PRIVATE SDL3::SDL3)

add_library(imgui STATIC
    ${CMAKE_CURRENT_LIST_DIR}/vendor/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vendor/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vendor/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vendor/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vendor/imgui/backends/imgui_impl_sdl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/vendor/imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/vendor/imgui
    ${CMAKE_CURRENT_LIST_DIR}/vendor/stb
    ${CMAKE_CURRENT_LIST_DIR}/src
)

target_link_libraries(imgui PRIVATE SDL3::SDL3)

target_link_libraries(PCKPP PRIVATE imgui)

target_link_libraries(imgui PRIVATE opengl32)

set(SDL3_DLL "${CMAKE_BINARY_DIR}/vendor/SDL/SDL3.dll")

add_custom_command(TARGET PCKPP POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SDL3_DLL}" $<TARGET_FILE_DIR:PCKPP>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:PCKPP>/assets
)