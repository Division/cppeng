cmake_minimum_required(VERSION 3.9)
project(engine)

message("CXX COMP " $ENV{CXX})

set(CMAKE_CXX_STANDARD 11)

add_library(engine src/Engine.cpp src/render/Mesh.cpp src/render/Mesh.h src/EngineMain.cpp include/engine/Engine.h include/engine/EngineMain.h)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(engine ${SDL2_LIBRARY})

target_include_directories(engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(OpenGL REQUIRED)
target_link_libraries(engine ${OPENGL_gl_LIBRARY})

target_include_directories(engine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

export(TARGETS engine FILE EngineConfig.cmake)
