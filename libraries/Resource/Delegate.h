
#ifndef _DELEGATE_H_841a4628_beb8_46e5_a65d_f24fcc7e5042
#define _DELEGATE_H_841a4628_beb8_46e5_a65d_f24fcc7e5042

template<typename ReturnType, typename Arg1Type>
class IDelegateFunction_1
{
public:
  virtual ReturnType invoke(Arg1Type arg1) = 0;
};

template<typename ReturnType, 
         typename Arg1Type, 
         typename ObjectType, 
         typename MethodType>
class MethodDelegate_1 : public IDelegateFunction_1<ReturnType, Arg1Type>
{
public:
  MethodDelegate_1(ObjectType* object, MethodType methodPtr) :
    mObject(object),
    mMethodPtr(methodPtr)
  {
  }

  virtual ReturnType invoke(Arg1Type arg1)
  {
    return (mObject->*mMethodPtr)(arg1);
  }

private:
  ObjectType* mObject;
  MethodType mMethodPtr;
};

template<typename ReturnType, 
         typename Arg1Type, 
         typename MethodType>
class StaticDelegate_1 : public IDelegateFunction_1<ReturnType, Arg1Type>
{
public:
  StaticDelegate_1(MethodType methodPtr) :
    mMethodPtr(methodPtr)
  {
  }

  virtual ReturnType invoke(Arg1Type arg1)
  {
    return (*mMethodPtr)(arg1);
  }

private:
  MethodType mMethodPtr;
};

template<typename ReturnType, typename Arg1Type>
class Delegate
{
public:
  template <typename MethodType>
  Delegate(MethodType method) :
    mDelegate(new StaticDelegate_1<ReturnType, Arg1Type, MethodType>(method))
  {
  }

  template <typename ObjectType, typename MethodType>
  Delegate(ObjectType* object, MethodType method) :
    mDelegate(new MethodDelegate_1<ReturnType, Arg1Type, ObjectType, MethodType>(object, method))
  {
  }

  ~Delegate() 
  {
    delete mDelegate;
  }

  ReturnType invoke(Arg1Type arg1)
  {
    return mDelegate->invoke(arg1);
  }

private:
  IDelegateFunction_1<ReturnType, Arg1Type>* mDelegate;
};

#endif // Header guard
