
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

    /* Default copy c-tor ok */

    ~Iterator()
    {
    }

    Iterator& operator++()
    {
      mPos = nextPos();
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator temp(*this);
      mPos = nextPos();
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
      size_t nxtPos = nextPos();
      return mStr.substr(mPos, 
        nxtPos != mString::npos ? (nxtPos - mTokLen) - mPos : mString::npos);
    }

  private:
    Iterator(const mString& str, size_t pos) :
      mStr(str),
      mPos(pos),
      mToken(NULL),
      mTokLen(0)
    {
    }

    Iterator(const mString& str, size_t pos, const char* token) :
      mStr(str),
      mPos(pos),
      mToken(token),
      mTokLen(strlen(token))
    {
    }

    size_t nextPos() const
    {
      if (mPos != mString::npos && mStr[mPos] != '\0')
      {
        if (emptyToken())
        {
          // No token, iterate through all chars
          if (mStr[mPos + 1] != '\0')
          {
            return mPos + 1;
          }
        }
        else
        {
          // Otherwise advance beyond the next token
          const char* next = strstr(mStr.c_Str() + mPos, mToken);
          if (next != NULL)
          {
            return (size_t)(next - mStr.c_Str()) + mTokLen;
          }
        }
      }

      // No further tokens, or end of string
      return mString::npos;
    }

    /**
     * Returns true if the token is... empty!
     */
    bool emptyToken() const
    {
      return mToken == NULL || *mToken == '\0';
    }

    const mString& mStr;
    size_t mPos;
    const char* mToken;
    size_t mTokLen;
  };

  mString() :
    mCapacity(0)
  {
  }

  mString(const mString& other) :
    mCapacity(0)
  {
    *this = other;
  }

  mString(char* other, size_t count = npos) :
    mCapacity(0)
  {
    set(other, count);
  }

  mString(const char* other, size_t count = npos) :
    mCapacity(0)
  {
    set(other, count);
  }

  mString(int other)  :
    mCapacity(0)
  {
    // This is platform specific:
    if (ensureCapacity(11 + 1))
    {
      itoa(other, mStr, 10);
    }
    else
    {
      set("NaN");
    }
  }

  mString(long other)  :
    mCapacity(0)
  {
    // This is platform specific:
    if (ensureCapacity(11 + 1))
    {
      ltoa(other, mStr, 10);
    }
    else
    {
      set("NaN");
    }
  }

  mString(float other) :
    mCapacity(0)
  {
    set("NaN");
  }

  mString(double other) :
    mCapacity(0)
  {
    set("NaN");
  }

  ~mString()
  {
  }

  mString clone(size_t capacity = npos)
  {
    return mString(c_Str(), capacity);
  }

  void dispose()
  {
    mStr = NULL;
    mCapacity = 0;
  }

  /*** Const operations ***/

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
    set(other);
    return *this;
  }

  mString& operator=(const char* other)
  {
    set(other);
    return *this;
  }

  mString& operator=(const mString& other)
  {
    mStr = other.mStr;
    mCapacity = other.mCapacity;
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

  char operator[](size_t idx) const
  {
    return ((char*)(mStr))[idx];
  }

  Iterator split(const char* token) const
  {
    return Iterator(*this, 0, token);
  }

  Iterator begin() const
  {
    return Iterator(*this, 0);
  }

  Iterator end() const
  {
    return Iterator(*this, npos);
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

  size_t findAt(const char* search) const
  {
    if (mStr != NULL)
    {
      const char* found = find(search);
      if (found != NULL)
      {
        return (size_t)(found - mStr.get());
      }
    }

    return npos;
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
   * Returns the c-compatible string, null if string is empty
   */
  const char* c_Str() const
  {
    return mStr;
  }

  /**
   * Returns a new substring beginning at 'start', and of length 'len'.
   * If 'len' is greater than the length of the full string, the 
   * result will be truncated.  A len of mString::npos indicates
   * all characters to the end of the string.
   */
  mString substr(size_t start = 0, size_t len = npos) const
  {
    return mString((((char*)mStr) + start), len);
  }

  /*** Non-const operations ***/

  bool set(const char* other, size_t count = npos)
  {
    // Optimization: release current string to 
    // avoid copy in ensureCapacity()
    dispose();

    if (other != NULL)
    {
      if (count == npos)
      {
        count = strlen(other);
      }

      if (!ensureCapacity(count + 1))
      {
        return false;
      }

      strncpy(mStr, other, count);
    }

    return true;
  }

  char& operator[](size_t idx) 
  {
    if (!canMutate())
    {
      *this = clone();
    }

    return ((char*)(mStr))[idx];
  }

  /**
   * Appends 'a' to this string.
   */
  bool append(const char* a)
  {
    if (a != NULL)
    {
      size_t neededCapacity = length() + strlen(a) + 1;

      if (!canMutate())
      {
        *this = clone(neededCapacity);
      }
      else
      {
        if (!ensureCapacity(neededCapacity))
        {
          return false;
        }
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
   * Ensures that the underlying memory location can hold 'size'
   * number of bytes.  Any existing string is preserved.
   */
  bool ensureCapacity(size_t size)
  {
    if (size > mCapacity)
    {
      return expand(size);
    }
    else
    {
      return true;
    }
  }

  static const size_t npos = (size_t)-1;

protected:

  /**
   * Ensures that the underlying memory location can hold 'size'
   * number of bytes.  Any existing string is preserved.
   */
  bool expand(size_t size) 
  {
    if (size > 0)
    {
      SharedPointer<char> newStr = new char[size];

      if (mStr != NULL && newStr != NULL)
      {
        strcpy(newStr, mStr);
      }
      else if (newStr != NULL)
      {
        memset(newStr, 0, size);
      }

      mStr = newStr;
      mCapacity = size;
      
      return mStr != NULL;
    }
    else
    {
      return true;
    }
  }

  bool canMutate() const
  {
    return mStr.getRefCount() == 1;
  }

  SharedPointer<char> mStr;
  size_t mCapacity;
};

#endif // header guard
