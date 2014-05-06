
#ifndef __TYPE_INFO_H__
#define __TYPE_INFO_H__

namespace mStd
{
  /**
  * Variant type identifier.
  */
  typedef unsigned char TypeInfo;

  /**
  * Type identifiers for build-in types.
  */
  static const TypeInfo TypeInfo_Empty = 0;
  static const TypeInfo TypeInfo_Char = 1;
  static const TypeInfo TypeInfo_ConstCharArray = 2;
  static const TypeInfo TypeInfo_Int = 3;
  static const TypeInfo TypeInfo_Long = 4;
  static const TypeInfo TypeInfo_Bool = 5;
  static const TypeInfo TypeInfo_Float = 6;
  static const TypeInfo TypeInfo_Double = 7;
  static const TypeInfo TypeInfo_String = 8;
  /** Begin user types at 20 */

  /**
  * Returns variant type information associated with a type.
  * Specialize for your type.
  */
  template <typename ValueType>
  const TypeInfo& type_of();
}

#endif
