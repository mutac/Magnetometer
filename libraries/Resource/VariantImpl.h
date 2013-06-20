
#ifndef _VARIANT_PRIMITIVES_H_227500b5_53ed_4aae_b16f_7d462f91b0a3
#define _VARIANT_PRIMITIVES_H_227500b5_53ed_4aae_b16f_7d462f91b0a3

#include "Variant.h"
#include "mString.h"

/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<char>()
{
  return TypeInfo_Char;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<unsigned char>()
{
  return type_info_of<char>();
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<const char*>()
{
  return TypeInfo_ConstCharArray;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<char*>()
{
  // Store char* as a mString (for ownership)
  return TypeInfo_String;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<mString>()
{
  return TypeInfo_String;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<int>()
{
  return TypeInfo_Int;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<unsigned int>()
{
  return type_info_of<int>();
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<long>()
{
  return TypeInfo_Long;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<unsigned long>()
{
  return type_info_of<long>();
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<bool>()
{
  return TypeInfo_Bool;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<float>()
{
  return TypeInfo_Float;
}
/**
 * type_info_of specialization for primitive types
 */
template <>
inline const TypeInfo& type_info_of<double>()
{
  return TypeInfo_Double;
}

/**
 */
template<>
bool variant_convert(const char& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);
template<>
inline bool variant_convert(const unsigned char& from, 
                            const TypeInfo& toType, 
                            Variant* outVar)
{
  return variant_convert<char>(from, toType, outVar);
}

/**
 */
template<>
bool variant_convert(const char* const& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);
/**
 */
template<>
bool variant_convert(const mString& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);
template<>
inline bool variant_convert(char* const& from, 
                            const TypeInfo& toType, 
                            Variant* outVar)
{
  // char* type is internally stored as mStrings.
  return variant_convert<mString>(from, toType, outVar);
}

/**
 */
template<>
bool variant_convert(const int& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);
template<>
inline bool variant_convert(const unsigned int& from, 
                            const TypeInfo& toType, 
                            Variant* outVar)
{
  return variant_convert<int>(from, toType, outVar);
}

/**
 */
template<>
bool variant_convert(const long& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);
template<>
inline bool variant_convert(const unsigned long& from, 
                            const TypeInfo& toType, 
                            Variant* outVar)
{
  return variant_convert<long>(from, toType, outVar);
}

/**
 */
template<>
bool variant_convert(const bool& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);

/**
 */
template<>
bool variant_convert(const double& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);

/**
 */
template<>
bool variant_convert(const float& from, 
                     const TypeInfo& toType, 
                     Variant* outVar);

#endif // header guard
