
#ifndef _RESOURCE_COLLECTION_H_
#define _RESOURCE_COLLECTION_H_

#include "IResource.h"
#include "IAllocator.h"
#include "ResourcePath.h"
#include <string.h>

/**
 * 
 */
class ResourceCollection
{
public: 
  /**
   */
  class ResourceContainer
  {
  public:
    ResourceContainer() :
      mResource(0),
      mChild(NULL),
      mNextSibling(NULL)
    {
    }

    void place(ResourceContainer* node)
    {
      if (node == NULL)
      {
        return;
      }

      if (node->isChildOf(this))
      {
        addChild(node);
      }
      else
      {
        addSibling(node);
      }
    }

    void set(const char* path, IResource* resource) 
    {
      mResource = resource;
      mPath.setPath(path);
    }

    bool isChildOf(ResourceContainer* node)
    {
      if (node == NULL)
      {
        return false;
      }

      return mPath.isChildOf(node->getPath());
    }

    IResource* getResource() { return mResource; }
    ResourcePath& getPath() { return mPath; }

  private:
    void addChild(ResourceContainer* node)
    {
      //
      // Bug - This wont always work.. 
      // Need to reorganize if adding a new parent
      //
      if (mChild == NULL)
      {
        mChild = node;
      }
      else
      {
        if (mChild->isChildOf(node))
        {
          // Swap children
          node->place(mChild);
          mChild = node;
        }
        else
        {
          mChild->place(node);
        }
      }
    }

    void addSibling(ResourceContainer* node)
    {
      ResourceContainer* temp = mNextSibling;
      mNextSibling = node;
      node->mNextSibling = temp;
    }

    ResourcePath mPath;
    IResource* mResource;
    ResourceContainer* mChild;
    ResourceContainer* mNextSibling;
  };

  /**
   */
  ResourceCollection(IAllocator<ResourceContainer>& allocator) :
    mRoot(NULL),
    mAllocator(allocator)
  {
  }

  bool exists(const char* path) const
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

    if (exists(path))
    {
      return false;
    }

    ResourceContainer* node = mAllocator.allocate();
    if (node == NULL)
    {
      return false;
    }
    
    node->set(path, res);

    if (mRoot == NULL)
    {
      mRoot = node;
    }
    else
    {
      if (mRoot->isChildOf(node))
      {
        // Swap roots
        node->place(mRoot);
        mRoot = node;
      }
      else
      {
        mRoot->place(node);
      }
    }

    return true;
  }

  // get(const char* path = NULL);

private:

  ResourceContainer* mRoot;
  IAllocator<ResourceContainer>& mAllocator;
};

/**
 * Example
 * 
 */

/*
class TemperatureSensorResource : public ResourceBase
{
public:
private:
};

void Test()
{
  TemperatureSensorResource tempSensor;
  ResourceCollection system;
  
  system.add("system.sensors.temperature", tempSensor);
  
  ResourceCollection sensors = system.get("system.sensors");
  for (int idx = 0; idx < sensors.count(), idx++)
  {
    
  }
}
*/

#endif
