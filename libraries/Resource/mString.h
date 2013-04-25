
#ifndef _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6
#define _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6

#include "mDefs.h"
#include <string.h>

/**
 */
class mString
{
public:
  mString() :
    mStr(NULL),
    mOwned(true)
  {
  }

  mString(char* other) :
    mStr(NULL),
    mOwned(true)
  {
    *this = other;
  }

  mString(const char* other) :
    mStr(NULL),
    mOwned(true)
  {
    *this = other;
  }

  ~mString()
  {
    dispose();
  }

  void dispose()
  {
    if (mOwned)
    {
      if (mStr)
      {
        delete [] mStr;
      }
    }

    mStr = NULL;
    mOwned = true;
  }

  int length() const
  {
    if (mStr != NULL)
    {
      return strlen(mStr);
    }
    else
    {
      return 0;
    }
  }

  mString& operator=(char* other)
  {
    dispose();

    mStr = new char[strlen(other) + 1];
    if (mStr != NULL)
    {
      strcpy(mStr, other);
    }

    return *this;
  }

  mString& operator=(const char* other)
  {
    dispose();

    mStr = const_cast<char*>(other);
    mOwned = false;

    return *this;
  }

  const char* c_Str() const
  {
    return mStr;
  }

private:
  char* mStr;
  bool mOwned;
};

#endif // header guard
