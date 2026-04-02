project(lob_business_logic)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_library(lob_business_logic ${SOURCE_FILES})
target_include_directories(lob_business_logic INTERFACE ./include)

#Dependencies
find_package(ZeroMQ REQUIRED)
target_link_libraries(lob_business_logic PRIVATE libzmq-static)