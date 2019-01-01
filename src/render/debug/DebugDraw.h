//
// Created by Sidorenko Nikita on 2/15/18.
//

#ifndef CPPWRAPPER_DEBUGDRAW_H
#define CPPWRAPPER_DEBUGDRAW_H

#include "EngMath.h"
#include <vector>
#include <memory>
#include "render/renderer/IRenderer.h"
#include "EngTypes.h"
#include "render/material/Material.h"
#include "render/shader/UniformBufferStruct.h"

class Mesh;
class MaterialDebug;

class DebugDraw {
public:
  DebugDraw();
  void drawLine(const vec3 &p1, const vec3 &p2, const vec4 &color);
  void drawPoint(const vec3 &p, const vec3 &color, float size = 5.0f);
  void drawFrustum(mat4 viewProjection, vec4 color = vec4(1, 0, 0, 1));
  void drawAABB(const vec3 &min, const  vec3 &max, const vec4 &color);
  void drawAABB(const AABB &bounds, const vec4 &color);
  void drawOBB(const OBB &bounds, const vec4 &color);
  void drawImage(TexturePtr texture, vec4 bounds);
  void drawDepthImage(TexturePtr texture, vec4 bounds, vec2 nearFar = vec2(0,0));

  void render(std::shared_ptr<IRenderer> &renderer);

private:
  class DebugTextureMaterial : public Material {
  public:
    DebugTextureMaterial();
    void setShader(ShaderPtr shader) { _shader = shader; };
    void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
    TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }
  private:
    int _texture0Binding = -1;
  };

  class DebugDepthMapMaterial : public Material {
  public:
    DebugDepthMapMaterial();
    void setShader(ShaderPtr shader) { _shader = shader; };
    void texture(TexturePtr texture) { _bindings.textureBindings[_texture0Binding].texture = texture; }
    TexturePtr texture() const { return _bindings.textureBindings[_texture0Binding].texture; }
    void setNearFar(vec2 nearFar) { _bindings.vec2Bindings[_nearFarBinding].v = nearFar; }
  private:
    int _texture0Binding = -1;
    int _nearFarBinding = -1;
  };

  struct Image {
    explicit Image(vec4 bounds = vec4()) : material(std::make_shared<DebugTextureMaterial>()), bounds(bounds) {}
    std::shared_ptr<DebugTextureMaterial> material;
    vec4 bounds;
    UBOStruct::ObjectParams renderParams;
  };

  struct DepthMapImage {
    explicit DepthMapImage(vec4 bounds = vec4()) : material(std::make_shared<DebugDepthMapMaterial>()), bounds(bounds) {}
    std::shared_ptr<DebugDepthMapMaterial> material;
    vec4 bounds;
    UBOStruct::ObjectParams renderParams;
  };

  ShaderPtr _textureShader;
  ShaderPtr _depthMapShader;

private:
  int _currentMeshIndex = 0;
  std::vector<vec3> _lines;
  std::vector<vec3> _points;
  std::vector<vec4> _lineColors;
  std::vector<vec4> _pointColors;

  int _imageCount = 0;
  std::vector<Image> _images;
  int _depthMapImageCount = 0;
  std::vector<DepthMapImage> _depthMapImages;

  std::shared_ptr<Mesh> _quadMesh;
  std::shared_ptr<Mesh> _lineMeshes[2]; // two meshes to swap
  std::shared_ptr<Mesh> _pointMeshes[2];
  std::shared_ptr<MaterialDebug> _material;
  UBOStruct::ObjectParams _renderParams;
};


#endif //CPPWRAPPER_DEBUGDRAW_H
