cmake_minimum_required(VERSION 3.9)
project(cppeng)

message("CXX COMP " $ENV{CXX})

set(CMAKE_CXX_STANDARD 11)

add_library(cppeng src/Engine.cpp src/EngineMain.cpp include/engine/Engine.h include/engine/EngineMain.h src/render/shader/Shader.cpp src/render/shader/Shader.h src/common/ICleanable.h src/common/ICleanableObserver.h src/Resources.cpp src/Resources.h src/Logging.h src/loader/ShaderLoader.h src/loader/ShaderLoader.cpp src/render/debug/DebugDraw.cpp src/render/debug/DebugDraw.h src/render/mesh/Mesh.cpp src/render/mesh/Mesh.h src/render/renderer/Renderer.cpp src/render/renderer/Renderer.h src/render/shader/Uniform.cpp src/render/shader/Uniform.h include/engine/EngineGL.js.h lib/nlohmann/json.hpp lib/inja.hpp src/render/shader/ShaderGenerator.cpp src/render/shader/ShaderGenerator.h)

target_compile_definitions(cppeng PRIVATE GL_GLEXT_PROTOTYPES=1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(cppeng ${SDL2_LIBRARY})

target_include_directories(cppeng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(OpenGL REQUIRED)
target_link_libraries(cppeng ${OPENGL_gl_LIBRARY})

add_subdirectory(lib/glm)
include_directories(lib)

target_include_directories(cppeng PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

export(TARGETS cppeng FILE EngineConfig.cmake)
