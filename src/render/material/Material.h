//
// Created by Sidorenko Nikita on 3/30/18.
//

#ifndef CPPWRAPPER_MATERIAL_H
#define CPPWRAPPER_MATERIAL_H

#include "MaterialBinding.h"
#include "render/shader/Shader.h"
#include "EngMath.h"
#include <memory>

class Material {
public:
  Material();
  virtual ~Material() = default;

  const ShaderPtr shader() const { return _shader; }

  bool hasTransformBlock() { return _bindings.hasTransform; }
  void setTransformBlock(UBOStruct::TransformStruct &value) { _bindings.transform.data = value; }
  const UBOStruct::TransformStruct &getTransformStruct() { return _bindings.transform.data; }

  void setView(const mat4 &viewMatrix);
  void setProjection(const mat4 &projection);
  void uploadBindings() const;
  void activateTextures() const;

protected:
  int _modelViewBinding = -1;
  int _projectionBinding = -1;
  int _normalMatrixBinding = -1;
  int _viewMatrixBinding = -1;

  int _addMat4Binding(UniformName uniform);
  int _addMat3Binding(UniformName uniform);
  int _addVec4Binding(UniformName uniform);
  int _addVec3Binding(UniformName uniform);
  int _addVec2Binding(UniformName uniform);
  int _addFloatBinding(UniformName uniform);
  int _addIntBinding(UniformName uniform);
  int _addTextureBinding(UniformName uniform);

  MaterialBingings _bindings;
  ShaderPtr _shader;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif //CPPWRAPPER_MATERIAL_H
