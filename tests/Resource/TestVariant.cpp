#include <cfixcc.h>
#include "Variant.h"

class TestVariant : public cfixcc::TestFixture
{
public:
  void AssignPrimitives()
  {
    Variant empty;
    CFIX_ASSERT (empty.empty());

    Variant v;

    v = 10;
    v = (unsigned int)10;
    v = 'c';
    v = (unsigned char)'c';
    v = 100.0f;
    v = 100.0;
    v = (const char*)"hello";
    v = mString("howdy");
  }

  template<typename FromType>
  void VerifyNumericConversionOf(FromType in, const char* str)
  {
    bool pass = false;
    Variant from;

    int toInt;
    unsigned int toUInt;
    char toChar;
    unsigned char toUChar;
    float toFloat;
    double toDouble;
    bool toBool;
    mString toStr;

    from = in;

    pass = from.convertTo(&toInt);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toInt == (int)in);

    pass = from.convertTo(&toUInt);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toUInt == (unsigned int)in);

    pass = from.convertTo(&toChar);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toChar == (char)in);

    pass = from.convertTo(&toUChar);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toUChar == (unsigned char)in);

    pass = from.convertTo(&toFloat);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toFloat == (float)in);

    pass = from.convertTo(&toDouble);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toDouble == (double)in);

    pass = from.convertTo(&toBool);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toBool == (bool)in);

    pass = from.convertTo(&toStr);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toStr == str);
  }

  void ConvertPrimitives()
  {
    VerifyNumericConversionOf<int>(10, "10");
    VerifyNumericConversionOf<unsigned int>(10, "10");
    VerifyNumericConversionOf<char>(97, "a");
    VerifyNumericConversionOf<unsigned char>(97, "a");
    //VerifyNumericConversionOf<float>(10.0f, "10.0");
    //VerifyNumericConversionOf<double>(10.0, "10.0");
    VerifyNumericConversionOf<bool>(true, "true");
    VerifyNumericConversionOf<bool>(false, "false");
  }
};

CFIXCC_BEGIN_CLASS(TestVariant)
  CFIXCC_METHOD(AssignPrimitives)
  CFIXCC_METHOD(ConvertPrimitives)
CFIXCC_END_CLASS()