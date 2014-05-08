

#ifndef _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582
#define _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582

#include "mDefs.h"
#include "mStd.h"
#include "TypeInfo.h"

namespace mStd
{
  /**
  * Define to ensure conversion specializations are present for all types
  * that are stored within a variant.
  */
  // #define mVariantStaticallyEnsureConversions

  class Variant;

  /**
  * Convert a type to another type, storing the result in a variant.
  * Specialize for your type.
  */
  template <typename FromType>
  inline bool type_conversion(const FromType& from,
    const TypeInfo& toType,
    Variant* to)
#ifdef mVariantStaticallyEnsureConversions
    ; // No default impl
#else
  {
    // Default impl -- no conversion possible
    return false;
  }
#endif // mVariantStaticallyEnsureConversions

  /**
   Specialize this for a type.  By default operator == is used for
   equality.
  */
  template <typename Type>
  inline bool type_compare_equal(const Type& left, const Type& right)
  {
    return left == right;
  }

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
      clear();
    }

    /**
    */
    bool empty() const
    {
      return mContent == NULL;
    }

    /**
    */
    void clear()
    {
      if (mContent != NULL)
      {
        delete mContent;
        mContent = NULL;
      }
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
     Returns true if the values are exactly the same (including type)
    */
    bool operator==(const Variant& rhs) const
    {
      if (mContent == NULL && rhs.mContent == NULL)
      {
        return true;
      }
      else if (mContent == NULL || rhs.mContent == NULL)
      {
        return false;
      }
      else
      {
        return mContent->equals(rhs.mContent);
      }
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
      const ValueType* val = get<ValueType>();
      if (val == NULL)
      {
        return false;
      }

      *outValue = *val;

      return true;
    }

    /** 
     * Convert, in place, from one type to another.  False is 
     * returned if a conversion is impossible.
     */
    bool changeType(const TypeInfo& toType)
    {
      if (empty())
      {
        return false;
      }

      if (getType() != toType)
      {
        Variant converted;

        if (!mContent->convertTo(toType, &converted))
        {
          return false;
        }

        swap(converted);
      }

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

          if (!mContent->convertTo(type_of<ToType>(), &converted))
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
    const TypeInfo& getType() const
    {
      if (!empty())
      {
        return mContent->getType();
      }
      else
      {
        return TypeInfo_Empty;
      }
    }

    /**
    * Returns true if variant currently contains a value of type 'T'
    */
    template <typename T>
    bool isType() const
    {
      return type_of<T>() == getType();
    }

  private:
    /**
    */
    class PlaceHolder
    {
    public:
      virtual ~PlaceHolder() {}
      virtual PlaceHolder* clone() const = 0;
      virtual const TypeInfo& getType() const = 0;
      virtual bool convertTo(const TypeInfo& toType, Variant* outConverted) const = 0;
      virtual bool equals(const PlaceHolder* other) const = 0;
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
        mTypeInfo = type_of<TypeHeld>();
      }

      /**
      */
      PlaceHolder* clone() const
      {
        return new Holder(mHeld);
      }

      /**
      */
      const TypeInfo& getType() const
      {
        return mTypeInfo;
      }

      /**
      */
      bool convertTo(const TypeInfo& toType, Variant* outConverted) const
      {
        // You gotta make one of these too:
        return type_conversion(mHeld, toType, outConverted);
      }

      /**
      */
      bool equals(const PlaceHolder* other) const
      {
        if (other->getType() == getType())
        {
          //static_cast<Holder<ValueType>*>(mContent)->mHeld;
          const Holder<TypeHeld>* otherOfMyType = 
            static_cast<const Holder<TypeHeld>*>(other);

          return type_compare_equal(mHeld, otherOfMyType->mHeld);
        }
        else
        {
          return false;
        }
      }

      const TypeHeld mHeld;
      TypeInfo mTypeInfo;
    };

    PlaceHolder* mContent;
  };
}

#include "VariantImpl.h"

#endif // header guard
