 cmake_minimum_required(VERSION 3.30)

# disable googlemock
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)

option(BUILD_SAMPLES "Builds all samples" OFF)
option(BUILD_BENCHMARKS "Builds all benchmarks" ON)

set(PROJECT_NAME matrix)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -openmp")
elseif(MSVC)
    find_package(OpenMP REQUIRED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /openmp:experimental")
else()
    find_package(OpenMP REQUIRED)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include(CTest)
enable_testing()  # defines BUILD_TESTING

set(MP2_TESTS   "test_${PROJECT_NAME}")
set(MP2_CUSTOM_PROJECT "${PROJECT_NAME}")
set(MP2_INCLUDES "${PROJECT_SOURCE_DIR}/include")

add_subdirectory(include)

if(BUILD_SAMPLES)
	add_subdirectory(samples)
endif()

if(BUILD_TESTING)
    # add googletest, version 1.18.0
    add_subdirectory("${PROJECT_SOURCE_DIR}/gtest/googletest-1.18.0")
    set(MP2_GTEST_INCLUDES "${gtest_SOURCE_DIR}/include/gtest")

	add_subdirectory(test)
endif()

#GOOGLE BENCHMARK-----------------------------------------------
if(BUILD_BENCHMARKS)
    # add googlebenchmark, version 1.9.1
    include(FetchContent)
    
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
    set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
    
    FetchContent_Declare(
      googlebenchmark
      GIT_REPOSITORY https://github.com/google/benchmark.git
      GIT_TAG        v1.9.1
    )
    
    FetchContent_MakeAvailable(googlebenchmark)
    
    add_subdirectory(benchmark)
endif()
#----------------------------------------------------------------

# REPORT
message( STATUS "")
message( STATUS "General configuration for ${PROJECT_NAME}")
message( STATUS "======================================")
message( STATUS "")
message( STATUS "   Configuration: ${CMAKE_BUILD_TYPE}")
message( STATUS "")
