cmake_minimum_required(VERSION 3.12)

project(agent VERSION 1.0.0 LANGUAGES C CXX)

# 包含目录
include_directories(include)

# 源文件
set(AGENT_SOURCES
    src/core/main.cpp
)

# 创建可执行文件
add_executable(agent ${AGENT_SOURCES})

# 设置输出目录（确保在build根目录）
set_target_properties(agent PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

message(STATUS "agent sources: ${AGENT_SOURCES}")