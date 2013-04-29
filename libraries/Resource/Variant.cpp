
#include "Variant.h"
#include "mString.h"
#include <string.h>

//
//
// Conversion implementations for built-in types
//
//

template<>
bool variant_convert(const char& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == TypeInfo_String)
  {
    // Convert single char to string, rather than
    // treat as numeric type.
    char str[2];
    str[0] = from;
    str[1] = '\0';
    *outVar = mString(str);
    return true;
  }
  else if (toType == TypeInfo_Int)
  {
    *outVar = (int)from;
    return true;
  }
  else if (toType == TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == TypeInfo_Float)
  {
    *outVar = (float)from;
    return true;
  }
  else if (toType == TypeInfo_Bool)
  {
    *outVar = from != 0;
    return true;
  }
  else
  {
    return false;
  }
}

template<>
bool variant_convert(const char* const& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == TypeInfo_String)
  {
    *outVar = mString(from);
    return true;
  }
  else if (toType == TypeInfo_Char)
  {
    if (from == NULL)
    {
      *outVar = (char)0;
    }
    else
    {
      *outVar = (char)atoi(from);
    }
    return true;
  }
  else if (toType == TypeInfo_Int)
  {
    if (from == NULL)
    {
      *outVar = (int)0;
    }
    else
    {
      *outVar = (int)atoi(from);
    }
    return true;
  }
  else if (toType == TypeInfo_Double)
  {
    if (from == NULL)
    {
      *outVar = (double)0;
    }
    else
    {
      *outVar = (double)atof(from);
    }
    return true;
  }
  else if (toType == TypeInfo_Float)
  {
    if (from == NULL)
    {
      *outVar = (float)0;
    }
    else
    {
      *outVar = (float)atof(from);
    }
    return true;
  }
  else if (toType == TypeInfo_Bool)
  {
    if (from == NULL)
    {
      *outVar = false;
    }
    else
    {
      *outVar = (stricmp(from, "true") == 0 || 
                 stricmp(from, "True") == 0 ||
                 stricmp(from, "TRUE") == 0);
    }
    return true;
  }
  else
  {
    return false;
  }
}

template<>
bool variant_convert(const mString& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  // Use const char* conversion.
  return variant_convert(from.c_Str(), toType, outVar);
}

template<>
bool variant_convert(const int& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == TypeInfo_Char)
  {
    *outVar = (char)from;
    return true;
  }
  else if (toType == TypeInfo_String)
  {
    *outVar = mString(from);
    return true;
  }
  else if (toType == TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == TypeInfo_Float)
  {
    *outVar = (float)from;
    return true;
  }
  else if (toType == TypeInfo_Bool)
  {
    *outVar = from != 0;
    return true;
  }
  else
  {
    return false;
  }
}

template<>
bool variant_convert(const bool& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == TypeInfo_Char)
  {
    *outVar = (char)from;
    return true;
  }
  else if (toType == TypeInfo_String)
  {
    *outVar = mString(const_cast<char*>(from ? "true" : "false"));
    return true;
  }
  else if (toType == TypeInfo_Int)
  {
    *outVar = (int)from;
    return true;
  }
  else if (toType == TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == TypeInfo_Float)
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
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == TypeInfo_Char)
  {
    *outVar = (char)(from + 0.5);
    return true;
  }
  else if (toType == TypeInfo_String)
  {
    *outVar = mString(from);
    return true;
  }
  else if (toType == TypeInfo_Int)
  {
    *outVar = (int)(from + 0.5);
    return true;
  }
  else if (toType == TypeInfo_Float)
  {
    *outVar = (float)from;
    return true;
  }
  else if (toType == TypeInfo_Bool)
  {
    *outVar = from != 0.0;
    return true;
  }
  else
  {
    return false;
  }
}

template<>
bool variant_convert(const float& from, 
                     const TypeInfo& toType, 
                     Variant* outVar)
{
  if (toType == TypeInfo_Char)
  {
    *outVar = (char)(from + 0.5);
    return true;
  }
  else if (toType == TypeInfo_String)
  {
    *outVar = mString(from);
    return true;
  }
  else if (toType == TypeInfo_Int)
  {
    *outVar = (int)(from + 0.5);
    return true;
  }
  else if (toType == TypeInfo_Double)
  {
    *outVar = (double)from;
    return true;
  }
  else if (toType == TypeInfo_Bool)
  {
    *outVar = from != 0.0f;
    return true;
  }
  else
  {
    return false;
  }
}