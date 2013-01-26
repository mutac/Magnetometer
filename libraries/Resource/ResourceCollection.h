
#ifndef _RESOURCE_COLLECTION_H_9daeee6d_4dd1_4918_9de9_5658eb715728
#define _RESOURCE_COLLECTION_H_9daeee6d_4dd1_4918_9de9_5658eb715728

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
  class Iterator;
  class PathTree;

  /**
   */
  class PathTree
  {
  public:
    friend class Iterator;

    PathTree() :
      mValue(NULL),
      mParent(NULL),
      mChild(NULL),
      mNextCousin(NULL)
    {
    }

    /**
     * @description Merges two path trees together.
     *
     * @returns The (potentially new) parent node to the hierarchy.
     */
    PathTree* merge(PathTree* rhs)
    {
      mDebugAssert(rhs != NULL);
     
      if (isChildOf(rhs))
      { 
        rhs->mParent = mParent;
        return rhs->merge(this);
      } 
      else if (rhs->isChildOf(this))
      {
        if (mChild != NULL)
        {
          mChild = mChild->merge(rhs);
          mChild->mParent = this;
        }
        else
        {
          mChild = rhs;
          mChild->mParent = this;

          // Merge cousins!?
          mergeCousins(rhs);
        }

        return this;
      }
      else
      {
        if (mNextCousin != NULL)
        {
          mNextCousin = mNextCousin->merge(rhs);
          mNextCousin->mParent = mParent;
        }
        else
        {
          mNextCousin = rhs;
          mNextCousin->mParent = mParent;

          // Merge cousins!?
          // This is incorrect. Cousins will be merged to a child, when
          // they should possibly end up next to the parent.
          mergeCousins(rhs);
        }

        return this;
      }
    }

    /**
     */
    void mergeCousins(PathTree* rhs)
    {
      mDebugAssert(rhs != NULL);

      // Detach cousins and merge
      if (rhs->mNextCousin)
      {
        PathTree* cousins = rhs->mNextCousin;
        rhs->mNextCousin = NULL;
        merge(cousins);
      }
    }

    /**
     * @see find(const ResourcePath& path)
     */
    PathTree* find(const char* path)
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
     * TODO: Inefficient: 
     *       1) path can be popped so that the full path isn't searched each time.
     *s
     */
    PathTree* find(const ResourcePath& searchPath)
    {
      // 1) it's an exact path: system.nodes.specific
      // 2) it's a parent node: system.nodes
      // 3) it's incorrect: system.doesnotexist

      if (getPath().isChildOf(searchPath))
      {
        return NULL;
      }
      else if (getPath().matches(searchPath))
      {
        return this;
      }
      else if (searchPath.isChildOf(getPath()))
      {
        return findInChildren(searchPath);
      }
      else
      {
        return findInCousins(searchPath);
      }
    }

    inline void setValue(IResource* val) { mValue = val; }
    inline void setPath(const char* path) { mPath.setPath(path); }
    inline IResource* getValue() { return mValue; }
    inline const ResourcePath& getPath() const { return mPath; }

  protected:
    inline bool isChildOf(PathTree* node) const
    {
      mDebugAssert(node != NULL);
      return mPath.isChildOf(node->getPath());
    }

    PathTree* findInCousins(const ResourcePath& searchPath)
    {
      PathTree* cousin = mNextCousin;
      while (cousin != NULL)
      {
        PathTree* match = cousin->find(searchPath);
        if (match != NULL)
        {
          return match;
        }

        cousin = cousin->mNextCousin;
      }
      return NULL;
    }

    PathTree* findInChildren(const ResourcePath& searchPath)
    {
      if (mChild != NULL)
      {
        return mChild->find(searchPath);
      }
      else
      {
        return NULL;
      }
    }

  private:
    ResourcePath mPath;
    IResource* mValue;
    PathTree* mParent;
    PathTree* mChild;
    PathTree* mNextCousin;
  };

  /**
   */
  class Iterator
  {
  public: 
    friend class ResourceCollection;

    Iterator() :
      mOrigin(NULL),
      mCurrent(NULL),
      mNext(NULL),
      mIncludeChildren(false),
      mIncludeSiblings(false)
    {
    }

    Iterator(const ResourceCollection& collection)
    {
      *this = collection.begin();
    }

    /**
     * @description prefix increment (advance the iterator)
     */
    Iterator& operator++() 
    {
      next();
      return *this;
    }

    /**
     * @description postfix increment (advance the iterator)
     */
    Iterator operator++(int)
    {
      Iterator temp(*this);
      operator++();
      return temp;
    }

    bool operator==(const Iterator& rhs)
    {
      return mCurrent == rhs.mCurrent;
    }

    bool operator!=(const Iterator& rhs)
    {
      return mCurrent != rhs.mCurrent;
    }

    IResource* operator*()
    {
      return value();
    }

    IResource* value()
    {
      mDebugAssert(mCurrent != NULL);
      return mCurrent->getValue();
    }

    const ResourcePath* path()
    {
      mDebugAssert(mCurrent != NULL);
      return &mCurrent->getPath();
    }

  protected:
    Iterator(PathTree* root) :
      mOrigin(root),
      mCurrent(root),
      mNext(NULL),
      mIncludeChildren(false),
      mIncludeSiblings(false)
    {
    }

  private:
    void next()
    {
      if (mIncludeSiblings && 
          mCurrent &&
          mCurrent->mNextCousin != NULL)
      {
        mNext = mCurrent->mNextCousin;
      }
      else if (mIncludeChildren && 
               mCurrent &&
               mCurrent->mChild != NULL)
      {
        mNext = mCurrent->mChild;
      }
      else
      {
        mNext = NULL;
      }

      mCurrent = mNext;
    }

    PathTree* mOrigin;
    PathTree* mCurrent;
    PathTree* mNext;
    bool mIncludeChildren;
    bool mIncludeSiblings;
  };

  /**
   */
  ResourceCollection(
    IAllocator<PathTree>& allocator = NullAllocator<PathTree>::instance(), 
      PathTree* root = NULL) 
    :
    mAllocator(allocator),
    mRoot(root)
  {
  }

  ResourceCollection(const ResourceCollection& rhs) :
    mAllocator(rhs.mAllocator),
    mRoot(rhs.mRoot)
  {
  }

  Iterator begin(void) const
  {
    return Iterator(mRoot);
  }

  Iterator end(void) const
  {
    return Iterator();
  }

  /**
   */
  const ResourceCollection find(const char* path) const
  {
    if (path == NULL || mRoot == NULL)
    {
      return ResourceCollection();
    }

    PathTree* found = mRoot->find(path);
    if (found)
    {
      return ResourceCollection(mAllocator, found);
    }
    else
    {
      return ResourceCollection();
    }
  }

  bool empty() const
  {
    return mRoot == NULL;
  }

  bool exists(const char* path) const
  {
    ResourceCollection exist = find(path);
    return !exist.empty();
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

    PathTree* node = mAllocator.allocate();
    if (node == NULL)
    {
      return false;
    }
    
    node->setPath(path);
    node->setValue(res);

    if (mRoot != NULL)
    {
      mRoot = mRoot->merge(node);
    }
    else
    {
      mRoot = node;
    }

    return true;
  }

private:
  PathTree* mRoot;
  IAllocator<PathTree>& mAllocator;
};

#endif
