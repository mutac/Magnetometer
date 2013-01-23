
#ifndef _RESOURCE_H_
#define _RESOURCE_H_

#include "Defs.h"
#include "IResource.h"

/**
 * 
 */
class ResourceBase : public IResource
{
public:
  virtual void set(const Variant& val) {}
  virtual Variant get() { return Variant::Empty; }
};

#endif
