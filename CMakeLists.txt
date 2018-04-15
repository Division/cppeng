cmake_minimum_required(VERSION 3.9)
project(cppeng)

message("CXX COMP " $ENV{CXX})

set(CMAKE_CXX_STANDARD 14)

set(ENGINE_SOURCES src/Engine.cpp src/EngineMain.cpp include/engine/Engine.h include/engine/EngineMain.h src/render/shader/Shader.cpp src/render/shader/Shader.h src/common/ICleanable.h src/common/ICleanableObserver.h src/Resources.cpp src/Resources.h src/system/Logging.h src/loader/ShaderLoader.h src/loader/ShaderLoader.cpp src/render/debug/DebugDraw.cpp src/render/debug/DebugDraw.h src/render/mesh/Mesh.cpp src/render/mesh/Mesh.h src/render/renderer/Renderer.cpp src/render/renderer/Renderer.h src/render/shader/Uniform.cpp src/render/shader/Uniform.h src/EngineGL.h lib/nlohmann/json.hpp lib/inja.hpp src/render/shader/ShaderGenerator.cpp src/render/shader/ShaderGenerator.h src/system/Window.cpp src/system/Window.h src/system/Input.cpp src/system/Input.h src/EngineGL.cpp src/loader/TextureLoader.cpp src/loader/TextureLoader.h src/render/texture/Texture.cpp src/render/texture/Texture.h src/scene/Scene.cpp src/scene/Scene.h src/scene/GameObject.cpp src/scene/GameObject.h src/scene/Transform.cpp src/scene/Transform.h src/EngMath.h src/IGame.h src/objects/MeshObject.cpp src/objects/MeshObject.h src/render/renderer/IRenderer.h include/engine/Objects.h src/render/material/Material.cpp src/render/material/Material.h src/render/material/MaterialBinding.h src/render/material/MaterialTypes.cpp src/render/material/MaterialTypes.h src/objects/Camera.cpp src/objects/Camera.h src/EngMath.cpp src/render/renderer/GlobalState.cpp src/render/renderer/GlobalState.h src/render/shader/UniformBinding.h src/loader/ModelLoader.cpp src/loader/ModelLoader.h src/resources/ModelBundle.cpp src/resources/ModelBundle.h src/loader/ModelLoaderUtils.h src/loader/HierarchyLoader.cpp src/loader/HierarchyLoader.h src/system/utils.h src/render/buffer/VertexBufferObject.cpp src/render/buffer/VertexBufferObject.h src/render/buffer/UniformBufferObject.cpp src/render/buffer/UniformBufferObject.h src/render/shader/UniformBufferStruct.h src/render/renderer/UBOManager.cpp src/render/renderer/UBOManager.h src/render/renderer/RenderOperation.h)
add_library(cppeng ${ENGINE_SOURCES})

target_compile_definitions(cppeng PUBLIC GL_GLEXT_PROTOTYPES=1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#
# When building on Mac OS, SDL2.framework and SDL2_image.framework should exist in the /Library/Frameworks/ directory
#

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
target_link_libraries(cppeng ${SDL2_LIBRARY})

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
target_link_libraries(cppeng ${SDL2_IMAGE_LIBRARY})

#target_include_directories(cppeng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

find_package(OpenGL REQUIRED)
target_link_libraries(cppeng ${OPENGL_gl_LIBRARY})

add_subdirectory(lib/glm)
include_directories(lib)

# Unit testing
# Prepare "Catch" library for other executables

IF(NOT EMSCRIPTEN)
    set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    add_library(Catch INTERFACE)
    target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})
    add_executable(tests tests/test_main.cpp tests/test_scene.cpp tests/catch.hpp tests/test_camera.cpp)
    target_link_libraries(tests Catch)
    target_link_libraries(tests cppeng)
ENDIF(NOT EMSCRIPTEN)

target_include_directories(cppeng PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        ${SDL2_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/lib)

enable_testing(true)

export(TARGETS cppeng FILE EngineConfig.cmake)

