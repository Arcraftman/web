cmake_minimum_required(VERSION 3.12)

project(receptor VERSION 1.0.0 LANGUAGES C DESCRIPTION "Receptor network library")

# 构建选项
option(RECEPTOR_BUILD_STATIC "Build receptor as static library" ON)
option(RECEPTOR_BUILD_SHARED "Build receptor as shared library" OFF)
option(RECEPTOR_BUILD_EXECUTABLE "Build receptor executable for testing" OFF)

# 设置默认构建类型为静态库
if(NOT RECEPTOR_BUILD_STATIC AND NOT RECEPTOR_BUILD_SHARED)
    set(RECEPTOR_BUILD_STATIC ON)
endif()

# 包含目录
include_directories(src/include)

# 自动收集源文件
file(GLOB_RECURSE RECEPTOR_CORE_SOURCES 
    "src/core/*.c"
)

file(GLOB_RECURSE RECEPTOR_EVENT_SOURCES 
    "src/event/*.c"
)

file(GLOB_RECURSE RECEPTOR_HTTP_SOURCES 
    "src/http/*.c"
)

file(GLOB_RECURSE RECEPTOR_OS_SOURCES 
    "src/os/*.c"
)

# 平台特定源文件
if(WIN32)
    # Windows 使用 IOCP 和 select
    set(RECEPTOR_EVENT_MODULE_SOURCES 
        "src/event/receptor_event_iocp.c"
        "src/event/receptor_event_select.c"
    )
    
    # Windows 特定 OS 源文件
    set(RECEPTOR_OS_PLATFORM_SOURCES 
        "src/os/win32/*.c"
    )
else()
    # Unix 使用 epoll/kqueue
    set(RECEPTOR_EVENT_MODULE_SOURCES 
        "src/event/receptor_event_epoll.c"
        "src/event/receptor_event_select.c"
    )
    
    # Unix 特定 OS 源文件
    set(RECEPTOR_OS_PLATFORM_SOURCES 
        "src/os/unix/*.c"
    )
endif()

# 合并所有源文件
set(ALL_SOURCES
    ${RECEPTOR_CORE_SOURCES}
    ${RECEPTOR_EVENT_SOURCES}
    ${RECEPTOR_EVENT_MODULE_SOURCES}
    ${RECEPTOR_HTTP_SOURCES}
    ${RECEPTOR_OS_SOURCES}
)

# 从源文件中排除不兼容的文件
if(WIN32)
    # Windows 排除 Unix 专用文件
    list(FILTER ALL_SOURCES EXCLUDE REGEX ".*receptor_event_epoll\\.c$")
    list(FILTER ALL_SOURCES EXCLUDE REGEX ".*/unix/.*")
else()
    # Unix 排除 Windows 专用文件  
    list(FILTER ALL_SOURCES EXCLUDE REGEX ".*receptor_event_iocp\\.c$")
    list(FILTER ALL_SOURCES EXCLUDE REGEX ".*/win32/.*")
endif()

# 配置头文件设置
set(receptor_VERSION_MAJOR 1)
set(receptor_VERSION_MINOR 0)
set(receptor_VERSION_PATCH 0)
set(receptor_VERSION "${receptor_VERSION_MAJOR}.${receptor_VERSION_MINOR}.${receptor_VERSION_PATCH}")

# 平台检测
if(WIN32)
    set(RECEPTOR_PLATFORM_WIN32 1)
    set(RECEPTOR_HAVE_IOCP 1)
    set(RECEPTOR_HAVE_SELECT 1)
    message(STATUS "Building for Windows platform")
else()
    set(RECEPTOR_PLATFORM_UNIX 1)
    set(RECEPTOR_HAVE_SELECT 1)
    message(STATUS "Building for Unix platform")
endif()

# 构建配置
if(RECEPTOR_BUILD_SHARED)
    set(RECEPTOR_BUILD_DLL 1)
endif()

# 创建构建目录
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/src/include/receptor)

# 配置头文件
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include/receptor_config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/src/include/receptor_config.h
    @ONLY
)

# 包含构建目录和源码目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
    ${CMAKE_CURRENT_BINARY_DIR}/src/include
)

# 根据配置选择构建类型
if(RECEPTOR_BUILD_STATIC)
    add_library(receptor STATIC ${ALL_SOURCES})
    message(STATUS "Building receptor as static library")
    
    # 设置静态库属性
    set_target_properties(receptor PROPERTIES
        OUTPUT_NAME "receptor"
        ARCHIVE_OUTPUT_NAME "receptor"
    )
    
    # 静态库编译定义
    target_compile_definitions(receptor PRIVATE
        RECEPTOR_VERSION="${receptor_VERSION}"
        _CRT_SECURE_NO_WARNINGS  # 禁用安全警告
    )
    
    # 平台特定定义
    if(WIN32)
        target_compile_definitions(receptor PRIVATE
            RECEPTOR_PLATFORM_WIN32=1
        )
    else()
        target_compile_definitions(receptor PRIVATE
            RECEPTOR_PLATFORM_UNIX=1
        )
    endif()
    
elseif(RECEPTOR_BUILD_SHARED)
    add_library(receptor SHARED ${ALL_SOURCES})
    message(STATUS "Building receptor as shared library")
    
    # 设置共享库属性
    set_target_properties(receptor PROPERTIES
        OUTPUT_NAME "receptor"
        LIBRARY_OUTPUT_NAME "receptor"
        VERSION ${receptor_VERSION}
        SOVERSION 1
    )
    
    # 共享库编译定义
    target_compile_definitions(receptor PRIVATE
        RECEPTOR_VERSION="${receptor_VERSION}"
        RECEPTOR_BUILD_DLL=1
        _CRT_SECURE_NO_WARNINGS
    )
    
    # 平台特定定义
    if(WIN32)
        target_compile_definitions(receptor PRIVATE
            RECEPTOR_PLATFORM_WIN32=1
        )
    else()
        target_compile_definitions(receptor PRIVATE
            RECEPTOR_PLATFORM_UNIX=1
        )
    endif()
endif()

# 编译选项
if(MSVC)
    target_compile_options(receptor PRIVATE /W4)
    
    # 禁用特定警告
    target_compile_options(receptor PRIVATE 
        /wd4996  # 安全警告
        /wd4204  # 非标准扩展警告
        /wd4100  # 未引用形参警告
    )
    
elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(receptor PRIVATE 
        -Wall 
        -Wextra
        -Wno-unused-parameter  # 忽略未使用参数警告
    )
    
    # 位置无关代码（共享库需要）
    if(RECEPTOR_BUILD_SHARED)
        target_compile_options(receptor PRIVATE -fPIC)
    endif()
endif()

# Windows 链接库
if(WIN32)
    target_link_libraries(receptor 
        ws2_32     # Winsock
        advapi32   # 注册表等
    )
endif()

# 安装配置
install(TARGETS receptor
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY src/include/receptor DESTINATION include)

# 导出目标供父项目使用
export(TARGETS receptor FILE "${CMAKE_CURRENT_BINARY_DIR}/receptor-targets.cmake")

# 可选：创建可执行文件目标
if(RECEPTOR_BUILD_EXECUTABLE)
    # 查找 main.c 文件
    file(GLOB RECEPTOR_MAIN_SOURCES 
        "src/core/main.c"
    )
    
    if(RECEPTOR_MAIN_SOURCES)
        add_executable(receptor_app ${RECEPTOR_MAIN_SOURCES})
        target_link_libraries(receptor_app receptor)
        
        set_target_properties(receptor_app PROPERTIES
            OUTPUT_NAME "receptor_app"
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        )
        
        message(STATUS "Building receptor executable: receptor_app")
    else()
        message(WARNING "main.c not found, skipping executable build")
    endif()
endif()

# 状态信息
message(STATUS "receptor sources found: ${ALL_SOURCES}")
message(STATUS "receptor configured as library subproject")