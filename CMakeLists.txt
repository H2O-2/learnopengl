cmake_minimum_required ( VERSION 3.0)       # 支持最低的CMake版本
project                ( learnOpenGL )              # 我的工程名叫red

# 下面几行设置c++编译选项
set(CXX_FLAGS -std=c++11 -g -Wall)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG   "-O0" )
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG " )

# 可选，设置可执行程序的输出目录，我习惯于放在build/bin. 库放在build/lib下
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

# 下面三个开关用来控制glfw的构建，关闭 帮助文档、测试项目、示例项目的构建。因为我只是把glfw作为一个库来使用。
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# Config
configure_file(config/root_directory.h.in config/root_directory.h)

# 添加 ./src 到头文件搜索路径，以后自己编写的头文件如果放在src下的话，就能被编译器搜索到。
include_directories(${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

# 添加要编译的cpp源代码，先只要一个main.cpp
set(SRC_LIST src/glad.c src/shader_s.cpp src/stb_image.h src/camera.h src/mesh.cpp src/model.cpp ${PROJECT_SOURCE_DIR}/src/parallaxMapping.cpp)
# set(SRC_LIST src/glad.c src/shader_s.cpp src/stb_image.h src/camera.h ${PROJECT_SOURCE_DIR}/src/advancedLighting.cpp)

# 添加项目的构建目标，一个可执行程序，叫：${PROJECT_NAME}, 也就是文件开头定义的 “red”
add_executable(${PROJECT_NAME} ${SRC_LIST} )

find_package(glfw3 3.2 REQUIRED)

# 让项目的可执行程序red链接glfw库
target_link_libraries(${PROJECT_NAME} glfw)

find_package(assimp REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARY})

# 让CMake去系统搜索OpenGL库，找不到就报错(REQUIRED).
# 找到了之后就会有两个变量可用：OPENGL_INCLUDE_DIR 和 OPENGL_gl_LIBRARY
find_package(OpenGL REQUIRED)

# 添加OpenGL头文件包含目录
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

# 让项目的可执行程序red链接系统OpenGL库
target_link_libraries(${PROJECT_NAME} ${OPENGL_gl_LIBRARY})

# GLEW
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif(GLEW_FOUND)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_custom_target(run
    COMMAND make
    COMMAND ./bin/learnopengl
    ${PROJECT_SOURCE_DIR}/build
)
