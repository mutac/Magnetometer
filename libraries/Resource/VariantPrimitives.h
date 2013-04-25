
#ifndef _VARIANT_PRIMITIVES_H_227500b5_53ed_4aae_b16f_7d462f91b0a3
#define _VARIANT_PRIMITIVES_H_227500b5_53ed_4aae_b16f_7d462f91b0a3

#include "Variant.h"

/**
 * variant_type_info specialization for primitive types
 */
template <>
inline const Variant::TypeInfo& variant_type_info<char>()
{
  return Variant::TypeInfo_Char;
}
/**
 * variant_type_info specialization for primitive types
 */
template <>
inline const Variant::TypeInfo& variant_type_info<unsigned char>()
{
  return variant_type_info<char>();
}
/**
 * variant_type_info specialization for primitive types
 */
template <>
inline const Variant::TypeInfo& variant_type_info<mString>()
{
  return Variant::TypeInfo_String;
}
/**
 * variant_type_info specialization for primitive types
 */
template <>
inline const Variant::TypeInfo& variant_type_info<int>()
{
  return Variant::TypeInfo_Int;
}
/**
 * variant_type_info specialization for primitive types
 */
template <>
inline const Variant::TypeInfo& variant_type_info<unsigned int>()
{
  return variant_type_info<int>();
}
/**
 * variant_type_info specialization for primitive types
 */
template <>
inline const Variant::TypeInfo& variant_type_info<float>()
{
  return Variant::TypeInfo_Float;
}
/**
 * variant_type_info specialization for primitive types
 */
template <>
inline const Variant::TypeInfo& variant_type_info<double>()
{
  return Variant::TypeInfo_Double;
}

/**
 */
template<>
bool variant_convert(const char& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar);
template<>
inline bool variant_convert(const unsigned char& from, 
                            const Variant::TypeInfo& toType, 
                            Variant* outVar)
{
  return variant_convert<char>(from, toType, outVar);
}

/**
 */
template<>
bool variant_convert(const int& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar);
template<>
inline bool variant_convert(const unsigned int& from, 
                            const Variant::TypeInfo& toType, 
                            Variant* outVar)
{
  return variant_convert<int>(from, toType, outVar);
}

/**
 */
template<>
bool variant_convert(const double& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar);

/**
 */
template<>
bool variant_convert(const float& from, 
                     const Variant::TypeInfo& toType, 
                     Variant* outVar);

#endif // header guard
