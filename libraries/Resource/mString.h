
#ifndef _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6
#define _STRING_H_9385b533_444a_4cd8_8938_5caaee6127d6

#include "mDefs.h"
#include "mStd.h"
#include "SharedPointer.h"
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

  /**
   * A string character/token iterator
   */
  class Iterator
  {
  public:
    friend class mString;

    Iterator() :
      mPos(NULL),
      mToken(NULL),
      mTokLen(0)
    {
    }

    Iterator& operator++()
    {
      next();
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator temp(*this);
      next();
      return temp;
    }

    bool operator==(const Iterator& rhs)
    {
      return mPos == rhs.mPos;
    }

    bool operator!=(const Iterator& rhs)
    {
      return mPos != rhs.mPos;
    }

    mString operator*()
    {
      mString part((char*)mPos);

      // HACKKKKK
      char* nextPart = const_cast<char*>(part.find(mToken));
      if (nextPart != NULL)
      {
        *nextPart = '\0';
      }

      return part;
    }

  private:
    Iterator(const mString& str, const char* token) :
      mPos(str.c_Str()),
      mToken(token),
      mTokLen(strlen(token))
    {
    }

    /**
     * Advances iterator to the next token.
     */
    void next()
    {
      if (mPos != NULL)
      {
        if (!emptyToken())
        {
          mPos = strstr(mPos, mToken);
          if (mPos != NULL)
          {
            mPos += mTokLen;
          }
        }
        else
        {
          mPos++;
        }
      }

      // Normalize end() conditions
      if (mPos != NULL && *mPos == '\0')
      {
        mPos = NULL;
      }
    }

    /**
     * Returns true if the token is... empty!
     */
    bool emptyToken() const
    {
      return mToken == NULL || *mToken == '\0';
    }

    const char* mPos;
    const char* mToken;
    size_t mTokLen;
  };

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
    // This is platform specific:
    if (ensureCapacity(11 + 1))
    {
      itoa(other, mStr, 10);
    }
    else
    {
      *this = (const char*)"NaN";
    }
  }

  mString(long other) :
    mStr(NULL),
    mCapacity(0)
  {
    // This is platform specific:
    if (ensureCapacity(11 + 1))
    {
      ltoa(other, mStr, 10);
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
    return mStr == NULL || *mStr == '\0';
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

  Iterator split(const char* token) const
  {
    return Iterator(*this, token);
  }

  Iterator begin() const
  {
    return Iterator(*this, NULL);
  }

  Iterator end() const
  {
    return Iterator();
  }

  /**
   * Returns location of first instance of 'search'
   */
  const char* find(const char* search) const
  {
    if (mStr != NULL)
    {
      return strstr(mStr, search);
    }
    else
    {
      return NULL;
    }
  }

  /**
   * Returns true if string begins with 'search'
   */
  bool beginsWith(const char* search) const
  {
    if (mStr != NULL)
    {
      const char* found = find(search);
      return found == mStr;
    }
    else
    {
      return false;
    }
  }

  /**
   * Appends 'a' to this string.
   */
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

  /**
   * Appends 'a' to this string.
   */
  bool append(char a)
  {
    char s[2];
    s[0] = a;
    s[1] = '\0';

    return append(s);
  }

  /**
   * Returns the c-compatible string, null if string is empty
   */
  const char* c_Str() const
  {
    return mStr;
  }
 
  /**
   * Returns true if this stream ownes the memory in which the
   * underlying string is stored.
   */
  inline bool isOwned() const
  {
    return mCapacity != 0;
  }

  /**
   * Ensures that the underlying memory location can hold 'size'
   * number of bytes.  Any existing string is preserved.
   */
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

protected:

  char* mStr;
  size_t mCapacity;
};

#endif // header guard
