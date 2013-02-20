

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

struct AbstractVariantImpl
{
  virtual ~AbstractVariantImpl() {}
  //virtual void* getUntyped() = 0;
};

//
// TODO: Ownership of value?
// 
template<class T>
class VariantImpl : public AbstractVariantImpl
{
public:
  VariantImpl(T inValue) : mValue(inValue) { }
  virtual ~VariantImpl() {}

  T& get()
  {
    return mValue;
  }

private:
  T mValue;
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
    //VariantImpl<typename TypeWrapper<T>::TYPE>* valRef = 
    //  reinterpret_cast<VariantImpl<typename TypeWrapper<T>::TYPE>*>(mImpl.get());

    //mdebugAssert(valRef != NULL);
    //return valRef->get<typename TypeWrapper<T>::TYPE>();
    AbstractVariantImpl* impl = mImpl.get();
    return impl->get<typename TypeWrapper<T>::TYPE>();
  }

  template<class T>
  typename TypeWrapper<T>::CONSTREFTYPE get() const
  {
    //VariantImpl<typename TypeWrapper<T>::TYPE>* valRef =
    //  reinterpret_cast<VariantImpl<typename TypeWrapper<T>::TYPE>*>(mImpl.get());

    //mDebugAssert(valRef != NULL);
    //return valRef->get<typename TypeWrapper<T>::TYPE>();
    AbstractVariantImpl* impl = mImpl.get();
    return impl->get<typename TypeWrapper<T>::TYPE>();
  }

  template<class T>
  void set(typename TypeWrapper<T>::CONSTREFTYPE inValue)
  {
    // TODO: Allocator/deallocator
    mImpl = new VariantImpl<typename TypeWrapper<T>::TYPE>(inValue);
  }

private:
  SharedPointer<AbstractVariantImpl> mImpl;
};

#endif
