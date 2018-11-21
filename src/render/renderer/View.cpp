//
// Created by Sidorenko Nikita on 11/20/18.
//

#include "View.h"
#include "Renderer.h"

View::View(bool lightingEnabled) {
   _renderer = std::make_unique<Renderer>(nullptr);
}
