//
// Created by Sidorenko Nikita on 11/21/18.
//

#ifndef CPPWRAPPER_SHADERCAPS_H
#define CPPWRAPPER_SHADERCAPS_H

#include <memory>
#include <unordered_set>

//---------------------
// Shader Caps

enum class ShaderCaps : int {
  Color = 0,
  ObjectData,
  VertexColor,
  PointSize,
  Lighting,
  Texture0,
  Texture1,
  NormalMap,
  SpecularMap,
  TerrainLayer0,
  TerrainLayer1,
  TerrainLayer2,
  ProjectedTexture,
  Skinning,
  Count
};

class ShaderCapsSet {
public:
  typedef unsigned int Bitmask;

  bool hasCap(ShaderCaps cap) const;
  void addCap (ShaderCaps cap);
  void removeCap (ShaderCaps cap);
  Bitmask getBitmask() const;
  const std::unordered_set<unsigned int> &caps() { return _caps; }

private:
  mutable Bitmask _bitmask = 0; // cached bitmask
  mutable bool _maskDirty = false;
  std::unordered_set<unsigned int> _caps;

private:
  Bitmask _calculateBitMask() const;
};

typedef std::shared_ptr<ShaderCapsSet> ShaderCapsSetPtr;

#endif //CPPWRAPPER_SHADERCAPS_H
