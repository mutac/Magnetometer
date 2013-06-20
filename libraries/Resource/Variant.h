

#ifndef _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582
#define _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582

#include "mDefs.h"
#include "mStd.h"

/**
 * Define to ensure conversion specializations are present for all types
 * that are stored within a variant.
 */
// #define mVariantStaticallyEnsureConversions

class Variant;

/**
 * Variant type identifier.
 */
typedef unsigned char TypeInfo;

/**
 * Type identifiers for build-in types.
 */
static const TypeInfo TypeInfo_Unknown = 0;
static const TypeInfo TypeInfo_Char = 1;
static const TypeInfo TypeInfo_ConstCharArray = 2;
static const TypeInfo TypeInfo_Int = 3;
static const TypeInfo TypeInfo_Long = 4;
static const TypeInfo TypeInfo_Bool = 5;
static const TypeInfo TypeInfo_Float = 6;
static const TypeInfo TypeInfo_Double = 7;
static const TypeInfo TypeInfo_String = 8;
/** Begin user types at 20 */

/**
 * Returns variant type information associated with a type.
 * Specialize for your type.
 */
template <typename ValueType>
const TypeInfo& type_info_of();

/**
 * Convert a type to another type, storing the result in a variant.
 * Specialize for your type.
 */
template <typename FromType>
inline bool variant_convert(const FromType& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
#ifdef mVariantStaticallyEnsureConversions
; // No default impl
#else
{
  // Default impl -- no conversion possible
  return false;
}
#endif // mVariantStaticallyEnsureConversions

/**
 */
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
    if (mContent != NULL)
    {
      delete mContent;
    }
  }

  /**
   */
  bool empty() const
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
   * Returns a pointer to the value contained by the variant.  'ValueType'
   * must match the type of the contained value.  NULL will be returne if
   * the type is incorrect.
   */
  template <typename ValueType>
  const ValueType* get() const
  {
    if (mContent != NULL && isType<ValueType>())
    {
      return &static_cast<Holder<ValueType>*>(mContent)->mHeld;
    }
    else
    {
      return NULL;
    }
  }

  /**
   * Returns the value contained by the variant.  'ValueType' must
   * match the type of the contained value.  False will be returne
   * if the type is incorrect.
   */
  template <typename ValueType>
  bool getValue(ValueType* outValue) const
  {
    const ValueType* val  = get<ValueType>();
    if (val == NULL)
    {
      return false;
    }

    *outValue = *val;

    return true;
  }

  /**
   * Convert a variant from one type to another.  False is returned
   * if conversion is impossible.
   */
  template <typename ToType>
  bool convertTo(ToType* outConverted) const
  {
    const ToType* result = NULL;

    if (!empty())
    {
      if (getValue(outConverted))
      {
        // Identity conversion
        return true;
      }
      else
      {
        // Other type conversion
        Variant converted;

        if (!mContent->convertTo(type_info_of<ToType>(), &converted))
        {
          return false;
        }

        return converted.getValue(outConverted);
      }
    }
    else
    {
      // Empty
      return false;
    }
  }

  /**
   * Returns the variant type information about the currently contained value.
   */
  const TypeInfo& getTypeInfo() const
  {
    if (!empty())
    {
      return mContent->getTypeInfo();
    }
    else
    {
      return TypeInfo_Unknown;
    }
  }

  /**
   * Returns true if variant currently contains a value of type 'T'
   */
  template <typename T>
  bool isType() const
  {
    return type_info_of<T>() == getTypeInfo();
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
      mTypeInfo = type_info_of<TypeHeld>();
    }

    /**
     */
    PlaceHolder* clone() const
    {
      return new Holder(mHeld);
    }

    /**
     */
    const TypeInfo& getTypeInfo() const
    {
      return mTypeInfo;
    }

    /**
     */
    bool convertTo(const TypeInfo& toType, Variant* outConverted) const
    {
      // You gotta make one of these too:
      return variant_convert(mHeld, toType, outConverted);
    }
  
    const TypeHeld mHeld;
    TypeInfo mTypeInfo;
  };

  PlaceHolder* mContent;
};

#include "VariantImpl.h"

#endif // header guard
