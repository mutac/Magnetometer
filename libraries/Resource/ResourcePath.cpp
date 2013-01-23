
#include "ResourcePath.h"
#include <string.h>

bool ResourcePath::popFront()
{
  mDebugAssert(mRelativePath != NULL);

  const char* next = mRelativePath;
  while (*next != '\0' && *next != sPathSeparator)
  {
    next++;
  }

  if (*next != '\0')
  {
    // Move beyond path separator, but
    // don't allow the last element to
    // be popped.
    next++;
    mRelativePath = next;
    return true;
  }
  else
  {
    return false;
  }
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

bool ResourcePath::matches(const ResourcePath& path, bool compareAbsolute) const
{
  if (compareAbsolute)
  {
    return matches(path.getAbsolutePath(), compareAbsolute);
  }
  else
  {
    return matches(path.getPath(), compareAbsolute);
  }
}

bool ResourcePath::matches(const char* path, bool compareAbsolute) const
{
  mDebugAssert(path != NULL);

  if (compareAbsolute)
  {
    mDebugAssert(mAbsolutePath != NULL);
    return (strcmp(mAbsolutePath, path) == 0);
  }
  else 
  {
    mDebugAssert(mRelativePath != NULL);
    return (strcmp(mRelativePath, path) == 0);
  }
}

bool ResourcePath::isChildOf(const ResourcePath& parent, bool compareAbsolute) const
{
  if (compareAbsolute)
  {
    return isChildOf(parent.getAbsolutePath(), compareAbsolute);
  }
  else
  {
    return isChildOf(parent.getPath(), compareAbsolute);
  }
}

bool ResourcePath::isChildOf(const char* root, bool compareAbsolute) const
{
  mDebugAssert(root != NULL);
  mDebugAssert(mRelativePath != NULL);
  mDebugAssert(mAbsolutePath != NULL);

  if (matches(root, compareAbsolute))
  {
    return false;
  }

  const char* checkAgainst = mRelativePath;
  if (compareAbsolute)
  {
    checkAgainst = mAbsolutePath;
  }

  //
  // BUG?: only a portion of the root needs to be specified.
  //       strstr is not sufficient.
  //

  const char* found = strstr(checkAgainst, root);
  return (found != NULL && found == checkAgainst);
}
