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
  }

  void ConvertPrimitives()
  {
    bool pass = false;
    Variant from;

    float toFloat;
    double toDouble;
    char toChar;

    // Int conversions
    from = 10;

    pass = from.convertTo<float>(&toFloat);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toFloat == 10.0f);

    pass = from.convertTo<double>(&toDouble);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toDouble == 10.0);

    pass = from.convertTo<char>(&toChar);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toChar = 10);

    // Unsigned int conversions
    from = (unsigned int)10;

    pass = from.convertTo<float>(&toFloat);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toFloat == 10.0f);

    pass = from.convertTo<double>(&toDouble);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toDouble == 10.0);

    pass = from.convertTo<char>(&toChar);
    CFIX_ASSERT(pass);
    CFIX_ASSERT(toChar = 10);
  }
};

CFIXCC_BEGIN_CLASS(TestVariant)
  CFIXCC_METHOD(AssignPrimitives)
  CFIXCC_METHOD(ConvertPrimitives)
CFIXCC_END_CLASS()