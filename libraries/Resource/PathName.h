
#ifndef _RESOURCE_PATH_H_e8e60196_748d_4dd1_bc85_e6a5a4856cdd
#define _RESOURCE_PATH_H_e8e60196_748d_4dd1_bc85_e6a5a4856cdd

#include "Defs.h"
#include <string.h>

/**
 * @description A convenience class useful for storing and comparing paths.
 */
class PathName
{
public:

  typedef enum
  {
    kUnequal,
    kMatches,
    kIsChild,
    kIsParent
  } Comparison;

  PathName() : 
    mAbsolutePath(NULL),
    mRelativePath(NULL)
  {
  }

  PathName(const char* path)
  {
    setPath(path);
  }

  PathName(const PathName& rhs) :
    mAbsolutePath(rhs.mAbsolutePath),
    mRelativePath(rhs.mRelativePath)
  {
  }

  ~PathName() {}

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
   * @description Make this path relative to root.  This path must be a child path to root.
   * @returns false if root is not a true root to this path.
   */
  bool makeRelativeTo(const PathName& root);

  /**
   * @description Pop the front element of the path.  i.e.
   *  'something.this.that' becomes 'this.that'.  The last
   *  path element cannot be popped.
   */
  bool popFront();

  /**
   */
  inline bool operator==(const PathName& rhs) const
  {
    return matches(rhs);
  }

  /**
   */
  inline bool operator!=(const PathName& rhs) const
  {
    return !matches(rhs);
  }

  /**
   */
  inline bool operator<(const PathName& rhs) const
  {
    return strcmp(mRelativePath, rhs.mRelativePath) < 0;
  }

  /**
   */
  inline bool matches(const PathName& rhs) const
  {
    return compare(rhs) == kMatches;
  }
 
  /**
   */
  inline bool isChildOf(const PathName& rhs) const
  {
    return compare(rhs) == kIsChild;
  }

  /**
   */
  inline bool isParentOf(const PathName& rhs) const
  {
    return compare(rhs) == kIsParent;
  }

  /**
   */
  Comparison compare(const PathName& path, bool compareAbsolute = false, bool* outMatchedWild = NULL) const;

  /**
   */
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
  /**
   */
  static Comparison sCompare(const char* lhs, const char* rhs, bool* matchedByWild);

  /**
   */
  static const char* sPopFront(const char* path);

  const char* mAbsolutePath;
  const char* mRelativePath;

  static const char sPathSeparator = '.';
  static const char sPathWildCard = '*';
};

#endif

