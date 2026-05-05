project(lob_visualizer VERSION 0.1 LANGUAGES CXX)

#Make cmake take prefix path for qt atomatically later <---------
include(${CMAKE_BINARY_DIR}/generators/conan_toolchain.cmake)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
    "*.h"
)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()
qt_add_executable(lob_visualizer ${PROJECT_SOURCES})

target_link_libraries(lob_visualizer PRIVATE Qt6::Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

message(STATUS "DEBUG: CMAKE_PREFIX_PATH is ${CMAKE_PREFIX_PATH}")

# if(APPLE)
#     target_link_libraries(lob_visualizer PRIVATE
#         "-framework Metal"
#         "-framework QuartzCore"
#         "-framework Cocoa"
#         "-framework Foundation"
#     )
# endif()

set_target_properties(lob_visualizer PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
