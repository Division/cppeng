//
// Created by Sidorenko Nikita on 11/20/18.
//

#ifndef CPPWRAPPER_VIEW_H
#define CPPWRAPPER_VIEW_H

#import "IRenderer.h"
#import <memory>

// Wraps a single pass with a particular camera
class View {
public:
  explicit View(bool lightingEnabled);
private:
  std::unique_ptr<IRenderer> _renderer;
};


#endif //CPPWRAPPER_VIEW_H
