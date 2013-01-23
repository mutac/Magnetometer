
#include "ResourcePath.h"
#include <string.h>

void ResourcePath::popFront()
{
  mDebugAssert(mRelativePath != NULL);

  const char* next = mRelativePath;
  while (*next != '\0' && *next != sPathSeparator)
  {
    next++;
  }

  if (*next != '\0')
  {
    // Move beyond path separator
    next++;
  }

  mRelativePath = next;
}

bool ResourcePath::makeRelativeTo(const char* root)
{
  mDebugAssert(root != NULL);
  mDebugAssert(mRelativePath != NULL);

  if (!isChildOf(root))
  {
    return false;
  }
  
  mRelativePath = mRelativePath + strlen(root);
  if (*mRelativePath != '\0')
  {
    // Move beyond path separator
    mRelativePath++;
  }
  return true;
}

bool ResourcePath::matches(const char* path) const
{
  mDebugAssert(path != NULL);
  return (strcmp(mRelativePath, path) == 0);
}

bool ResourcePath::isChildOf(const ResourcePath& parent, bool checkAbsolute) const
{
  if (checkAbsolute)
  {
    return isChildOf(parent.getAbsolutePath(), true);
  }
  else
  {
    return isChildOf(parent.getPath(), false);
  }
}

bool ResourcePath::isChildOf(const char* root, bool checkAbsolute) const
{
  mDebugAssert(root != NULL);
  mDebugAssert(mRelativePath != NULL);

  const char* checkAgainst = mRelativePath;
  if (checkAbsolute)
  {
    checkAgainst = mAbsolutePath;
  }

  const char* found = strstr(checkAgainst, root);
  return (found != NULL && found == checkAgainst);
}
