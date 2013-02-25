
#ifndef _IRESOURCE_H_e8f244c5_1eeb_4c32_8c07_1e0d3bc173e4
#define _IRESOURCE_H_e8f244c5_1eeb_4c32_8c07_1e0d3bc173e4

#include "mDefs.h"
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
