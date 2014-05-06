
#include "mString.h"
#include <ctype.h>
#include <string.h>
#include <float.h>

#if defined(mPlatformAvr)
#include <stdlib.h>
#else defined(mUseStl)
#include <sstream>
#include <iomanip>
#endif

namespace mStd
{
  int strcasecmp(const char* left, const char* right)
  {
    while (true)
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

#ifdef mUseStl
  template<>
  mString to_string(const float& from)
  {
    std::stringstream s;
    s << from;

    return mString(s.str().c_str());
  }
#else
  template<>
  mString to_string(const float& from)
  {
    return mString("NaN");
  }
#endif

#ifdef mUseStl
  template<>
  mString to_string(const double& from)
  {
    std::stringstream s;
    s << std::setprecision(16) << from;

    return mString(s.str().c_str());
  }
#else
  template<>
  mString to_string(const double& from)
  {
    return mString("NaN");
  }
#endif
}
