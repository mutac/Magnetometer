
#include "Variant.h"

template<>
bool variant_convert(const char& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == Variant::TypeInfo_Char)
  {
    *outVar = (char)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Int)
  {
    *outVar = (int)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Float)
  {
    *outVar = (float)from;
    return true;
  }
  else
  {
    return false;
  }
}

template<>
bool variant_convert(const int& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == Variant::TypeInfo_Char)
  {
    *outVar = (char)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Int)
  {
    *outVar = (int)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Float)
  {
    *outVar = (float)from;
    return true;
  }
  else
  {
    return false;
  }
}

template<>
bool variant_convert(const double& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == Variant::TypeInfo_Char)
  {
    *outVar = (char)(from + 0.5);
    return true;
  }
  else if (toType == Variant::TypeInfo_Int)
  {
    *outVar = (int)(from + 0.5);
    return true;
  }
  else if (toType == Variant::TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Float)
  {
    *outVar = (float)from;
    return true;
  }
  else
  {
    return false;
  }
}

template<>
bool variant_convert(const float& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == Variant::TypeInfo_Char)
  {
    *outVar = (char)(from + 0.5);
    return true;
  }
  else if (toType == Variant::TypeInfo_Int)
  {
    *outVar = (int)(from + 0.5);
    return true;
  }
  else if (toType == Variant::TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == Variant::TypeInfo_Float)
  {
    *outVar = (float)from;
    return true;
  }
  else
  {
    return false;
  }
}