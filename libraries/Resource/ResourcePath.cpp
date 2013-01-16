
#include "ResourcePath.h"
#include <string.h>

void ResourcePath::setPath(const char* path)
{
  mFullPath = mRelativePath = path;
}

void ResourcePath::popFront()
{
  const char* next = mRelativePath;

  while (*next && *next != sPathSeperator)
  {
    *next++;
  }

  if (*next)
  {
    next++;
  }

  mRelativePath = next;
}

bool ResourcePath::makeRelativeTo(const char* root)
{
  if (!isChildOf(root))
  {
    return false;
  }

  // Special case, don't allow the path to be made relative
  // to itself.
  if (strcmp(mFullPath, root) == 0)
  {
    return false;
  }
  
  mRelativePath = mFullPath + strlen(root) + 
    sPathSeperatorLength;
  return true;
}

bool ResourcePath::matches(const char* path) const
{
  return (strcmp(getFullPath(), path) == 0);
}

bool ResourcePath::isChildOf(const char* root) const
{
  // Bug - The full root does not have to be specified
  //
  const char* rootLocation = strstr(getFullPath(), root);
  return (rootLocation != NULL);
}
