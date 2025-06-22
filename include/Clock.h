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

  // RTC
  void setTime(const Time &time);
  void setDate(const Date &date);

  // Settings
  void adjustSetting(int setting, int part);
  void loadSettings();
  void saveSettings();

  // Alarm
  bool isAlarmTriggered() const;
  void stopAlarm();
  void setAlarmTime(uint8_t hour, uint8_t minute);
  void enableAlarm();
  void disableAlarm();
  void setAlarmData(const AlarmData &alarmData);

  // Timer
  void startTimer();
  void stopTimer();
  void resetTimer();
  void setTimerTime(uint8_t hour, uint8_t minute, uint8_t second);
};

#endif