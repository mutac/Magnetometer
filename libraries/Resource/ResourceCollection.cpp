
#include "ResourceCollection.h"
#include "IAllocator.h"


template <>
NullAllocator<ResourceCollection::Container>& NullAllocator<ResourceCollection::Container>::instance()
{
  static NullAllocator<ResourceCollection::Container> sTheInstance;
  return sTheInstance;
}
