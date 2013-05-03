
#ifndef _RESOURCE_COLLECTION_H_9daeee6d_4dd1_4918_9de9_5658eb715728
#define _RESOURCE_COLLECTION_H_9daeee6d_4dd1_4918_9de9_5658eb715728

#include "mDefs.h"
#include "PathName.h"
#include <string.h>

/**
 * 
 */
template<typename T>
class PathCollection
{
public: 
  class Iterator;
  class PathTree;

  typedef enum
  {
    kRootVisibility,
    kAllVisibility,
    kDefaultVisibility = kRootVisibility
  } Visibility;

  /**
   */
  class PathTree
  {
  public:
    PathTree() :
      mValue(NULL),
      mParent(NULL),
      mChild(NULL),
      mNextCousin(NULL)
    {
    }

    /**
     * @description Merges two path trees together.
     * @returns The parent node to the hierarchy.
     */
    PathTree* merge(PathTree* rhs)
    {
      mDebugAssert(rhs != NULL);
     
      PathName::Comparison relationship = mPath.compare(rhs->mPath);

      // TODO: could attempt to merge cousins and children.
      mDebugAssert(relationship != PathName::kMatches);

      if (relationship == PathName::kIsChild)
      { 
        rhs->mParent = mParent;
        return rhs->merge(this);
      } 
      else if (relationship == PathName::kIsParent)
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

          mergeCousins(rhs);
        }

        return this;
      }
    }

    /**
     * @description Finds a subtree.
     * 
     * @returns a subtree defined by 'path', or NULL if
     * no subtree is found.
     *
     * TODO: Inefficient: 
     *       1) path can be popped so that the full path isn't searched each time.
     *s
     */
    PathTree* find(const PathName& searchPath)
    {
      bool matchedWild = false;

      PathName::Comparison relationship = mPath.compare(searchPath, false, &matchedWild);

      if (relationship == PathName::kIsChild)
      {
        return NULL;
      }
      else if (relationship == PathName::kMatches)
      {
        return this;
      }
      else if (relationship == PathName::kIsParent)
      {
        return findInChildren(searchPath);
      }
      else
      {
        return findInCousins(searchPath);
      }
    }

    inline PathTree* getParent() { return mParent; }
    inline PathTree* getChild() { return mChild; }
    inline PathTree* getCousin() { return mNextCousin; }
    inline PathTree* getBeginningCousin()
    {
      if (mParent != NULL)
      {
        return mParent->mChild;
      }
      else
      {
        return this;
      }
    }

    inline void setValue(T* val) { mValue = val; }
    inline void setPath(const char* path) { mPath.setPath(path); }
    inline T* getValue() { return mValue; }
    inline const PathName& getPath() const { return mPath; }

  protected:
    /**
     */
    void mergeCousins(PathTree* rhs)
    {
      mDebugAssert(rhs != NULL);

      // Detach cousins and merge
      PathTree* cousin = rhs->mNextCousin;
      rhs->mNextCousin = NULL;

      while (cousin != NULL)
      {
        PathTree* next = cousin->mNextCousin;
        cousin->mNextCousin = NULL;
        merge(cousin);
        cousin = next;
      }
    }

    PathTree* findInCousins(const PathName& searchPath)
    {
      if (mNextCousin != NULL)
      {
        return mNextCousin->find(searchPath);
      }
      else
      {
        return NULL;
      }
    }

    PathTree* findInChildren(const PathName& searchPath)
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
    PathName mPath;
    T* mValue;
    PathTree* mParent;
    PathTree* mChild;
    PathTree* mNextCousin;
  };

  /**
   */
  class Iterator
  {
  public: 
    friend class PathCollection;

    Iterator() :
      mCurrent(NULL),
      mVisibility(kDefaultVisibility)
    {
    }

    Iterator(const PathCollection& collection)
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

    T* operator*()
    {
      return value();
    }

    T* operator->()
    {
      return value();
    }

    T* value()
    {
      mDebugAssert(mCurrent != NULL);
      return mCurrent->getValue();
    }

    const PathName* path()
    {
      mDebugAssert(mCurrent != NULL);
      return &mCurrent->getPath();
    }

  protected:
    Iterator(PathTree* root, Visibility visibility) :
      mCurrent(root),
      mVisibility(visibility)
    {
    }

  private:
    void next()
    {
      if (mCurrent == NULL)
      {
        return;
      }

      if (mVisibility == kRootVisibility)
      {
        mCurrent = NULL;
      }
      else if (mVisibility == kAllVisibility)
      {
        if (mCurrent->getChild())
        {
          mCurrent = mCurrent->getChild();
        }
        else if (mCurrent->getCousin())
        {
          mCurrent = mCurrent->getCousin();
        }
        else
        {
          if (mCurrent->getParent())
          {
            mCurrent = mCurrent->getParent()->getCousin();
          }
          else
          {
            mCurrent = NULL;
          }
        }
      }
      else 
      {
        // Mode not implemented...
        mDebugAssert(0);
      }
    }

    PathTree* mCurrent;
    Visibility mVisibility;
  };

  /**
   */
  PathCollection(
      PathTree* root = NULL,
      Visibility visibility = kAllVisibility) 
    :
    mRoot(root),
    mVisibility(visibility)
  {
  }

  PathCollection(const PathCollection& rhs) :
    mRoot(rhs.mRoot),
    mVisibility(rhs.mVisibility)
  {
  }

  Iterator begin(void) const
  {
    return Iterator(mRoot, mVisibility);
  }

  Iterator end(void) const
  {
    return Iterator();
  }

  /**
   */
  const PathCollection find(const char* path) const
  {
    if (path == NULL || mRoot == NULL)
    {
      return PathCollection();
    }

    PathTree* found = mRoot->find(path);
    if (found)
    {
      // TODO: Put this somewhere else
      bool matchedWild = false;
      found->getPath().compare(path, false, &matchedWild);

      // Use the path relationship to determine visibility
      // of search results.
      Visibility visibility = kDefaultVisibility;
      if (matchedWild)
      {
        // TODO: expand on this...
        visibility = kAllVisibility;
      }

      // Returning a const, so no need for it to have
      // an allocator.
      return PathCollection<T>(
        found, 
        visibility);
    }
    else
    {
      return PathCollection();
    }
  }

  bool empty() const
  {
    return mRoot == NULL;
  }

  bool exists(const char* path) const
  {
    PathCollection exist = find(path);
    return !exist.empty();
  }

  bool add(const char* path, T* v)
  {
    if (path == NULL || v == NULL)
    {
      return false;
    }

    if (exists(path))
    {
      return false;
    }

    PathTree* node = new PathTree();
    if (node == NULL)
    {
      return false;
    }
    
    node->setPath(path);
    node->setValue(v);

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
  Visibility mVisibility;
};

#endif
