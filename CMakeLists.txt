cmake_minimum_required(VERSION 3.24)

project(NoteApp)

# 使用MSVC构建
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc")
endif()

# 启用自动元对象编译
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find the Qt6 package
find_package(Qt6 6.10 REQUIRED COMPONENTS Core Gui Widgets PrintSupport Sql)

# 设置MSVC运行时库
if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加源文件
set(APP_SOURCES
    main.cpp
    MainWindow.cpp
    NoteEditor.cpp
    CategoryManager.cpp
    TagManager.cpp
)

set(APP_HEADERS
    MainWindow.h
    NoteEditor.h
    CategoryManager.h
    TagManager.h
)
# 添加可执行文件
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
    ${APP_SOURCES}
    ${APP_HEADERS}
)

# 包含当前目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# 链接Qt6模块
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::PrintSupport
    Qt6::Sql
)
