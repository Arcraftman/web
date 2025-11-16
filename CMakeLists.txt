cmake_minimum_required(VERSION 3.12)
project(MyProject VERSION 1.0.0 LANGUAGES C CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置输出目录，让可执行文件在build根目录下
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# 编译类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 添加子项目
add_subdirectory(web)
add_subdirectory(front)
add_subdirectory(agent)
add_subdirectory(receptor)

# 使用不同的目标名（不要使用"all"）
add_custom_target(build_all DEPENDS web front)

message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")