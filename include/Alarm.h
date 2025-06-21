#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>
#include "RTClock.h"

// Forward declaration
class Buzzer;

struct AlarmData
{
  uint8_t hour;
  uint8_t minute;
  bool enabled;
};

class Alarm
{
private:
  AlarmData data;
  bool triggered;
  unsigned long triggerStartTime;
  uint16_t ALARM_DURATION = 10000; // 10 seconds
  Buzzer *buzzer;

public:
  Alarm();

  // Initialization
  void begin(Buzzer *buzzer = nullptr);

  // Alarm control
  void enable();
  void disable();
  void stop();

  // Alarm setting
  void setTime(uint8_t hour, uint8_t minute);
  void adjustHour();
  void adjustMinute();

  // Status queries
  bool isEnabled() const;
  bool isTriggered() const;
  bool shouldTrigger(const Time &currentTime) const;

  // Getters
  uint8_t getHour() const;
  uint8_t getMinute() const;
  AlarmData getTime() const;

  // Update method (called every loop)
  void update(const Time &currentTime);
};

#endif // ALARM_H