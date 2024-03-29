cmake_minimum_required(VERSION 3.17)

project(CudaApp
    VERSION 1.0.0
    DESCRIPTION "CUDA apps"
    LANGUAGES CXX CUDA
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE Release)
endif()

# create SOURCE_LIB static library: user souce library
add_subdirectory(src)
add_subdirectory(bench)

# final binary output
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(Boost_USE_STATIC_LIBS ON)
endif()
find_package(Boost COMPONENTS program_options REQUIRED)

# add important compile features
add_executable(${PROJECT_NAME}
    src/main.cc
    src/entrypoint.cc
    )
target_compile_features(${PROJECT_NAME} PRIVATE
    cxx_std_17
    )
target_compile_options(${PROJECT_NAME} PRIVATE
    "-Wall"
    "-Wextra"
    $<$<CONFIG:Release>:-Werror -Ofast>
    $<$<CONFIG:Debug>:-O0 -ggdb>
    "-pedantic"
    )
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${Boost_INCLUDE_DIR}
    )

target_link_libraries(${PROJECT_NAME} LINK_PUBLIC
    ${Boost_LIBRARIES}
    SOURCE_LIB
    )
