//
// Created by Sidorenko Nikita on 4/8/18.
//

#ifndef CPPWRAPPER_HIERARCHYLOADER_H
#define CPPWRAPPER_HIERARCHYLOADER_H

#include "resources/ModelBundle.h"
#include "objects/MeshObject.h"
#include <render/material/MaterialTypes.h>

namespace loader {

  class MaterialPicker {
  public:
    virtual MaterialPtr getMaterial(const HierarchyData *hierarchy);
  };

  GameObjectPtr loadHierarchy(ModelBundlePtr bundle, const HierarchyData *hierarchy = nullptr);

}


#endif //CPPWRAPPER_HIERARCHYLOADER_H
