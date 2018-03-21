//
// Created by Sidorenko Nikita on 1/21/18.
//

#ifndef CPPWRAPPER_ICLEANABLEOBSERVER_H
#define CPPWRAPPER_ICLEANABLEOBSERVER_H

class ICleanableObserver
{
public:
  virtual ~ICleanableObserver() {}
  virtual void notifyDirty() = 0;
};

#endif //CPPWRAPPER_ICLEANABLEOBSERVER_H
