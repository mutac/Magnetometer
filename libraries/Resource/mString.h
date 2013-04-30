
#ifndef _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6
#define _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6

#include "mDefs.h"
#include "mStd.h"
#include <string.h>
#include <float.h>

namespace mStd
{
  /**
   */
  int strcasecmp(const char* left, const char* right);
};

/**
 */
class mString
{
public:
  mString() :
    mStr(NULL),
    mCapacity(0)
  {
  }

  mString(const mString& other) :
    mStr(NULL),
    mCapacity(0)
  {
    *this = other;
  }

  mString(char* other) :
    mStr(NULL),
    mCapacity(0)
  {
    *this = other;
  }

  mString(const char* other) :
    mStr(NULL),
    mCapacity(0)
  {
    *this = other;
  }

  mString(int other) :
    mStr(NULL),
    mCapacity(0)
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
    mCapacity(0)
  {
    *this = (const char*)"NaN";
  }

  mString(double other) :
    mStr(NULL),
    mCapacity(0)
  {
    *this = (const char*)"NaN";
  }

  ~mString()
  {
    dispose();
  }

  void dispose()
  {
    if (isOwned())
    {
      if (mStr)
      {
        delete [] mStr;
      }
    }

    mStr = NULL;
    mCapacity = 0;
  }

  size_t capacity() const
  {
    return mCapacity;
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
    return *this;
  }

  mString& operator=(const mString& other)
  {
    if (other.isOwned())
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

  bool operator==(const mString& rhs) const
  {
    return *this == rhs.c_Str();
  }

  bool append(const char* a)
  {
    if (a != NULL)
    {
      int newLen = strlen(a) + 1;
      if (!ensureCapacity(mCapacity + newLen))
      {
        return false;
      }
      
      strcat(mStr, a);
    }

    return true;
  }

  const char* c_Str() const
  {
    return mStr;
  }
 
  inline bool isOwned() const
  {
    return mCapacity != 0;
  }

protected:


  bool ensureCapacity(size_t size)
  {
    if (size > mCapacity && size > 0)
    {
      char* newStr = new char[size];
      if (mStr != NULL && newStr != NULL)
      {
        strcpy(newStr, mStr);
      }
      else
      {
        newStr[0] = '\0';
      }

      dispose();
      
      mStr = newStr;
      mCapacity = size;
      return mStr != NULL;
    }
    else
    {
      return true;
    }
  }

  char* mStr;
  size_t mCapacity;
};

#endif // header guard
