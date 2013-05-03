
#ifndef _IREQUEST_H_96ba8dda_754b_40ed_8355_65ed824d82c3
#define _IREQUEST_H_96ba8dda_754b_40ed_8355_65ed824d82c3

#include "mDefs.h"
#include "Delegate.h"
#include "PathCollection.h"
#include "Variant.h"

/**
 */
class IResponse
{
public:
  virtual bool write(const char* name, const Variant& val) = 0;
  virtual void setFailure(const char* reason = "") = 0;
  virtual bool failed() const = 0;
};

/**
 */
class StoredResponse : public IResponse
{
public:
  StoredResponse() :
    mFailed(false)
  {
  }

  ~StoredResponse()
  {
  }

  bool write(const char* name, const Variant& val)
  {
    mVal = val;
    return true;
  }

  void setFailure(const char* reason = "")
  {
    mFailed = true;
  }

  bool failed() const
  {
    return mFailed;
  }

protected:
  bool mFailed;
  const char* mName;
  Variant mVal;
};

/**
 */
class NullResponse : public IResponse
{
public:
  NullResponse() :
    mFailed(false)
  {
  }

  ~NullResponse()
  {
  }

  bool write(const char* name, const Variant& val)
  {
    return true;
  }

  void setFailure(const char* reason = "")
  {
    mFailed = true;
  }

  bool failed() const
  {
    return mFailed;
  }

protected:
  bool mFailed;
};

/**
 */
class IRequest
{
public:
  enum RequestType
  {
    /** Set property request */
    eSet,
    /** Get property request */
    eGet,
    /** Invoke/call method request */
    eInvoke
  };

  virtual IResponse* sender() = 0;
  virtual RequestType type() const = 0;
  virtual const Variant& getArgument() const = 0;
  virtual bool failed() const = 0;
};

/**
 *
 */
class Request : public IRequest
{
public:
  Request(RequestType type, IResponse* response) :
    mType(type),
    mResponse(response)
  {
    mDebugAssert(response != NULL);
  }

  void setArgument(const Variant& arg) { mArg = arg; }
  const Variant& getArgument() const { return mArg; }
  IResponse* sender() { return mResponse; }
  RequestType type() const { return mType; }
  bool failed() const { return mResponse->failed(); }

protected:
  RequestType mType;
  IResponse* mResponse;
  Variant mArg;
};


class Resources
{
public:
  Resources()
  {
  }

  virtual ~Resources()
  {
  }

  template <typename MethodType>
  bool add(const char* path, MethodType method)
  {
    return mRoot.add(path, new RequestHandler(method));
  }

  template <typename ObjectType, typename MethodType>
  bool add(const char* path, ObjectType* object, MethodType method)
  {
    return mRoot.add(path, new RequestHandler(object, method));
  }

  template <typename ArgType>
  bool set(const char* path, ArgType val)
  {
    StoredResponse response;
    Request request(IRequest::eSet, &response);
    request.setArgument(val);

    return invoke(path, &request);
  }

  bool invoke(const char* path, IResponse* response = NULL)
  {
    NullResponse nil;

    if (response == NULL)
    {
      response = &nil;
    }

    Request request(IRequest::eInvoke, response);

    return invoke(path, &request);
  }

protected:
  bool invoke(const char* path, IRequest* request)
  {
    ResourceCollection found = mRoot.find(path);

    if (found.empty())
    {
      return false;
    }

    bool overall = true;
    for (ResourceCollection::Iterator it = found.begin();
      it != found.end(); ++it)
    {
      it->invoke(request);
      if (request->failed())
      {
        overall = false;
      }
    }

    return overall;
  }

  typedef Delegate<void, IRequest*> RequestHandler;
  typedef PathCollection<RequestHandler> ResourceCollection;

  ResourceCollection mRoot;
};

#endif // Header guard
