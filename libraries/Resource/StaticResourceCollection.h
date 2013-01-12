

#ifndef _STATIC_RESOURCE_COLLECTION_H_
#define _STATIC_RESOURCE_COLLECTION_H_

#include "IResource.h"
#include "ResourcePath.h"
#include "StaticPool.h"
#include <string.h>

/**
 * 
 */
template <int CollectionSize>
class StaticResourceCollection
{
public: 
  StaticResourceCollection() :
    mRoot(NULL)
  {
  }

  bool exists(const char* path)
  {
    if (path == NULL)
    {
      return false;
    }

    return false;
  }

  bool add(const char* path, IResource* res)
  {
    if (path == NULL || res == NULL)
    {
      return false;
    }

    return false;
  }

  // get(const char* path = NULL);

  int getCount() const { return mPool.getAllocated(); }

private:

  class ResourceContainer
  {
  public:
    ResourceContainer() :
      mResource(0),
      mNextChild(NULL),
      mNextSibling(NULL)
    {
    }

    void setResource(IResource* resource) { mResource = resource; }
    IResource* getResource() { return mResource; }
    ResourcePath& getPath() { return mPath; }

  private:
    ResourcePath mPath;
    IResource* mResource;
    ResourceContainer* mNextChild;
    ResourceContainer* mNextSibling;
  };

  ResourceContainer* mRoot;
  StaticPool<CollectionSize, ResourceContainer> mPool;
};

#endif
