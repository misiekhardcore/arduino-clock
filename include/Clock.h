#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include "RTClock.h"
#include "Timer.h"
#include "Alarm.h"
#include "HTSensor.h"


// Forward declarations
class Buzzer;

class Clock
{
private:
  RTClock *rtc;
  HTSensor *dht11;
  Buzzer *buzzer;
  Alarm alarm;
  Timer timer;

public:
  Clock();

  void begin(RTClock *rtc, HTSensor *dht11, Buzzer *buzzer);
  void update();

  // Getters
  Time getTime() const;
  Date getDate() const;
  AlarmData getAlarmTime();
  TimerData getTimerTime();
  int8_t getTemperature() const;
  int8_t getHumidity() const;

  char *getTimeString() const;
  char *getDateString() const;
  char *getAlarmTimeString() const;
  char *getTimerString() const;
  char *getTemperatureString() const;
  char *getHumidityString() const;

  // Settings
  void adjustSetting(int setting, int part);
  void loadSettings();
  void saveSettings();

  // Alarm (delegated to Alarm class)
  bool isAlarmTriggered() const;
  void stopAlarm();

  // Timer (delegated to Timer class)
  void startTimer();
  void stopTimer();
  void resetTimer();
};

#endif