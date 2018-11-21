//
// Created by Sidorenko Nikita on 11/21/18.
//

#include "ShaderCaps.h"

bool ShaderCapsSet::hasCap(const ShaderCaps cap) const {
  return _caps.find((int)cap) != _caps.end();
}

void ShaderCapsSet::addCap(const ShaderCaps cap) {
  _caps.insert((int)cap);
  _maskDirty = true;
}

void ShaderCapsSet::removeCap(const ShaderCaps cap) {
  _caps.erase((int)cap);
  _maskDirty = true;
}

ShaderCapsSet::Bitmask ShaderCapsSet::getBitmask() const {
  if (_maskDirty) {
    _bitmask = _calculateBitMask();
    _maskDirty = false;
  }
  return _bitmask;
}

ShaderCapsSet::Bitmask ShaderCapsSet::_calculateBitMask() const {
  Bitmask result = 0;

  for (auto cap : _caps) {
    result |= 1u << (Bitmask)cap;
  }

  return result;
}
