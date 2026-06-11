project(lob_business_logic)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "src/*.cpp")
add_library(lob_business_logic ${SOURCE_FILES})

find_package(Boost REQUIRED)

target_include_directories(lob_business_logic INTERFACE ./include)
target_include_directories(lob_business_logic PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(lob_business_logic PUBLIC reciever)
target_link_libraries(lob_business_logic PUBLIC infrastructure)