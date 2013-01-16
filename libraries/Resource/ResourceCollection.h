
#ifndef _RESOURCE_COLLECTION_H_
#define _RESOURCE_COLLECTION_H_

#include "IResource.h"
#include "IAllocator.h"
#include "ResourcePath.h"
#include "Diagnostics.h"
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
      mChildren(NULL),
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
        // Special case:
        //   node is actually a parent to this,
        //   so re-order and return the new parent.
        //
        node->place(this);
        return node;
      }
      else if (node->isChildOf(this))
      {
        addToChildren(node);
        return this;
      }
      else
      {
        addToSiblings(node);
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
    void addToChildren(ResourceContainer* node)
    {
      if (mChildren != NULL)
      {
        mChildren = mChildren->place(node);
      }
      else
      {
        mChildren = node;
      }
    }

    void addToSiblings(ResourceContainer* node)
    {
      ResourceContainer* sibling = mNextSibling;
      while (sibling != NULL)
      {
        if (node->isChildOf(sibling))
        {
          ResourceContainer* checkRoot = sibling->place(node);
          
          // Placement in this case should never result in a
          // new root.
          mDiag_DebugAssert(checkRoot == sibling);
          (void)checkRoot;

          return;
        }

        sibling = sibling->mNextSibling;
      }

      //
      // Not placed as a child of any sibling, so place it
      // as a sibling.
      //
      node->mNextSibling = mNextSibling;
      mNextSibling = node;
    }

    ResourceContainer* findInSiblings(const ResourcePath& searchPath)
    {
      ResourceContainer* sibling = mNextSibling;
      while (sibling != NULL)
      {
        ResourceContainer* match = sibling->find(searchPath);
        if (match != NULL)
        {
          return match;
        }

        sibling = sibling->mNextSibling;
      }
      return NULL;
    }

    ResourceContainer* findInChildren(const ResourcePath& searchPath)
    {
      if (mChildren != NULL)
      {
        return mChildren->find(searchPath);
      }
      else
      {
        return NULL;
      }
    }

    ResourcePath mPath;
    IResource* mValue;
    ResourceContainer* mChildren;
    ResourceContainer* mNextSibling;
  };

  /**
   */
  ResourceCollection(IAllocator<ResourceContainer>& allocator) :
    mRoot(NULL),
    mAllocator(allocator)
  {
  }

  ResourceContainer* find(const char* path) const
  {
    if (path == NULL)
    {
      return NULL;
    }
    if (mRoot == NULL)
    {
      return NULL;
    }

    return mRoot->find(path);
  }

  bool exists(const char* path) const
  {
    return find(path) != NULL;
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

    if (mRoot != NULL)
    {
      mRoot = mRoot->place(node);
    }
    else
    {
      mRoot = node;
    }

    return true;
  }

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
