
#ifndef _RESOURCE_H_cb694b3b_069e_4bfb_b216_4e71d4f9bc54
#define _RESOURCE_H_cb694b3b_069e_4bfb_b216_4e71d4f9bc54

#include "mDefs.h"
#include "IResource.h"

/**
 * 
 */
class ResourceBase : public IResource
{
public:
  virtual void set(const Variant& val) {}
  virtual Variant get() { return Variant(); }
};

#endif
