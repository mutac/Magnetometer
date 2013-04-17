
#ifndef _IREQUEST_H_96ba8dda_754b_40ed_8355_65ed824d82c3
#define _IREQUEST_H_96ba8dda_754b_40ed_8355_65ed824d82c3

#include "mDefs.h"
#include "PathCollection.h"

/**
 *
 */
class IRequest
{
public:
  virtual void beginResponse() = 0;
  virtual void endResponse() = 0;
};

class IRequestHandler
{
public:
  typedef void (IRequestHandler::*handler)(IRequest* request); 
  //void nil() {}
};

/**
 * 
 */
class RequestHandler
{
public:
  RequestHandler(IRequestHandler* instance, IRequestHandler::handler handler) :
    mInstance(instance),
    mHandler(handler)
  {
  }

  bool invoke(IRequest* request)
  {
    mDebugAssert(mInstance != NULL);
    mDebugAssert(mHandler != NULL);
    
    // TODO: This doesn't seem right.  Can you invoke a function
    // pointer given only a pointer to the base?
    (mInstance->*mHandler)(request);
  }

protected:
  IRequestHandler* mInstance;
  IRequestHandler::handler mHandler;
};

/**
 */
typedef PathCollection<RequestHandler> ResourceCollection;

#endif // Header guard
