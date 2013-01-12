
#ifndef _RESOURCECOLLECTION_H_
#define _RESOURCECOLLECTION_H_

#include "Resource.h"

/**
 * 
 */
class ResourceCollection
{
public:
  ResourceCollection();
  ~ResourceCollection();
 
  void add(const char* path, IResource& res);
  //ResourceCollection& get(const char* path = NULL);
private:
  const char* mPath;
};

/**
 * Example
 * 
 */

/*
class TemperatureSensorResource : public ResourceBase
{
public:
private:
};

void Test()
{
  TemperatureSensorResource tempSensor;
  ResourceCollection system;
  
  system.add("system.sensors.temperature", tempSensor);
  
  ResourceCollection sensors = system.get("system.sensors");
  for (int idx = 0; idx < sensors.count(), idx++)
  {
    
  }
}
*/

#endif
