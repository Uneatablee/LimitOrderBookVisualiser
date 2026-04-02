cmake_minimum_required(VERSION 4.2.3)
project(lob)

SET(CMAKE_CXX_STANDARD 23)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra)

#Agl error mock solution
if(APPLE)
    message(STATUS "Creating agl mock")
    set(FAKE_FRAMEWORKS_DIR "${CMAKE_BINARY_DIR}/FakeFrameworks")
    set(AGL_FRAMEWORK_DIR "${FAKE_FRAMEWORKS_DIR}/AGL.framework")

    if(NOT EXISTS "${AGL_FRAMEWORK_DIR}/AGL")
        file(MAKE_DIRECTORY "${AGL_FRAMEWORK_DIR}")
        file(WRITE "${FAKE_FRAMEWORKS_DIR}/empty_agl.c" "void agl_dummy(void) {}")
        execute_process(
            COMMAND clang -dynamiclib -o "${AGL_FRAMEWORK_DIR}/AGL" "${FAKE_FRAMEWORKS_DIR}/empty_agl.c"
        )
    endif()
    add_link_options("-F${FAKE_FRAMEWORKS_DIR}")
endif()
# --------------------------------

add_subdirectory(src/lob_business_logic)
add_subdirectory(src/console)
#add_subdirectory(src/abides_generator)
add_subdirectory(src/qt)
add_subdirectory(tests)

