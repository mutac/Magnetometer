
#include "ResourcePath.h"
#include "Diagnostics.h"
#include <string.h>

void ResourcePath::setPath(const char* path)
{
  mDiag_DebugAssert(path != NULL);

  mFullPath = mRelativePath = path;
}

void ResourcePath::popFront()
{
  mDiag_DebugAssert(mRelativePath != NULL);

  const char* next = mRelativePath;

  while (*next != '\0' && *next != sPathSeperator)
  {
    next++;
  }

  if (*next != '\0')
  {
    next += sPathSeperatorLength;
    mDiag_DebugAssert(sPathSeperatorLength == 1);
  }

  mRelativePath = next;
}

bool ResourcePath::makeRelativeTo(const char* root)
{
  mDiag_DebugAssert(root != NULL);

  if (!isChildOf(root))
  {
    return false;
  }
  
  mRelativePath = mFullPath + strlen(root);
  if (*mRelativePath != '\0')
  {
    mRelativePath += sPathSeperatorLength;
    mDiag_DebugAssert(sPathSeperatorLength == 1);
  }
  return true;
}

bool ResourcePath::matches(const char* path) const
{
  mDiag_DebugAssert(path != NULL);

  return (strcmp(getFullPath(), path) == 0);
}

bool ResourcePath::isChildOf(const char* root) const
{
  mDiag_DebugAssert(root != NULL);

  // TODO: Optimize
  //
  // Bug - The full root does not have to be specified
  //
  const char* rootLocation = strstr(getFullPath(), root);
  return (rootLocation != NULL && strcmp(rootLocation, getFullPath()) != 0);
}
