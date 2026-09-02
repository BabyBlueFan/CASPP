# 该指令必须在最顶部; 要求CMake版本不低于 3.10
cmake_minimum_required(VERSION 3.10)
# !@该指令必须在最顶部; 要求CMake版本不低于 3.10

# 生成 compile_commands.json 文件
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
# !@生成 compile_commands.json 文件

# 顺序：`cmake_minimum_required()` → `project()` → 其他代码
project(MyProjectxxxx LANGUAGES C)
#MyProject 是逻辑项目名称, 并无实际意义
# ${PROJECT_NAME} 是 CMake 变量, 表示项目名称
# LANGUAGE 指定项目语言 C CXX 表示该项目支持 C 和 C++ 语言
# ${PROJECT_SOURCE_DIR} 是 CMake 变量, 表示项目根目录
# ${PROJECT_BINARY_DIR} 是 CMake 变量, 表示构建目录
# !@顺序：`cmake_minimum_required()` → `project()` → 其他代码


# 指定 语言 标准
# 全局变量，必须写在 `project()` 之后，add_executable /add_library 之前
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)   # 配套常用，很多人漏掉
set(CMAKE_C_STANDARD 99)                # C语言版本：90 / 99 / 11 / 17
set(CMAKE_C_STANDARD_REQUIRED ON)       # 强制该C标准，不支持则报错
set(CMAKE_C_EXTENSIONS OFF)             # 关闭编译器GNU扩展，跨平台
# !@指定 语言 标准


# 指定 可执行文件
add_executable(echo_client
    ${PROJECT_SOURCE_DIR}/main.c
    ${PROJECT_SOURCE_DIR}/src/csapp.c
)
# !@指定 可执行文件

# 服务器可执行文件
add_executable(echo_server
    ${PROJECT_SOURCE_DIR}/mainServer.c
    ${PROJECT_SOURCE_DIR}/src/csapp.c
)

# 头文件路径
target_include_directories(echo_client
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_include_directories(echo_server
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)
# !@头文件路径

## -----------------------------------------------------------------额外补充----------------------------------------------------------------

# 链接 库文件
# # ①: 设置 库 的搜索目录
# target_link_directories(MyEXExxxx
#     PRIVATE
#         ${PROJECT_SOURCE_DIR}/third_party/lib
# )
# # ②: 指定链接库的名字 不需要写libxxx.so / xxx.lib全名
# target_link_libraries(MyEXExxxx
#     PRIVATE
#         libxx
#         libxxxx
# )
# !@链接 库文件

# 生成库文件
# add_library(MyLibxxxx STATIC/SHARED                                             # STATIC 生成静态库 ;SHARED 生成动态库
#     ${PROJECT_SOURCE_DIR}/src/x1.cpp
#     ${PROJECT_SOURCE_DIR}/srcxx/x2.cpp
# )
# target_include_directories(Mylibxxxx
#     PUBLIC
#         ${PROJECT_SOURCE_DIR}/include
# )
# !@生成库文件
