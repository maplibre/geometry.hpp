cmake_minimum_required(VERSION 3.30)
project(hpp_skel LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(WERROR "Add -Werror flag to build (turns warnings into errors)" ON)

# configure optimization
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTIMIZATION_FLAGS "-O0 -DDEBUG")
    message("-- Configuring debug build")
else()
    set(OPTIMIZATION_FLAGS "-O3 -DNDEBUG")
    message("-- Configuring release build")
endif()

# Enable extra warnings to adhere to https://github.com/mapbox/cpp/issues/37
set(DESIRED_WARNINGS "-Wall -Wextra -Wconversion -Wunreachable-code -Wuninitialized -pedantic-errors -Wold-style-cast -Wno-error=unused-variable -Wshadow -Wfloat-equal -Weffc++")
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  set(DESIRED_WARNINGS "${DESIRED_WARNINGS} -Wmost")
endif()

# Note: -D_GLIBCXX_USE_CXX11_ABI=0 is needed to support mason packages that are precompiled libs
# Currently we only depend on a header only library, but this will help avoid issues when more libs are added via mason
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS} -D_GLIBCXX_USE_CXX11_ABI=0 ${DESIRED_WARNINGS}")

if (WERROR)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB TEST_SOURCES test/*.cpp)
add_executable(unit-tests ${TEST_SOURCES})

Include(FetchContent)

# SYSTEM property is not propagated so
# set this globally ¯\_(ツ)_/¯
# https://gitlab.kitware.com/cmake/cmake/-/issues/25364
add_compile_options(-Wno-error)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
  SYSTEM
)
FetchContent_MakeAvailable(Catch2)

target_link_libraries(unit-tests PRIVATE Catch2WithMain)

# libbenchmark.a supports threads and therefore needs pthread support
find_package(Threads REQUIRED)
file(GLOB BENCH_SOURCES bench/*.cpp)
add_executable(bench-tests ${BENCH_SOURCES})

