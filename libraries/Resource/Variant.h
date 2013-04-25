

#ifndef _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582
#define _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582

#include "mDefs.h"
#include "mStd.h"

template <typename T>
struct TypeWrapper
{
  typedef T TYPE;
  typedef const T CONSTTYPE;
  typedef T& REFTYPE;
  typedef const T& CONSTREFTYPE;
};

template <typename T>
struct TypeWrapper<const T>
{
  typedef T TYPE;
  typedef const T CONSTTYPE;
  typedef T& REFTYPE;
  typedef const T& CONSTREFTYPE;
};

template <typename T>
struct TypeWrapper<const T&>
{
  typedef T TYPE;
  typedef const T CONSTTYPE;
  typedef T& REFTYPE;
  typedef const T& CONSTREFTYPE;
};

template <typename T>
struct TypeWrapper<T&>
{
  typedef T TYPE;
  typedef const T CONSTTYPE;
  typedef T& REFTYPE;
  typedef const T& CONSTREFTYPE;
};

class Variant;

typedef int TypeInfo;
static const TypeInfo TypeInfo_Unknown = 0;
static const TypeInfo TypeInfo_Char = 1;
static const TypeInfo TypeInfo_ConstCharArray = 3;
static const TypeInfo TypeInfo_Int = 4;
static const TypeInfo TypeInfo_Bool = 5;
static const TypeInfo TypeInfo_Float = 6;
static const TypeInfo TypeInfo_Double = 7;
static const TypeInfo TypeInfo_String = 8;
/** Begin user types at 20 */

/**
 * Variant type info method, specialize for your new type.
 */
template <typename ValueType>
const TypeInfo& variant_type_info();

/**
 * Variant type conversion, specialize for your new type.
 */
template <typename FromType>
inline bool variant_convert(const FromType& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  // TODO: Remove default impl
  return false;
}

class Variant
{
public:
  Variant() :
    mContent(NULL)
  {
  }

  Variant(const Variant& rhs) :
    mContent(rhs.mContent ? rhs.mContent->clone() : NULL)
  {
  }

  template<typename ValueType>
  Variant(const ValueType& value) :
    mContent(new Holder<ValueType>(value))
  {
  }

  ~Variant()
  {
    if (mContent)
    {
      // TODO: allocator/deallocator
      delete mContent;
    }
  }

  /**
   */
  bool empty()
  {
    return mContent == NULL;
  }

  /**
   */
  Variant& swap(Variant& rhs)
  {
    mStd::swap(mContent, rhs.mContent);
    return *this;
  }

  /**
   */
  Variant& operator=(const Variant& rhs)
  {
    // Super-duper inefficient
    Variant tmp(rhs);
    return swap(tmp);
  }

  /**
   */
  template <typename ValueType>
  Variant& operator=(const ValueType& rhs)
  {
    Variant tmp(rhs);
    return swap(tmp);
  }

  /**
   */
  template <typename ValueType>
  const ValueType* ptr() const
  {
    if (variant_type_info<ValueType>() == mContent->getTypeInfo())
    {
      return &static_cast<Holder<ValueType>*>(mContent)->mHeld;
    }
    else
    {
      return NULL;
    }
  }

  template <typename ValueType>
  const ValueType& value() const
  {
    mDebugAssert(variant_type_info<ValueType>() == mContent->getTypeInfo());
    return static_cast<Holder<ValueType>*>(mContent)->mHeld;
  }

  template <typename ToType>
  bool convertTo(ToType* outConverted) const
  {
    if (mContent != NULL)
    {
      Variant converted;

      bool succeeded = false;
      
      succeeded = mContent->convertTo(
        variant_type_info<ToType>(), 
        &converted);
      if (!succeeded)
      {
        return false;
      }

      const ToType* result = converted.ptr<ToType>();
      mDebugAssert(result != NULL);

      *outConverted = *result;
      return succeeded;
    }
    else
    {
      return false;
    }
  }

  const TypeInfo& getTypeInfo() const
  {
    if (mContent != NULL)
    {
      return mContent->getTypeInfo();
    }
    else
    {
      return TypeInfo_Unknown;
    }
  }

private:
  /**
   */
  class PlaceHolder
  {
  public:
    virtual ~PlaceHolder() {}
    virtual PlaceHolder* clone() const = 0;
    virtual const TypeInfo& getTypeInfo() const = 0;
    virtual bool convertTo(const TypeInfo& toType, Variant* outConverted) const = 0;
  };

  /**
   */
  template<typename TypeHeld>
  class Holder : public PlaceHolder
  {
  public:
    Holder(const TypeHeld& value)
      : mHeld(value)
    {
      // You gotta make one of these:
      mTypeInfo = variant_type_info<TypeHeld>();
    }
    /**
     */
    PlaceHolder* clone() const
    {
      // TODO: Allocator/Deallocator
      return new Holder(mHeld);
    }

    const TypeInfo& getTypeInfo() const
    {
      return mTypeInfo;
    }

    bool convertTo(const TypeInfo& toType, Variant* outConverted) const
    {
      // You gotta make one of these too:
      return variant_convert<TypeHeld>(mHeld, toType, outConverted);
    }
  
    const TypeHeld mHeld;
    TypeInfo mTypeInfo;
  };

  PlaceHolder* mContent;
};

#include "VariantImpl.h"

#endif // header guard
