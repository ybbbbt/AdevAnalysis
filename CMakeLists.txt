cmake_minimum_required(VERSION 3.10)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

project(AdevAnalysis)

find_package(TBB 2018 REQUIRED)

add_executable(aa
    AdevAnalysis.cpp
    gnuplot.h
    Random.h
    RANSAC.h
)
# target_include_directories(aa
#     PUBLIC
#         ${TBB_INCLUDE_DIRS}
# )
target_compile_features(aa
    PUBLIC
        cxx_std_17
)
target_link_libraries(aa
    PUBLIC
        TBB::tbb
)
