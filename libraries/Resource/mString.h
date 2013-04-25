
#ifndef _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6
#define _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6

#include "mDefs.h"
#include <string.h>
#include <float.h>

/**
 */
class mString
{
public:
  mString() :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
  }

  mString(char* other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
    *this = other;
  }

  mString(const char* other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
    *this = other;
  }

  mString(int other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
    if (ensureSize(11 + 1))
    {
      itoa(other, mStr, 10);
    }
  }

  mString(float other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
  }

  mString(double other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
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
  
  bool ensureSize(size_t size)
  {
    if (size > mSize)
    {
      dispose();

      mStr = new char[size];
      mOwned = true;
      return mStr != NULL;
    }
    else
    {
      return true;
    }
  }

  char* mStr;
  size_t mSize;
  bool mOwned;
};

#endif // header guard
