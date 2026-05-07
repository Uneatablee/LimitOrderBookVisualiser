project(infrastructure)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE DEPENDS ./*.cpp)
add_library(infrastructure ${SOURCE_FILES})
target_include_directories(infrastructure INTERFACE ./Mocks/UiMocks)

