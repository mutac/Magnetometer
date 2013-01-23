
#ifndef _RESOURCE_PATH_H_
#define _RESOURCE_PATH_H_

#include "Defs.h"
#include <string.h>

/**
 * @description A convenience class useful for storing and comparing paths.
 */
class ResourcePath
{
public:
  ResourcePath() : 
    mAbsolutePath(NULL),
    mRelativePath(NULL)
  {
  }

  ResourcePath(const char* path)
  {
    setPath(path);
  }

  ~ResourcePath() {}

  /**
   * @description Set path (resets relative path as well)
   */
  inline void setPath(const char* path)
  {
    mDebugAssert(path != NULL);
    mAbsolutePath = mRelativePath = path;
  }

  /**
   * @description Resets a path to its full path
   */
  inline void resetPath()
  {
    mRelativePath = mAbsolutePath;
  }

  /**
   * @see makeRelativeTo(const char* root)
   */
  inline bool makeRelativeTo(const ResourcePath& root)
  {
    return makeRelativeTo(root.getPath());
  }

  /**
   * @description Make this path relative to root.  This path must be a child path to root.
   * @returns false if root is not a true root to this path.
   */
  bool makeRelativeTo(const char* root);

  /**
   * @description Pop the front element of the path.  i.e.
   *  'something.this.that' becomes 'this.that'
   */
  void popFront();

  /**
   * @see matches(const char* path)
   */
  inline bool matches(const ResourcePath& path) const 
  {
    return matches(path.getPath());
  }

  /**
   * @description Determines if this path matches path.
   * @note The evaulation takes place on the full path.
   */
  bool matches(const char* path) const;

  /**
   * @see isChildOf(const char* root)
   * @param root The root path to check
   * @param checkAbsolute use the absolute path from both root and 'this' to evalute child/parent relationship.
   */
  bool isChildOf(const ResourcePath& parent, bool checkAbsolute = false) const;

  /**
   * @description Evaluates of this path is a child of root.
   * @param root The root path to check
   * @param checkAbsolute  When evaluating child/parent relationship, check the absolute path rather than the relative path.
   * @returns True if this path is a child of root.
   */
  bool isChildOf(const char* parent, bool checkAbsolute = false) const;

  inline const char* getAbsolutePath() const
  {
    return mAbsolutePath;
  }

  inline int getAbsolutePathLength() const
  {
    if (mAbsolutePath == NULL)
      return 0;
    else
      return strlen(mAbsolutePath);
  }

  inline const char* getPath() const
  {
    return mRelativePath;
  }

  inline int getPathLength() const
  {
    if (mRelativePath == NULL)
      return 0;
    else
      return strlen(mRelativePath);
  }

private:
  const char* mAbsolutePath;
  const char* mRelativePath;

  static const char sPathSeparator = '.';
};

#endif

