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
  unsigned long lastUpdate = 0;
  int8_t temperature = 0;
  int8_t humidity = 0;
  static const unsigned long UPDATE_INTERVAL = 3000;

public:
  HTSensor(int pin);

  void begin();
  void update();

  // Getters
  int8_t getTemperature();
  int8_t getHumidity();
};

#endif