
#ifndef _VARIANT_IMPL_H_227500b5_53ed_4aae_b16f_7d462f91b0a3
#define _VARIANT_IMPL_H_227500b5_53ed_4aae_b16f_7d462f91b0a3

#include "Variant.h"
#include "mString.h"

namespace mStd
{
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<char>()
  {
    return TypeInfo_Char;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<unsigned char>()
  {
    return type_of<char>();
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<const char*>()
  {
    return TypeInfo_ConstCharArray;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<char*>()
  {
    // Store char* as a mString (for ownership)
    return TypeInfo_String;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<mString>()
  {
    return TypeInfo_String;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<int>()
  {
    return TypeInfo_Int;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<unsigned int>()
  {
    return type_of<int>();
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<long>()
  {
    return TypeInfo_Long;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<unsigned long>()
  {
    return type_of<long>();
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<bool>()
  {
    return TypeInfo_Bool;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<float>()
  {
    return TypeInfo_Float;
  }
  /**
  * type_of specialization for primitive types
  */
  template <>
  inline const TypeInfo& type_of<double>()
  {
    return TypeInfo_Double;
  }

  /**
  */
  template<>
  bool type_conversion(const char& from,
    const TypeInfo& toType,
    Variant* to);
  template<>
  inline bool type_conversion(const unsigned char& from,
    const TypeInfo& toType,
    Variant* to)
  {
    return type_conversion<char>(from, toType, to);
  }

  /**
  */
  template<>
  bool type_conversion(const char* const& from,
    const TypeInfo& toType,
    Variant* to);
  /**
  */
  template<>
  bool type_conversion(const mString& from,
    const TypeInfo& toType,
    Variant* to);
  template<>
  inline bool type_conversion(char* const& from,
    const TypeInfo& toType,
    Variant* to)
  {
    // char* type is internally stored as mStrings.
    return type_conversion<mString>(from, toType, to);
  }

  /**
  */
  template<>
  bool type_conversion(const int& from,
    const TypeInfo& toType,
    Variant* to);
  template<>
  inline bool type_conversion(const unsigned int& from,
    const TypeInfo& toType,
    Variant* to)
  {
    return type_conversion<int>(from, toType, to);
  }

  /**
  */
  template<>
  bool type_conversion(const long& from,
    const TypeInfo& toType,
    Variant* to);
  template<>
  inline bool type_conversion(const unsigned long& from,
    const TypeInfo& toType,
    Variant* to)
  {
    return type_conversion<long>(from, toType, to);
  }

  /**
  */
  template<>
  bool type_conversion(const bool& from,
    const TypeInfo& toType,
    Variant* to);

  /**
  */
  template<>
  bool type_conversion(const double& from,
    const TypeInfo& toType,
    Variant* to);

  /**
  */
  template<>
  bool type_conversion(const float& from,
    const TypeInfo& toType,
    Variant* to);
}

#endif // header guard
