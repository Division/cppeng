//
// Created by Sidorenko Nikita on 2018-12-13.
//

#include "SkinnedMeshObject.h"
#include "loader/HierarchyLoader.h"
#include "EngineMain.h"
#include <iostream>

void SkinnedMeshObject::setSkinningData(ModelBundlePtr bundle, SkinningDataPtr skinningData) {
  if (_rootJoint) {
    DestroyGameObject(_rootJoint);
    _rootJoint = nullptr;
  }

  _skinningData = skinningData;

  _jointList.clear();
  _jointMap.clear();

  if (_skinningData) {
    _rootJoint = loader::loadHierarchy(bundle, _skinningData->joints);
    _rootJoint->transform()->parent(transform());

    if (_rootJoint->animation()->hasAnimation()) {
      _animation = _rootJoint->animation();
    }

    auto processJoint = [&](TransformPtr transform) {
      if (!_animation->hasAnimation() && transform->gameObject()->animation()->hasAnimation()) {
        _animation = transform->gameObject()->animation();
      }

      _jointMap[transform->gameObject()->sid()] = transform->gameObject();
      transform->gameObject()->animation()->autoUpdate(false);
      _rootJoint->animation()->addChildController(transform->gameObject()->animation());
    };

    _animation->autoUpdate(false);
    _jointMap[_rootJoint->name()] = _rootJoint;
    _rootJoint->transform()->forEachChild(true, processJoint);

    for (auto &jointName : _skinningData->jointNames) {
      _jointList.push_back(_jointMap.at(jointName));
    }

    _matrices.resize(_jointList.size());
  } else {
    _animation = std::make_shared<AnimationController>();
  }
}

void SkinnedMeshObject::_processAnimations(float dt) {
  _animation->update(dt);
}

void SkinnedMeshObject::start() {
  animation()->play("default", true);
}

void SkinnedMeshObject::postUpdate() {
  for (int i = 0; i < _jointList.size(); i++) {
    _matrices[i] = _jointList[i]->transform()->worldMatrix() * _skinningData->bindPoses[i];
  }

//  for (int i = 0; i < _mesh->vertexCount(); i++) {
//    vec3 vertex = _mesh->getVertex(i);
//    vec4 weight = _mesh->getWeights(i);
//    vec4 jointIndices = _mesh->getJointIndices(i);
//
//    mat4 matrix = _matrices[jointIndices.x] * weight.x + _matrices[jointIndices.y] * weight.y + _matrices[jointIndices.z] * weight.z;
//    vertex = vec3(matrix * vec4(vertex, 1));
//    getEngine()->debugDraw()->drawPoint(vertex, vec3(1, 1, 1));
//  }
//

//  transform()->forEachChild(true, [&](TransformPtr transform) {
//    if (transform->parent()->gameObject()->id() == this->id()) { return; }
//    getEngine()->debugDraw()->drawLine(transform->worldPosition(), transform->parent()->worldPosition(), vec4(1, 0, 0, 1));
//  });
}

void SkinnedMeshObject::render(IRenderer &renderer) {
  if (!_mesh || !_material) {
    return;
  }

  UBOStruct::SkinningMatrices skinningMatrices;
  memcpy(&skinningMatrices.matrices[0], &_matrices[0], sizeof(mat4) * _matrices.size());
  _material->setSkinningMatrices(skinningMatrices);

  RenderOperation rop;
  rop.mesh = _mesh;
  rop.material = _material;
  rop.modelMatrix = transform()->worldMatrix();
  rop.renderOrder = renderOrder;
  rop.debugInfo = name();
  rop.isSkinning = true;
  renderer.addRenderOperation(rop, _renderQueue);
}