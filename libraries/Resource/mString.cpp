
#include "mString.h"
#include <ctype.h>
#include <string.h>
#include <float.h>

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

template<>
mString to_string(const long& from)
{
  mString s;

  // This is platform specific:
  if (s.ensureCapacity(11 + 1))
  {
    ltoa(from, s.get(), 10);
  }
  else
  {
    s = "NaN";
  }

  return s;
}

template<>
mString to_string(const int& from)
{
  return to_string((long)from);
}

template<>
mString to_string(const float& from)
{
  return mString("NaN");
}

template<>
mString to_string(const double& from)
{
  return mString("NaN");
}