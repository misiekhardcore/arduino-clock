#include "HTSensor.h"

HTSensor::HTSensor(int pin) : dht(pin, DHTTYPE), pin(pin)
{
}

void HTSensor::begin()
{
  dht.begin();
}

void HTSensor::update()
{
  // Sensor updates - only every 6 seconds
  static unsigned long lastSensorUpdate = 0;
  if (millis() - lastSensorUpdate >= 6000) // 6 seconds
  {
    temperature = (int8_t)dht.readTemperature();
    delay(20);
    humidity = (int8_t)dht.readHumidity();

    lastSensorUpdate = millis();
  }
}

int8_t HTSensor::getTemperature()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= UPDATE_INTERVAL)
  {
    int newTemp = dht.readTemperature();
    if (!isnan(newTemp) && newTemp >= -50 && newTemp <= 100)
    {
      temperature = newTemp;
    }
    lastUpdate = currentMillis;
  }
  return temperature;
}

int8_t HTSensor::getHumidity()
{
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate >= UPDATE_INTERVAL)
  {
    int newHum = dht.readHumidity();
    if (!isnan(newHum) && newHum >= 0 && newHum <= 100)
    {
      humidity = newHum;
    }
    lastUpdate = currentMillis;
  }
  return humidity;
}
