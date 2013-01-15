
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
      mValue(NULL),
      mChild(NULL),
      mNextSibling(NULL)
    {
    }

    /**
     * @description Adds the resource container at the correct
     * place within the path hierarchy.
     *
     * @returns The parent node to the hierarchy.
     */
    ResourceContainer* place(ResourceContainer* node)
    {
      if (node == NULL)
      {
        return this;
      } 

      if (isChildOf(node))
      {
        // 'node' is actually a parent to 'this',
        // so re-order and return the new parent.
        node->place(this);
        return node;
      }
      else if (node->isChildOf(this))
      {
        addChild(node);
        return this;
      }
      else
      {
        addSibling(node);
        return this;
      }
    }

    /**
     * @see find(const ResourcePath& path)
     */
    ResourceContainer* find(const char* path)
    {
      ResourcePath resPath(path);
      return find(resPath);
    }

    /**
     * @description Finds a subsection of the tree.
     * 
     * @returns a subtree defined by 'path', or NULL if
     * no subtree is found.
     *
     * TODO: Ineffiecient: 
     *       1) path can be poped so that the full path isn't searched each time.
     */
    ResourceContainer* find(const ResourcePath& searchPath)
    {
      ResourceContainer* found = NULL;
      // 1) it's an exact path: system.nodes.specific
      // 2) it's a parent node: system.nodes
      // 3) it's incorrect: system.doesnotexist

      if (getPath().matches(searchPath))
      {
        // The path is exact!
        return this;
      }

      found = findInSiblings(searchPath);
      if (found != NULL)
      {
        return found;
      }

      found = findInChildren(searchPath);
      if (found != NULL)
      {
        return found;
      }

      return NULL;
    }

    inline bool isChildOf(ResourceContainer* node) const
    {
      return mPath.isChildOf(node->getPath());
    }

    inline bool isChildOf(const char* path) const
    {
      return mPath.isChildOf(path);
    }

    inline void setValue(IResource* val) { mValue = val; }
    inline void setPath(const char* path) { mPath.setPath(path); }
    inline IResource* getValue() { return mValue; }
    inline ResourcePath& getPath() { return mPath; }

  private:
    void addChild(ResourceContainer* node)
    {
      //ResourceContainer* temp = mNextChild;
      //mNextChild = mNextChild->place(node);
    }

    void addSibling(ResourceContainer* node)
    {
      //
      // TODO: This is incorrect. Possibly add to the child of a sibling
      //
      ResourceContainer* temp = mNextSibling;
      mNextSibling = node;
      node->mNextSibling = temp;
    }

    ResourceContainer* findInSiblings(const ResourcePath& searchPath)
    {
      ResourceContainer* sibling = mNextSibling;
      while (sibling != NULL)
      {
        if (sibling->getPath().matches(searchPath))
        {
          return sibling;
        }

        sibling = sibling->mNextSibling;
      }
      return NULL;
    }

    ResourceContainer* findInChildren(const ResourcePath& searchPath)
    {
      ResourceContainer* child = mNextChild;
      while (child != NULL)
      {
        ResourceContainer* found = child->find(searchPath);
        if (found != NULL)
        {
          return found;
        }
      }
      return NULL;
    }

    ResourcePath mPath;
    IResource* mValue;
    ResourceContainer* mNextChild;
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
    if (mRoot == NULL)
    {
      return false;
    }

    ResourceContainer* subTree = mRoot->find(path);
    return (subTree != NULL);
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
    
    node->setPath(path);
    node->setValue(res);

    if (mRoot == NULL)
    {
      mRoot = node;
    }
    else
    {
      mRoot = mRoot->place(node);
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
