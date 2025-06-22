#include "HTSensor.h"

HTSensor::HTSensor(int pin) : dht(pin, DHTTYPE), pin(pin)
{
}

void HTSensor::begin()
{
  dht.begin();
}

int8_t HTSensor::getTemperature()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastTemperatureUpdate >= UPDATE_INTERVAL)
  {
    int newTemp = dht.readTemperature();
    if (!isnan(newTemp) && newTemp >= -50 && newTemp <= 100)
    {
      temperature = newTemp;
    }
    lastTemperatureUpdate = currentMillis;
  }
  return temperature;
}

int8_t HTSensor::getHumidity()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastHumidityUpdate >= UPDATE_INTERVAL)
  {
    int newHum = dht.readHumidity();
    if (!isnan(newHum) && newHum >= 0 && newHum <= 100)
    {
      humidity = newHum;
    }
    lastHumidityUpdate = currentMillis;
  }
  return humidity;
}
