cmake_minimum_required(VERSION 3.9)
project(cppeng)

set(EXPORT_TARGETS cppeng)

message("CXX COMP " $ENV{CXX})

set(CMAKE_CXX_STANDARD 14)

set(ENGINE_SOURCES
        src/Engine.cpp
        src/EngineMain.cpp
        src/Engine.h
        src/EngineMain.h
        src/render/shader/Shader.cpp
        src/render/shader/Shader.h
        src/common/ICleanable.h
        src/common/ICleanableObserver.h
        src/Resources.cpp
        src/Resources.h
        src/system/Logging.h
        src/loader/ShaderLoader.h
        src/loader/ShaderLoader.cpp
        src/render/debug/DebugDraw.cpp
        src/render/debug/DebugDraw.h
        src/render/mesh/Mesh.cpp
        src/render/mesh/Mesh.h
        src/render/renderer/Renderer.cpp
        src/render/renderer/Renderer.h
        src/render/shader/Uniform.cpp
        src/render/shader/Uniform.h
        src/EngineGL.h
        lib/nlohmann/json.hpp
        lib/inja.hpp
        src/render/shader/ShaderGenerator.cpp
        src/render/shader/ShaderGenerator.h
        src/system/Window.cpp
        src/system/Window.h
        src/system/Input.cpp
        src/system/Input.h
        src/EngineGL.cpp
        src/loader/TextureLoader.cpp
        src/loader/TextureLoader.h
        src/render/texture/Texture.cpp
        src/render/texture/Texture.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/scene/GameObject.cpp
        src/scene/GameObject.h
        src/scene/Transform.cpp
        src/scene/Transform.h
        src/EngMath.h src/IGame.h
        src/objects/MeshObject.cpp
        src/objects/MeshObject.h
        src/render/renderer/IRenderer.h
        src/Objects.h
        src/render/material/Material.cpp
        src/render/material/Material.h
        src/render/material/MaterialBinding.h
        src/render/material/MaterialTypes.cpp
        src/render/material/MaterialTypes.h
        src/objects/Camera.cpp
        src/objects/Camera.h
        src/EngMath.cpp
        src/render/shader/UniformBinding.h
        src/loader/ModelLoader.cpp
        src/loader/ModelLoader.h
        src/resources/ModelBundle.cpp
        src/resources/ModelBundle.h
        src/loader/ModelLoaderUtils.h
        src/loader/HierarchyLoader.cpp
        src/loader/HierarchyLoader.h
        src/system/utils.h
        src/render/buffer/VertexBufferObject.cpp
        src/render/buffer/VertexBufferObject.h
        src/render/shader/UniformBufferStruct.h
        src/render/renderer/UBOManager.cpp
        src/render/renderer/UBOManager.h
        src/render/renderer/RenderOperation.h
        src/objects/LightObject.cpp
        src/objects/LightObject.h
        src/utils/MeshGeneration.cpp
        src/utils/MeshGeneration.h
        src/render/buffer/MemoryBuffer.cpp
        src/render/buffer/MemoryBuffer.h
        src/EngTypes.h
        src/render/buffer/TextureBufferObject.cpp
        src/render/buffer/TextureBufferObject.h
        src/render/texture/TextureSlot.h
        src/render/shading/LightGrid.cpp
        src/render/shading/LightGrid.h
        src/utils/Math.h
        lib/stb/stb_image.h
        src/EngImage.h
        src/EngImage.cpp
        src/objects/Terrain.cpp
        src/objects/Terrain.h
        src/render/texture/SpriteSheet.cpp
        src/render/texture/SpriteSheet.h
        src/loader/SpritesheetLoader.cpp
        src/loader/SpritesheetLoader.h
        src/objects/Projector.cpp
        src/objects/Projector.h
        src/render/renderer/SceneRenderer.cpp
        src/render/renderer/SceneRenderer.h
        src/render/renderer/RenderPass.h
        src/render/shader/ShaderCaps.h
        src/render/shader/ShaderCaps.cpp
        src/render/renderer/ICameraParamsProvider.h
        src/render/buffer/MultiVertexBufferObject.cpp src/render/buffer/MultiVertexBufferObject.h src/render/buffer/MultiVBOAddress.h src/utils/Performance.h src/utils/Performance.cpp src/render/buffer/FrameBufferObject.cpp src/render/buffer/FrameBufferObject.h src/render/effect/PostEffect.cpp src/render/effect/PostEffect.h src/render/shading/ShadowMap.cpp src/render/shading/ShadowMap.h src/render/shading/IShadowCaster.h)

add_library(cppeng ${ENGINE_SOURCES})

target_compile_definitions(cppeng PUBLIC GL_GLEXT_PROTOTYPES=1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/lib)

IF(NOT EMSCRIPTEN)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(lib/glfw)
    target_link_libraries(cppeng glfw)
    set(EXPORT_TARGETS ${EXPORT_TARGETS} glfw)
ENDIF(NOT EMSCRIPTEN)

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
    add_executable(tests tests/test_main.cpp tests/test_scene.cpp tests/catch.hpp tests/test_camera.cpp tests/test_caps.cpp)
    target_link_libraries(tests Catch)
    target_link_libraries(tests cppeng)
ENDIF(NOT EMSCRIPTEN)

# glad
set(GLAD_DIR "lib/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PUBLIC "${GLAD_DIR}/include")
target_include_directories(cppeng PUBLIC "${GLAD_DIR}/include")
target_link_libraries(cppeng "glad" "${CMAKE_DL_LIBS}")
set(EXPORT_TARGETS ${EXPORT_TARGETS} glad)

target_include_directories(cppeng PUBLIC
        ${INCLUDE_DIRS})

enable_testing(true)

export(TARGETS ${EXPORT_TARGETS} FILE EngineConfig.cmake)

