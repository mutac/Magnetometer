

#ifndef _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582
#define _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582

#include "mDefs.h"
#include "mStd.h"
//#include "SharedPointer.h"

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

///**
// */
//struct IVariantContainer
//{
//  virtual ~IVariantContainer() {}
//};
//
///**
// */
//template<class T>
//class VariantContainer : public IVariantContainer
//{
//public:
//  VariantContainer(T inValue) : mValue(inValue) { }
//  virtual ~VariantContainer() {}
//private:
//  T mValue;
//};
//
///**
// * @brief
// */
//class Variant
//{
//public:
//  Variant() :
//    mImpl(NULL)
//  {
//  }
//
//  template<class T>
//  Variant(T inValue) :
//    // TODO: Allocator/deallocator
//    mImpl(new VariantContainer<typename TypeWrapper<T>::TYPE>(inValue))
//  {
//  }
//
//  bool empty() const
//  {
//    return mImpl == NULL;
//  }
//
//  template<class ToType>
//  typename TypeWrapper<ToType>::REFTYPE get()
//  {
//    mDebugging(mImpl != NULL);
//    IVariantContainer* impl = mImpl.get();
//
//    //VariantConversion<ToType> conversion;
//    //impl->convert(conversion);
//
//    return conversion.get();
//  }
//
//  //template<class T>
//  //typename TypeWrapper<T>::REFTYPE get()
//  //{
//  //  //VariantContainer<typename TypeWrapper<T>::TYPE>* valRef = 
//  //  //  reinterpret_cast<VariantContainer<typename TypeWrapper<T>::TYPE>*>(mImpl.get());
//
//  //  //mdebugAssert(valRef != NULL);
//  //  //return valRef->get<typename TypeWrapper<T>::TYPE>();
//  //  IVariantContainer* impl = mImpl.get();
//  //  //return impl->get<typename TypeWrapper<T>::TYPE>();
//  //}
//
//  //template<class T>
//  //typename TypeWrapper<T>::CONSTREFTYPE get() const
//  //{
//  //  //VariantContainer<typename TypeWrapper<T>::TYPE>* valRef =
//  //  //  reinterpret_cast<VariantContainer<typename TypeWrapper<T>::TYPE>*>(mImpl.get());
//
//  //  //mDebugAssert(valRef != NULL);
//  //  //return valRef->get<typename TypeWrapper<T>::TYPE>();
//  //  IVariantContainer* impl = mImpl.get();
//  //  //return impl->get<typename TypeWrapper<T>::TYPE>();
//  //}
//
//  template<class T>
//  void set(typename TypeWrapper<T>::CONSTREFTYPE inValue)
//  {
//    // TODO: Allocator/deallocator
//    mImpl = new VariantContainer<typename TypeWrapper<T>::TYPE>(inValue);
//  }
//
//private:
//  SharedPointer<IVariantContainer> mImpl;
//};

class Variant
{
public:
  Variant() :
    mContent(NULL)
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

#ifdef mUseRtti
  std::type_info& typeInfo() const
  {
    if (mContent != NULL)
    {
      return mContent->typeInfo();
    }
    else
    {
      return typeid(void);
    }
  }
#endif // mUseRtti

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
    return swap(Variant(rhs));
  }

  /**
   */
  template <typename ValueType>
  const ValueType* toPtr() const
  {
#ifdef mUseRtti
    if (typeInfo() == typeid(ValueType))
    {
#endif // mUseRtti
      return &static_cast<Holder<ValueType> *>(mContent)->mHeld;
#ifdef mUseRtti
    }
    else
    {
      return NULL;
    }
#endif // mUseRtti
  }

  /**
   */
  template <typename ValueType>
  ValueType toValue(const Variant& rhs)
  {
    ValueType* result = rhs.toPtr<ValueType>();
    if (result == NULL)
    {
#ifdef mUseExceptions
      throw std::bad_cast();
#endif // mUseExceptions
    }

    return result;
  }
private:
  /**
   */
  class PlaceHolder
  {
  public:
    virtual ~PlaceHolder() {}
#ifdef mUseRtti
    virtual std::type_info& typeInfo() const = 0;
#endif // mUseRtti
    virtual PlaceHolder* clone() const = 0;
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
    }

    /**
     */
#ifdef mUseRtti
    std::type_info& typeInfo() const
    {
      return typeid(TypeHeld);
    }
#endif // mUseRtti

    /**
     */
    virtual PlaceHolder* clone() const
    {
      // TODO: Allocator/Deallocator
      return new Holder(mHeld);
    }
  private:
    const TypeHeld mHeld;
  };

  PlaceHolder* mContent;
};

#endif // header guard
