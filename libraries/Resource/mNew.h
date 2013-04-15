
#ifndef _NEW_H_e241a013_f9c0_4389_92ce_dce6d9c6e71f
#define _NEW_H_e241a013_f9c0_4389_92ce_dce6d9c6e71f

#include "mDefs.h"
#include <string.h>

#ifdef mUseExceptions
//#pragma mCompilerWarning("Throwing new operators unimplemented")
#endif // mUseExceptions

#ifndef mHasNew
void* operator new(size_t size);
#endif // !defined(mHasNew)

#ifndef mHasPlacementNew
void* operator new(size_t size, void* ptr);
#endif // !defined(mHasNew)

#ifndef mHasDelete
void operator delete(void* ptr);
#endif // !defined(mHasDelete)

#endif // Header guard
