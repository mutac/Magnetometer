
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

  bool makeRelativeTo(const ResourcePath& root)
  {
    return makeRelativeTo(root.getFullPath());
  }

  bool makeRelativeTo(const char* root);
  bool isRelativeTo(const ResourcePath& root)
  {
    return isRelativeTo(root.getFullPath());
  }

  bool isRelativeTo(const char* root);

  const char* getFullPath() const
  {
    return mFullPath;
  }

  int getFullPathLength() const
  {
    if (mFullPath == NULL)
      return 0;
    else
      return strlen(mFullPath);
  }

  const char* getPath() const
  {
    return mRelativePath;
  }

  int getPathLength() const
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

