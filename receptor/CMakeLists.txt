
cmake_minimum_required(VERSION 3.12)

project(receptor 
    VERSION 0.0.1 
    LANGUAGES C 
    DESCRIPTION "Receptor library"
)

find_library(WS2_32_LIBRARY ws2_32)
if(NOT WS2_32_LIBRARY)
    message(FATAL_ERROR "Cannot find ws2_32 library")
endif()

# 构建选项
option(RECEPTOR_BUILD_STATIC "Build receptor as static library" OFF)
option(RECEPTOR_BUILD_SHARED "Build receptor as shared library" ON)
option(RECEPTOR_BUILD_EXECUTABLE "Build receptor executable for testing" ON)

# 自动收集源文件
file(GLOB_RECURSE RECEPTOR_CORE_SOURCES "src/core/*.c")
file(GLOB_RECURSE RECEPTOR_EVENT_SOURCES "src/event/*.c")
file(GLOB_RECURSE RECEPTOR_HTTP_SOURCES "src/http/*.c")

# 平台特定源文件
if(WIN32)
    set(RECEPTOR_EVENT_MODULE_SOURCES 
        "src/event/module/receptor_event_iocp.c"
        "src/event/module/receptor_event_select.c"
    )
    file(GLOB RECEPTOR_OS_PLATFORM_SOURCES "src/os/win32/*.c")
else()
    set(RECEPTOR_EVENT_MODULE_SOURCES 
        "src/event/module/receptor_event_epoll.c"
        "src/event/module/receptor_event_select.c"
    )
    file(GLOB RECEPTOR_OS_PLATFORM_SOURCES "src/os/unix/*.c")
endif()

# 合并所有源文件
set(ALL_SOURCES
    ${RECEPTOR_CORE_SOURCES}
    ${RECEPTOR_EVENT_SOURCES}
    ${RECEPTOR_EVENT_MODULE_SOURCES}
    ${RECEPTOR_HTTP_SOURCES}
    ${RECEPTOR_OS_PLATFORM_SOURCES}
)


# 构建库
if(RECEPTOR_BUILD_SHARED)
    add_library(receptor SHARED ${ALL_SOURCES})

    if(WIN32)
        # 启用自动导出所有符号
        set_target_properties(receptor PROPERTIES
            WINDOWS_EXPORT_ALL_SYMBOLS ON
        )
        
        # 或者手动设置导出宏
        target_compile_definitions(receptor PRIVATE 
            RECEPTOR_DLL_EXPORT
        )
    endif()
    
    message(STATUS "Building receptor as shared library")
elseif(RECEPTOR_BUILD_STATIC)
    add_library(receptor STATIC ${ALL_SOURCES})
    message(STATUS "Building receptor as static library")
else()
    message(FATAL_ERROR "Please select either SHARED or STATIC library type")
endif()

# 包含目录
target_include_directories(receptor PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/event
    ${CMAKE_CURRENT_SOURCE_DIR}/src/http
    ${CMAKE_CURRENT_SOURCE_DIR}/os/win32
)

target_link_libraries(receptor PRIVATE ${WS2_32_LIBRARY})


# 可执行文件（可选）
if(RECEPTOR_BUILD_EXECUTABLE)
    add_executable(receptor_test tests/main.c)
    target_link_libraries(receptor_test PUBLIC receptor)
    message(STATUS "Building test executable")
endif()