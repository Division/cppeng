//
// Created by Sidorenko Nikita on 4/8/18.
//

#ifndef CPPWRAPPER_HIERARCHYLOADER_H
#define CPPWRAPPER_HIERARCHYLOADER_H

#include "resources/ModelBundle.h"
#include "objects/MeshObject.h"
#include "objects/SkinnedMeshObject.h"
#include <render/material/MaterialTypes.h>
#include "EngTypes.h"

namespace loader {

  class MaterialPicker {
  public:
    MaterialPicker();
    explicit MaterialPicker(MaterialPtr material);
    virtual MaterialPtr getMaterial(const HierarchyData *hierarchy) const;

  protected:
    MaterialPtr _defaultMaterial;
  };

  GameObjectPtr loadHierarchy(ModelBundlePtr bundle, const HierarchyData *hierarchy,
                              const MaterialPicker *materialPicker = nullptr);

  SkinnedMeshObjectPtr loadSkinnedMesh(ModelBundlePtr bundle, const SkinningDataPtr skinningData = nullptr);

}


#endif //CPPWRAPPER_HIERARCHYLOADER_H
