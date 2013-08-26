
#ifndef _CRITICAL_H_897a82ea_5480_4d57_af34_88b0254b46c8
#define _CRITICAL_H_897a82ea_5480_4d57_af34_88b0254b46c8

#include "mDefs.h"

class CriticalSection
{
public:
  CriticalSection();
  ~CriticalSection();

  void enter();
  void exit();

  class Scope;

private:
  // Storage for critical section:
  // Hide platform type details from client.
  char mSection[24];
};

class CriticalSection::Scope
{
public:
  Scope(CriticalSection& section) :
    mSection(&section)
  {
    mSection->enter();
  }

  ~Scope()
  {
    dispose();
  }

  void dispose()
  {
    if (mSection)
    {
      mSection->exit();
      mSection = NULL;
    }
  }

private:
  CriticalSection* mSection;
};

#endif
