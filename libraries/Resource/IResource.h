
#ifndef _IRESOURCE_H_
#define _IRESOURCE_H_

#include "Variant.h"

/**
 * 
 */
class IResource
{
public:
  virtual void set(const Variant& val) = 0;
  virtual Variant get() = 0;
};

#endif
