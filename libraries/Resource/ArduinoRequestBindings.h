

#ifndef _REQUEST_BINDINGS_H_da4cc686_b97b_4892_8aa8_8d5d11224c3f
#define _REQUEST_BINDINGS_H_da4cc686_b97b_4892_8aa8_8d5d11224c3f

#include <Print.h>
#include "IRequest.h"
#include "mString.h"

/*
 */
class mStringPrinter : public Print, public mString
{
public:
  mStringPrinter()
  {
  }
  
  size_t write(uint8_t val) 
  {
    char str[2];
    str[0] = (char)val;
    str[1] = '\0';
    append(str);
  }

  mString& toString()
  {
    return *this;
  }
};

/**
 */
class PrintingResponseHandler : public IResponse
{
public:
  PrintingResponseHandler(Print* printer) :
    mPrinter(printer),
    mFailed(false)
  {
    mDebugAssert(printer != NULL);
  }
  
  bool write(const char* name, const Variant& val)
  {
    bool succeeded = true;
    mString str;
       
    mPrinter->print(name);
    mPrinter->print(": ");
    
    succeeded = val.convertTo(&str);
    if (succeeded) 
    {
      mPrinter->println(str.c_Str());
    }
    else
    {
      mPrinter->println("Unknown");
      succeeded = false;
    }
    
    return succeeded;
  }
  
  void setFailure(const char* reason = "")
  {
    mFailed = true;
    mPrinter->print("Request failure: ");
    mPrinter->println(reason);
  }
  
  bool failed() const
  {
    return mFailed;
  }
  
private:
  bool mFailed;
  Print* mPrinter;
};


#endif

