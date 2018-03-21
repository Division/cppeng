//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_ICLEANABLE_H
#define CPPWRAPPER_ICLEANABLE_H

class ICleanable
{
public:
  virtual ~ICleanable() {}
  virtual void clean() = 0;
};

#endif //CPPWRAPPER_ICLEANABLE_H
