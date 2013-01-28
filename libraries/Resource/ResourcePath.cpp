
#include "ResourcePath.h"
#include <string.h>

bool ResourcePath::popFront()
{
  const char* next = popFront(mRelativePath);

  if (*next != '\0')
  {
    // Don't allow the last element to
    // be popped.
    mRelativePath = next;
    return true;
  }
  else
  {
    return false;
  }
}

bool ResourcePath::makeRelativeTo(const ResourcePath& root)
{
  mDebugAssert(root.getPath() != NULL);
  mDebugAssert(mRelativePath != NULL);

  if (!isChildOf(root))
  {
    return false;
  }
  
  mRelativePath = mRelativePath + root.getPathLength();
  if (*mRelativePath != '\0')
  {
    // Move beyond path separator
    mRelativePath++;
  }
  return true;
}

ResourcePath::Comparison ResourcePath::compare(const ResourcePath& path, bool compareAbsolute, bool* outMatchedWild) const
{
  if (compareAbsolute)
  {
    return compare(mAbsolutePath, path.mAbsolutePath, outMatchedWild);
  }
  else
  {
    return compare(mRelativePath, path.mRelativePath, outMatchedWild);
  }
}

bool ResourcePath::isChildOf(const ResourcePath& parent, bool compareAbsolute) const
{
  if (compareAbsolute)
  {
    return isChildOf(mAbsolutePath, parent.mAbsolutePath);
  }
  else
  {
    return isChildOf(mRelativePath, parent.mRelativePath);
  }
}

bool ResourcePath::isChildOf(const char* lhs, const char* rhs)
{
  mDebugAssert(lhs != NULL && rhs != NULL);

  // 
  // BUG?: only a portion of the root needs to be specified.
  //       strstr is not sufficient.
  //
  // Inefficient..
  //

  if (strcmp(lhs, rhs) == 0)
  {
    return false;
  }

  const char* found = strstr(lhs, rhs);
  return (found != NULL && found == lhs);
}

const char* ResourcePath::popFront(const char* path)
{
  mDebugAssert(path != NULL);

  while (*path != '\0' && *path != sPathSeparator)
  {
    path++;
  }

  if (*path != '\0')
  {
    // Move beyond path separator
    path++;
  }

  return path;
}

ResourcePath::Comparison ResourcePath::compare(const char* lhs, const char* rhs, bool* outMatchedWild)
{
  mDebugAssert(lhs != NULL && rhs != NULL);
  bool matchedWild = false;

  while (*lhs != '\0' &&
         *rhs != '\0')
  {
    if (*lhs != *rhs)
    {
      // Not a match
      if (*lhs == sPathWildCard ||
          *rhs == sPathWildCard)
      {
        // But it's ok, it's a wild card!
        lhs = popFront(lhs);
        rhs = popFront(rhs);
        matchedWild = true;
        // Keep going...
      }
      else
      {
        // Nope, dang...
        break;
      }
    }
    else
    {
      lhs++;
      rhs++;
    }
  }

  if (outMatchedWild)
  {
    *outMatchedWild = matchedWild;
  }

  if (*lhs == '\0' && *rhs == '\0')
  {
    return kExact;
  }
  else if (*rhs == sPathSeparator &&
           *lhs == '\0')
  {
    return kParentOf;
  }
  else if (*lhs == sPathSeparator &&
           *rhs == '\0')
  {
    return kChildOf;
  }
  else
  {
    return kUnequal;
  }
}

