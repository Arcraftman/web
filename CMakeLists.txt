cmake_minimum_required(VERSION 3.12)
project(webset VERSION 1.0.0 LANGUAGES C CXX)

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

# 显示所有系统相关信息
message(STATUS "CMake System: ${CMAKE_SYSTEM}")
message(STATUS "CMake System Name: ${CMAKE_SYSTEM_NAME}")
message(STATUS "CMake System Version: ${CMAKE_SYSTEM_VERSION}")
message(STATUS "CMake System Processor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")

# 编译器信息
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# 添加子项目
add_subdirectory(frontend)
add_subdirectory(front)
add_subdirectory(agent)
add_subdirectory(receptor)

add_custom_target(final DEPENDS web front agent receptor)

message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")