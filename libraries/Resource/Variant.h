

#ifndef _VARIANT_H_
#define _VARIANT_H_

#include "Defs.h"

// #include <var/variant.hpp>

/** Defines what the variant can hold.
 *  Keep the VariantType_* identifiers up-to-date.
 */
// typedef ttl::var::variant<float, int, char*> VarType;

/** List of supported types, number implies index within VarType template
 */
enum VariantTypes
{
  VariantTypes_float = 0,
  VariantTypes_int = 1,
  VariantTypes_charPtr = 2
};

/**
 */
class Variant /* : public VarType */
{
public:
  template<typename T>
  inline bool isType() const;

  /** 
   * Pre-defined variant constants 
   */
  static Variant Empty;

private:
  int which() const { return 1; }
};

template<>
inline bool Variant::isType<float>() const
{
  return which() == (int)VariantTypes_float;
}

template<>
inline bool Variant::isType<int>() const
{
  return which() == (int)VariantTypes_int;
}

template<>
inline bool Variant::isType<char*>() const
{
  return which() == (int)VariantTypes_charPtr;
}

/**
 */
class VariantVisitor 
{
  virtual void operator()(float val) {}
  virtual void operator()(int val) {}
  virtual void operator()(char* val) {}
};

#endif
