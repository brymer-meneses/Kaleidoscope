cmake_minimum_required(VERSION 3.13.4)
project(Kaleidoscope)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE RelWithDebInfo)

include(cmake/llvm.cmake)
include(cmake/googletest.cmake)

file(GLOB Kaleidoscope_SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB TEST_FILES "${PROJECT_SOURCE_DIR}/tests/*.cpp")

# PolyLang
add_executable(Kaleidoscope src/main.cpp ${Kaleidoscope_SOURCES})
add_library(KaleidoscopeLib STATIC ${Kaleidoscope_SOURCES})

target_include_directories(KaleidoscopeLib PUBLIC ${PROJECT_SOURCE_DIR}/include) # Link to polylang
target_link_libraries(Kaleidoscope KaleidoscopeLib ${llvm_libs})

# Tests files
add_executable(TestRunner ${PROJECT_SOURCE_DIR}/tests/test_runner.cpp ${TEST_FILES})

target_include_directories(TestRunner PUBLIC ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(TestRunner KaleidoscopeLib ${llvm_libs} gtest_main gmock_main)


