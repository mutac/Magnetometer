
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
  class Container;

  /**
   */
  class Container
  {
  public:
    friend class Iterator;

    Container() :
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
    Container* place(Container* node)
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

          Container* nextSibling = mSiblings->mSiblings;
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
    Container* find(const char* path)
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
    Container* find(const ResourcePath& searchPath)
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
        return findInSiblings(searchPath);
      }
    }

    inline void setValue(IResource* val) { mValue = val; }
    inline void setPath(const char* path) { mPath.setPath(path); }
    inline IResource* getValue() { return mValue; }
    inline const ResourcePath& getPath() const { return mPath; }

  private:
    inline bool isChildOf(Container* node) const
    {
      mDebugAssert(node != NULL);
      return mPath.isChildOf(node->getPath());
    }

    Container* findInSiblings(const ResourcePath& searchPath)
    {
      Container* sibling = mSiblings;
      while (sibling != NULL)
      {
        Container* match = sibling->find(searchPath);
        if (match != NULL)
        {
          return match;
        }

        sibling = sibling->mSiblings;
      }
      return NULL;
    }

    Container* findInChildren(const ResourcePath& searchPath)
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
    Container* mChildren;
    Container* mSiblings;
  };

  /**
   */
  class Iterator
  {
  public: 
    Iterator() :
      mRoot(NULL),
      mPosition(NULL),
      mIncludeChildren(false)
    {
    }

    Iterator(Container* root) :
      mRoot(root),
      mPosition(root),
      mIncludeChildren(false)
    {
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
      return mPosition == rhs.mPosition;
    }

    bool operator!=(const Iterator& rhs)
    {
      return mPosition != rhs.mPosition;
    }

    IResource* operator*()
    {
      return value();
    }

    IResource* value()
    {
      mDebugAssert(mPosition != NULL);
      return mPosition->getValue();
    }

    const ResourcePath* path()
    {
      mDebugAssert(mPosition != NULL);
      return &mPosition->getPath();
    }

  private:
    void next()
    {
      mDebugAssert(mPosition != NULL);
      mPosition = mPosition->mSiblings;
    }

    Container* mRoot;
    Container* mPosition;
    bool mIncludeChildren;
  };

  /**
   */
  ResourceCollection(
    IAllocator<Container>& allocator = NullAllocator<Container>::instance(), 
      Container* root = NULL) 
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
  ResourceCollection find(const char* path) const
  {
    if (path == NULL || mRoot == NULL)
    {
      return ResourceCollection();
    }

    Container* found = mRoot->find(path);
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

    Container* node = mAllocator.allocate();
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
  Container* mRoot;
  IAllocator<Container>& mAllocator;
};

#endif
