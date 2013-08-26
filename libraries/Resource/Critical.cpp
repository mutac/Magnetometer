
#include "Critical.h"

#ifdef mPlatformWindows
#include <Windows.h>

#define mGetSection_(sec) \
  reinterpret_cast<CRITICAL_SECTION*>(sec)

CriticalSection::CriticalSection()
{
  // Ensure that the size reserved to store the critical
  // section is large enough for the Windows CRITCAL_SECTION type.
  mCompilerAssert(SizeOfCriticalSectionWindows, 
    sizeof(mSection) >= sizeof(CRITICAL_SECTION));

  InitializeCriticalSection(mGetSection_(mSection));
}

CriticalSection::~CriticalSection()
{
  DeleteCriticalSection(mGetSection_(mSection));
}

void CriticalSection::enter()
{
  EnterCriticalSection(mGetSection_(mSection));
}

void CriticalSection::exit()
{
  LeaveCriticalSection(mGetSection_(mSection));
}

#else // mPlatformWindows

mCompilerWarning(No Default critical section implementation available)

CriticalSection::CriticalSection()
{
}

CriticalSection::~CriticalSection()
{
}

void CriticalSection::enter()
{
}

void CriticalSection::exit()
{
}

#endif // default implementation