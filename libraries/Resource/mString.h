
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

  mString(const mString& other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
    *this = other;
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
    if (ensureCapacity(11 + 1))
    {
      itoa(other, mStr, 10);
    }
    else
    {
      *this = (const char*)"NaN";
    }
  }

  mString(float other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
    *this = (const char*)"NaN";
  }

  mString(double other) :
    mStr(NULL),
    mSize(0),
    mOwned(true)
  {
    *this = (const char*)"NaN";
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
    mSize = 0;
    mOwned = true;
  }

  size_t capacity() const
  {
    return mSize;
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

  bool empty() const
  {
    return mStr == NULL;
  }

  mString& operator=(char* other)
  {
    dispose();

    if (other != NULL)
    {
      if (ensureCapacity(strlen(other) + 1))
      {
        strcpy(mStr, other);
      }
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

  mString& operator=(const mString& other)
  {
    if (other.mOwned)
    {
      // Make a copy
      *this = other.mStr;
    }
    else
    {
      // Hold onto the unowned str too
      *this = (const char*)other.mStr;
    }

    return *this;
  }

  bool operator==(const mString& rhs) const
  {
    if (!empty() && !rhs.empty())
    {
      return strcmp(mStr, rhs.mStr) == 0;
    }
    else
    {
      return empty() && rhs.empty();
    }
  }

  bool operator==(const char* rhs) const
  {
    if (!empty() && rhs != NULL)
    {
      return strcmp(mStr, rhs) == 0;
    }
    else
    {
      return empty() && rhs == NULL;
    }
  }

  const char* c_Str() const
  {
    return mStr;
  }

private:
  
  bool ensureCapacity(size_t size)
  {
    if (size > mSize)
    {
      dispose();

      mStr = new char[size];
      mSize = size;
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
