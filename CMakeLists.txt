cmake_minimum_required(VERSION 3.10)

project(AdevAnalysis)

find_package(Eigen3 REQUIRED)

add_library(sensors
    ${CMAKE_CURRENT_SOURCE_DIR}/include/libsensors.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/libsensors.cpp
)
target_compile_features(sensors PUBLIC cxx_std_17)
target_include_directories(sensors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(sensors PRIVATE Eigen3::Eigen)

add_executable(sensors_decode
    ${CMAKE_CURRENT_SOURCE_DIR}/src/sensors_decode.cpp
)
target_compile_features(sensors_decode PUBLIC cxx_std_17)
target_link_libraries(sensors_decode PUBLIC sensors)

add_executable(compute_mean
    ${CMAKE_CURRENT_SOURCE_DIR}/src/compute_mean.cpp
)
target_compile_features(compute_mean PUBLIC cxx_std_17)
target_include_directories(compute_mean
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(adev_analysis
    ${CMAKE_CURRENT_SOURCE_DIR}/src/adev_analysis.cpp
)
target_include_directories(adev_analysis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_compile_features(adev_analysis PUBLIC cxx_std_17)
