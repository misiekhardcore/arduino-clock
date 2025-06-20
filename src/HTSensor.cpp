#include "HTSensor.h"

HTSensor::HTSensor(int pin) : dht(pin, DHTTYPE), pin(pin)
{
}

void HTSensor::begin()
{
  dht.begin();
}

float HTSensor::getTemperature()
{
  return dht.readTemperature();
}

float HTSensor::getHumidity()
{
  return dht.readHumidity();
}
