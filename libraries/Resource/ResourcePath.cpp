
#include "ResourcePath.h"
#include <string.h>

void ResourcePath::setPath(const char* path)
{
  mFullPath = mRelativePath = path;
}

bool ResourcePath::makeRelativeTo(const char* root)
{
  if (!isRelativeTo(root))
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

bool ResourcePath::isRelativeTo(const char* root)
{
  // Bug - The full root does not have to be specified
  //
  const char* rootLocation = strstr(mFullPath, root);
  return (rootLocation != NULL);
}
