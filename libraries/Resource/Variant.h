

#ifndef _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582
#define _VARIANT_H_7b8ba50a_bf45_4dd2_9272_a13596a52582

#include "Defs.h"
#include "SharedPointer.h"

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

/**
 * @brief
 */
class Variant
{
public:
  Variant() :
    mImpl(NULL)
  {
  }

  template<class T>
  Variant(T inValue) :
    // TODO: Allocator/deallocator
    mImpl(new VariantImpl<typename TypeWrapper<T>::TYPE>(inValue))
  {
  }

  template<class T>
  typename TypeWrapper<T>::REFTYPE get()
  {
    VariantImpl<typename TypeWrapper<T>::TYPE> valRef = 
      *reinterpret_cast<VariantImpl<typename TypeWrapper<T>::TYPE>*>(mImpl.get());

    return valRef.mValue;
  }

  template<class T>
  typename TypeWrapper<T>::CONSTREFTYPE get() const
  {
    return reinterpret_cast<VariantImpl<typename TypeWrapper<T>::TYPE>*>(mImpl.get()).mValue;
  }

  template<class T>
  void set(typename TypeWrapper<T>::CONSTREFTYPE inValue)
  {
    // TODO: Allocator/deallocator
    mImpl = new VariantImpl<typename TypeWrapper<T>::TYPE>(inValue);
  }

private:
  struct AbstractVariantImpl
  {
    virtual ~AbstractVariantImpl() {}
  };

  template<class T>
  struct VariantImpl : public AbstractVariantImpl
  {
    VariantImpl(T inValue) : mValue(inValue) { }

    ~VariantImpl() {}

    T mValue;
  };

  SharedPointer<AbstractVariantImpl> mImpl;
};

#endif
