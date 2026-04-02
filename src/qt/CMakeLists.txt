project(lob_visualizer VERSION 0.1 LANGUAGES CXX)
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
    "*.hpp"
    "*.h"
)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()
qt_add_executable(lob_visualizer ${PROJECT_SOURCES})

target_link_libraries(lob_visualizer PUBLIC Qt6::Widgets)


set_target_properties(lob_visualizer PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
