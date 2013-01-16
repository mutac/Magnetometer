
#ifndef _RESOURCE_PATH_H_
#define _RESOURCE_PATH_H_

#include <string.h>

class ResourcePath
{
public:
  /**
   */
  ResourcePath() : 
    mFullPath(NULL),
    mRelativePath(NULL)
  {
  }

  /**
   */
  ResourcePath(const char* path)
  {
    setPath(path);
  }

  /**
   */
  ~ResourcePath()
  {
  }

  void setPath(const char* path);

  /**
   * @description Pop the front element of the path.  i.e.
   *  'something.this.that' becomes 'this.that'
   */
  void popFront();

  inline bool makeRelativeTo(const ResourcePath& root)
  {
    return makeRelativeTo(root.getFullPath());
  }

  bool makeRelativeTo(const char* root);
  inline bool matches(const ResourcePath& path) const 
  {
    return matches(path.getFullPath());
  }

  bool matches(const char* path) const;
  inline bool isChildOf(const ResourcePath& root) const
  {
    return isChildOf(root.getFullPath());
  }

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

