# Find libraries
find_package(OpenCV REQUIRED)
if (NOT "${OpenCV_opencv_core_FOUND}"
        OR NOT "${OpenCV_opencv_imgcodecs_FOUND}"
        OR NOT "${OpenCV_opencv_highgui_FOUND}")
    message(SEND_ERROR "Missing opencv `core`|`imgcodecs`|`highgui` library !")
endif ()

set(MY_OPENCV_LIBS "opencv_core;opencv_imgcodecs;opencv_highgui")

# ============================ CUDA SOURCE ====================================
add_library(SOURCE_LIB_CUDA SHARED
    app/pi/gpuPi.cu
    )
target_compile_features(SOURCE_LIB_CUDA PRIVATE
    cuda_std_17
    )
target_compile_options(SOURCE_LIB_CUDA PRIVATE
    "-Wall"
    "-Wextra"
    $<$<COMPILE_LANGUAGE:CUDA>:
        "-use_fast_math"
    >
    )
set_target_properties(SOURCE_LIB_CUDA
    PROPERTIES CUDA_SEPARABLE_COMPILATION ON
    )
target_link_libraries(SOURCE_LIB_CUDA
    ${MY_OPENCV_LIBS}
    )
target_include_directories(SOURCE_LIB_CUDA PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

# ============================ CXX SOURCE =====================================
add_library(SOURCE_LIB STATIC
    app/pi/cpuPi.cc
    )
target_compile_features(SOURCE_LIB PRIVATE
    cxx_std_17
    )
target_compile_options(SOURCE_LIB PRIVATE
    "-Wall"
    "-Wextra"
    $<$<CONFIG:Release>:-Werror -O3>
    $<$<CONFIG:Debug>:-O0 -ggdb>
    "-pedantic"
    )
target_link_libraries(SOURCE_LIB
    SOURCE_LIB_CUDA
    ${MY_OPENCV_LIBS}
    )
target_include_directories(SOURCE_LIB PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    )