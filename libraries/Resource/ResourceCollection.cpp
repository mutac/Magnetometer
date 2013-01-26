
#include "ResourceCollection.h"
#include "IAllocator.h"


template <>
NullAllocator<ResourceCollection::PathTree>& NullAllocator<ResourceCollection::PathTree>::instance()
{
  static NullAllocator<ResourceCollection::PathTree> sTheInstance;
  return sTheInstance;
}
