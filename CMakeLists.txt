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

# GoogleBenchmark
FetchContent_Declare(
	googlebenchmark
	URL https://github.com/google/benchmark/archive/refs/tags/v1.8.0.zip
)
FetchContent_MakeAvailable(googlebenchmark)
target_link_libraries(random_benchmarks benchmark::benchmark benchmark::benchmark_main)
