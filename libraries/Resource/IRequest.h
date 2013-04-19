
#ifndef _IREQUEST_H_96ba8dda_754b_40ed_8355_65ed824d82c3
#define _IREQUEST_H_96ba8dda_754b_40ed_8355_65ed824d82c3

#include "mDefs.h"
#include "Delegate.h"
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

/**
 */
typedef Delegate<void, IRequest&> RequestHandler;

/**
 */
typedef PathCollection<RequestHandler> ResourceCollection;

class Request
{
public:
  bool static Invoke(const ResourceCollection& root, 
                     const char* path,
                     IRequest& request)
  {
    ResourceCollection found = root.find(path);
    
    if (found.begin() == found.end())
    {
      return false;
    }

    for (ResourceCollection::Iterator it = found.begin(); 
      it != found.end();
      it++)
    {
      it->invoke(request);
    }

    return true;
  }
};

#endif // Header guard
