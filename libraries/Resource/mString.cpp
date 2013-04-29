
#include "mString.h"
#include <ctype.h>

int mStd::strcasecmp(const char* left, const char* right)
{
  while(true)
  {
    int l = tolower((int)*left++);
    int r = tolower((int)*right++);
    if (l == 0 || l != r)
    {
      return l - r;
    }
  }
}