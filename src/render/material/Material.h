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
  Material() = default;
  virtual ~Material() = default;

  const ShaderPtr shader() const { return _shader; }
  const ShaderPtr shaderSkinning() const { return _shaderSkinning; }
  const ShaderPtr shaderDepthOnly() const { return _shaderDepthOnly; }
  const ShaderPtr shaderDepthOnlySkinning() const { return _shaderDepthOnlySkinning; }
  
  bool hasSkinning() { return _bindings.hasSkinning; }
  void setSkinningMatrices(const UBOStruct::SkinningMatrices &value) { _bindings.skinningMatrices.data = value; }
  const UBOStruct::SkinningMatrices &getSkinningMatrices() { return _bindings.skinningMatrices.data; }

  void uploadBindings(ShaderPtr shader) const;
  void activateTextures() const;

protected:
  bool _supportsSkinning = true; // by default skinning is OK

  // Bindings are mostly used for the textures and fast shader debug
  // Uniforms usually stored in UBO
  int _addMat4Binding(UniformName uniform);
  int _addMat3Binding(UniformName uniform);
  int _addVec4Binding(UniformName uniform);
  int _addVec3Binding(UniformName uniform);
  int _addVec2Binding(UniformName uniform);
  int _addFloatBinding(UniformName uniform);
  int _addIntBinding(UniformName uniform);
  int _addTextureBinding(UniformName uniform);

  void _setup(ShaderCapsSetPtr caps);

  MaterialBingings _bindings;

  ShaderPtr _shader;
  ShaderPtr _shaderSkinning;
  ShaderPtr _shaderDepthOnly;
  ShaderPtr _shaderDepthOnlySkinning;
};

typedef std::shared_ptr<Material> MaterialPtr;

#endif //CPPWRAPPER_MATERIAL_H
