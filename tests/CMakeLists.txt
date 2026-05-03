project(lob_business_logic_unit_tests)

file(GLOB_RECURSE SOURCES "./unit_tests/lob_business_logic/*.cpp")

find_package(GTest REQUIRED CONFIG)
add_executable(lob_business_logic_unit_tests ${SOURCES})
target_link_libraries(lob_business_logic_unit_tests PUBLIC GTest::gtest GTest::gtest_main)
target_link_libraries(lob_business_logic_unit_tests PUBLIC lob_business_logic)

include(GoogleTest)
gtest_discover_tests(lob_business_logic_unit_tests)