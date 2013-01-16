
#ifndef _RESOURCE_PATH_H_
#define _RESOURCE_PATH_H_

#include <string.h>

/**
 * @description A convenience class useful for storing and comparing paths.
 */
class ResourcePath
{
public:
  ResourcePath() : 
    mFullPath(NULL),
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
  void setPath(const char* path);

  /**
   * @description Pop the front element of the path.  i.e.
   *  'something.this.that' becomes 'this.that'
   */
  void popFront();

  /**
   * @see makeRelativeTo(const char* root)
   */
  inline bool makeRelativeTo(const ResourcePath& root)
  {
    return makeRelativeTo(root.getFullPath());
  }

  /**
   * @description Make this path relative to root.  This path
   *  must be a child path to root.
   * @returns false if root is not a true root to this path.
   */
  bool makeRelativeTo(const char* root);

  /**
   * @see matches(const char* path)
   */
  inline bool matches(const ResourcePath& path) const 
  {
    return matches(path.getFullPath());
  }

  /**
   * @description Determines if this path matches path.
   * @note The evaulation takes place on the full path.
   */
  bool matches(const char* path) const;

  /**
   * @see isChildOf(const char* root)
   */
  inline bool isChildOf(const ResourcePath& root) const
  {
    return isChildOf(root.getFullPath());
  }

  /**
   * @description Evaluates of this path is a child of root.
   * @returns True if this path is a child of root
   */
  bool isChildOf(const char* root) const;

  inline const char* getFullPath() const
  {
    return mFullPath;
  }

  inline int getFullPathLength() const
  {
    if (mFullPath == NULL)
      return 0;
    else
      return strlen(mFullPath);
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
  const char* mFullPath;
  const char* mRelativePath;

  static const char sPathSeperator = '.';
  static const int sPathSeperatorLength = 1;
};

#endif

