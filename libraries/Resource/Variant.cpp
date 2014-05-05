
#include "Variant.h"
#include "mString.h"
#include <string.h>

namespace mResource
{
  //
  //
  // Conversion implementations for built-in types
  //
  //

  template<>
  bool type_conversion(const char& from,
    const TypeInfo& toType,
    Variant* to)
  {
    if (toType == TypeInfo_String)
    {
      // Convert single char to string, rather than
      // treat as numeric type.
      char str[2];
      str[0] = from;
      str[1] = '\0';
      *to = mString(str);
      return true;
    }
    else if (toType == TypeInfo_Int)
    {
      *to = (int)from;
      return true;
    }
    else if (toType == TypeInfo_Long)
    {
      *to = (long)from;
      return true;
    }
    else if (toType == TypeInfo_Double)
    {
      *to = (double)from;
      return true;
    }
    else if (toType == TypeInfo_Float)
    {
      *to = (float)from;
      return true;
    }
    else if (toType == TypeInfo_Bool)
    {
      *to = from != 0;
      return true;
    }
    else
    {
      return false;
    }
  }

  template<>
  bool type_conversion(const char* const& from,
    const TypeInfo& toType,
    Variant* to)
  {
    if (toType == TypeInfo_String)
    {
      *to = mString(from);
      return true;
    }
    else if (toType == TypeInfo_Char)
    {
      if (from == NULL)
      {
        *to = (char)0;
      }
      else
      {
        *to = (char)atoi(from);
      }
      return true;
    }
    else if (toType == TypeInfo_Int)
    {
      if (from == NULL)
      {
        *to = (int)0;
      }
      else
      {
        *to = (int)atoi(from);
      }
      return true;
    }
    else if (toType == TypeInfo_Long)
    {
      if (from == NULL)
      {
        *to = (long)0;
      }
      else
      {
        *to = (long)atol(from);
      }
      return true;
    }
    else if (toType == TypeInfo_Double)
    {
      if (from == NULL)
      {
        *to = (double)0;
      }
      else
      {
        *to = (double)atof(from);
      }
      return true;
    }
    else if (toType == TypeInfo_Float)
    {
      if (from == NULL)
      {
        *to = (float)0;
      }
      else
      {
        *to = (float)atof(from);
      }
      return true;
    }
    else if (toType == TypeInfo_Bool)
    {
      if (from == NULL)
      {
        *to = false;
      }
      else
      {
        *to = (mStd::strcasecmp(from, "true") == 0);
      }
      return true;
    }
    else
    {
      return false;
    }
  }

  template<>
  bool type_conversion(const mString& from,
    const TypeInfo& toType,
    Variant* to)
  {
    // Use const char* conversion.
    return type_conversion(from.c_str(), toType, to);
  }

  template<>
  bool type_conversion(const int& from,
    const TypeInfo& toType,
    Variant* to)
  {
    if (toType == TypeInfo_Char)
    {
      *to = (char)from;
      return true;
    }
    else if (toType == TypeInfo_Long)
    {
      *to = (long)from;
      return true;
    }
    else if (toType == TypeInfo_String)
    {
      *to = to_string(from);
      return true;
    }
    else if (toType == TypeInfo_Double)
    {
      *to = (double)from;
      return true;
    }
    else if (toType == TypeInfo_Float)
    {
      *to = (float)from;
      return true;
    }
    else if (toType == TypeInfo_Bool)
    {
      *to = from != 0;
      return true;
    }
    else
    {
      return false;
    }
  }

  template<>
  bool type_conversion(const long& from,
    const TypeInfo& toType,
    Variant* to)
  {
    if (toType == TypeInfo_Char)
    {
      *to = (char)from;
      return true;
    }
    else if (toType == TypeInfo_String)
    {
      *to = to_string(from);
      return true;
    }
    else if (toType == TypeInfo_Int)
    {
      *to = (int)from;
      return true;
    }
    else if (toType == TypeInfo_Double)
    {
      *to = (double)from;
      return true;
    }
    else if (toType == TypeInfo_Float)
    {
      *to = (float)from;
      return true;
    }
    else if (toType == TypeInfo_Bool)
    {
      *to = from != 0;
      return true;
    }
    else
    {
      return false;
    }
  }

  template<>
  bool type_conversion(const bool& from,
    const TypeInfo& toType,
    Variant* to)
  {
    if (toType == TypeInfo_Char)
    {
      *to = (char)from;
      return true;
    }
    else if (toType == TypeInfo_String)
    {
      *to = mString(from ? "true" : "false");
      return true;
    }
    else if (toType == TypeInfo_Int)
    {
      *to = (int)from;
      return true;
    }
    else if (toType == TypeInfo_Long)
    {
      *to = (long)from;
      return true;
    }
    else if (toType == TypeInfo_Double)
    {
      *to = (double)from;
      return true;
    }
    else if (toType == TypeInfo_Float)
    {
      *to = (float)from;
      return true;
    }
    else
    {
      return false;
    }
  }

  template<>
  bool type_conversion(const double& from,
    const TypeInfo& toType,
    Variant* to)
  {
    if (toType == TypeInfo_Char)
    {
      *to = (char)(from + 0.5);
      return true;
    }
    else if (toType == TypeInfo_String)
    {
      *to = to_string(from);
      return true;
    }
    else if (toType == TypeInfo_Int)
    {
      *to = (int)(from + 0.5);
      return true;
    }
    else if (toType == TypeInfo_Long)
    {
      *to = (long)from;
      return true;
    }
    else if (toType == TypeInfo_Float)
    {
      *to = (float)from;
      return true;
    }
    else if (toType == TypeInfo_Bool)
    {
      *to = from != 0.0;
      return true;
    }
    else
    {
      return false;
    }
  }

  template<>
  bool type_conversion(const float& from,
    const TypeInfo& toType,
    Variant* to)
  {
    if (toType == TypeInfo_Char)
    {
      *to = (char)(from + 0.5);
      return true;
    }
    else if (toType == TypeInfo_String)
    {
      *to = to_string(from);
      return true;
    }
    else if (toType == TypeInfo_Int)
    {
      *to = (int)(from + 0.5);
      return true;
    }
    else if (toType == TypeInfo_Long)
    {
      *to = (long)from;
      return true;
    }
    else if (toType == TypeInfo_Double)
    {
      *to = (double)from;
      return true;
    }
    else if (toType == TypeInfo_Bool)
    {
      *to = from != 0.0f;
      return true;
    }
    else
    {
      return false;
    }
  }

}