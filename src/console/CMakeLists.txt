project(console)

add_executable(console main.cpp)

target_link_libraries(console PUBLIC lob_business_logic)