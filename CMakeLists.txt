cmake_minimum_required(VERSION 3.14)
set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "Explicit Makefile")
set(SANITIZE OFF CACHE BOOL "Build with sanitizers")

project(random_benchmarks
	VERSION 1.0
	DESCRIPTION "Benchmarks of multiple C++ random generator"
	LANGUAGES CXX)

# Build types
set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release")
endif()

#setting includes and sources
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
file(GLOB SOURCES "${SRC_DIR}/*.cpp")

# add binary
add_executable(random_benchmarks ${SOURCES})
target_include_directories(random_benchmarks PUBLIC ${INCLUDE_DIR})

#Setting C++ flags
target_compile_features(random_benchmarks PUBLIC cxx_std_20)
target_compile_options(random_benchmarks PUBLIC
	$<$<CXX_COMPILER_ID:MSVC>:/W4>
	$<$<CXX_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
	#$<$<CXX_COMPILER_ID:Clang>:-stdlib=libc++>
	$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:/O2>
	$<$<AND:$<CONFIG:Release>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-O3 -march=native -g>
	$<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:Clang>>:-Ofast>
	$<$<CONFIG:Debug>:-g>
	$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<BOOL:${SANITIZE}>>:-fsanitize=address -fno-omit-frame-pointer -fsanitize=undefined -fno-sanitize=vptr>
	$<$<AND:$<BOOL:${COVERAGE}>,$<CXX_COMPILER_ID:GNU>>:-ftest-coverage -fprofile-arcs>
	$<$<AND:$<BOOL:${COVERAGE}>,$<CXX_COMPILER_ID:Clang>>:-fprofile-instr-generate -fcoverage-mapping>
	)

target_link_options(random_benchmarks PUBLIC
	$<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:Clang>,$<BOOL:${SANITIZE}>>:-fsanitize=address -fsanitize=undefined -fno-sanitize=vptr>
	$<$<AND:$<BOOL:${COVERAGE}>,$<NOT:$<CXX_COMPILER_ID:MSVC>>>:-fprofile-instr-generate>
	#$<$<CXX_COMPILER_ID:Clang>:-fuse-ld=lld>
	)

include(FetchContent)
cmake_policy(SET CMP0135 NEW)

# Google test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# GoogleBenchmark
FetchContent_Declare(
	googlebenchmark
	URL https://github.com/google/benchmark/archive/refs/tags/v1.8.0.zip
)
FetchContent_MakeAvailable(googlebenchmark)
target_link_libraries(random_benchmarks benchmark::benchmark benchmark::benchmark_main)

# pcg-cpp
FetchContent_Declare(
	pcg-cpp
	GIT_REPOSITORY https://github.com/imneme/pcg-cpp.git
	GIT_TAG 428802d
)
FetchContent_MakeAvailable(pcg-cpp)
target_include_directories(random_benchmarks PUBLIC ${pcg-cpp_SOURCE_DIR})

# pcg32
FetchContent_Declare(
	pcg32
	GIT_REPOSITORY https://github.com/wjakob/pcg32.git
	GIT_TAG 70099ea
)
FetchContent_MakeAvailable(pcg32)
target_include_directories(random_benchmarks PUBLIC ${pcg32_SOURCE_DIR})

