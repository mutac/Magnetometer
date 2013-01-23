
#ifndef _RESOURCE_COLLECTION_H_
#define _RESOURCE_COLLECTION_H_

#include "Defs.h"
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
      mChildren(NULL),
      mSiblings(NULL)
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
      mDebugAssert(node != NULL);

      if (isChildOf(node))
      { 
        //
        // re-place 'this' down inside 'node', but first re-place all of its siblings
        //

        while (mSiblings)
        {
          // Truth: 'node' is a parent to 'this', therefore any sibling of 'this' 
          // cannot be a parent to 'node'.  Thus we can ignore the return value
          // of placement.

          ResourceContainer* nextSibling = mSiblings->mSiblings;
          mSiblings->mSiblings = NULL;

          node->place(mSiblings);

          mSiblings = nextSibling;
        }

        // Truth: We have already established that 'node' is a parent to 'this',
        // thus we can can ignore the return value of placement.
        node->place(this);

        return node;
      } 
      else if (node->isChildOf(this))
      {
        if (mChildren != NULL)
        {
          mChildren = mChildren->place(node);
        }
        else
        {
          mChildren = node;
        }
        return this;
      }
      else
      {
        if (mSiblings != NULL)
        {
          mSiblings = mSiblings->place(node);
        }
        else
        {
          mSiblings = node;
        }
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
     *s
     */
    ResourceContainer* find(const ResourcePath& searchPath)
    {
      // 1) it's an exact path: system.nodes.specific
      // 2) it's a parent node: system.nodes
      // 3) it's incorrect: system.doesnotexist

      if (getPath().matches(searchPath))
      {
        // The path is exact!
        return this;
      }
      else if (getPath().isChildOf(searchPath))
      {
        // TODO: isChildOf() is true if the searchPath == thisPath.
        // That may change...
        return NULL;
      }
      else if (searchPath.isChildOf(getPath()))
      {
        return findInChildren(searchPath);
      }
      else
      {
        return findInSiblings(searchPath);
      }
    }

    inline void setValue(IResource* val) { mValue = val; }
    inline void setPath(const char* path) { mPath.setPath(path); }
    inline IResource* getValue() { return mValue; }
    inline ResourcePath& getPath() { return mPath; }

  private:
    inline bool isChildOf(ResourceContainer* node) const
    {
      mDebugAssert(node != NULL);
      return mPath.isChildOf(node->getPath());
    }

    ResourceContainer* findInSiblings(const ResourcePath& searchPath)
    {
      ResourceContainer* sibling = mSiblings;
      while (sibling != NULL)
      {
        ResourceContainer* match = sibling->find(searchPath);
        if (match != NULL)
        {
          return match;
        }

        sibling = sibling->mSiblings;
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
    ResourceContainer* mSiblings;
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

#endif
