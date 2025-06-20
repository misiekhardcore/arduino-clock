#ifndef HTSENSOR_H
#define HTSENSOR_H

#include <Arduino.h>
#include <DHT.h>

#define DHTTYPE DHT11

class HTSensor
{
private:
  DHT dht;
  int pin;

public:
  HTSensor(int pin);

  void begin();

  // Getters
  float getTemperature();
  float getHumidity();
};

#endif