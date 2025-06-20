#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>

// Forward declarations
class RTClock;
class HTSensor;

struct Time
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
};

struct Date
{
  uint8_t day;
  uint8_t month;
  uint16_t year;
};

struct AlarmTime
{
  uint8_t hour;
  uint8_t minute;
  bool enabled;
};

struct Timer
{
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  bool running;
  bool completed;
};

class Clock
{
private:
  RTClock *rtc;
  HTSensor *dht11;

  Time currentTime;
  Date currentDate;
  AlarmTime alarmTime;
  Timer timer;

  float temperature;
  float humidity;

  unsigned long lastUpdate;
  unsigned long lastTempUpdate;
  const unsigned long TEMP_UPDATE_INTERVAL = 2000; // Update temperature every 2 seconds

  bool alarmTriggered;
  unsigned long alarmStartTime;
  const unsigned long ALARM_DURATION = 10000; // 10 seconds

public:
  Clock();

  void begin(RTClock *rtc, HTSensor *dht11);
  void update();

  // Getters
  Time getTime() const;
  Date getDate() const;
  AlarmTime getAlarmTime() const;
  Timer getTimer() const;
  float getTemperature() const;
  float getHumidity() const;

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

  // Alarm
  bool isAlarmTriggered() const;
  void stopAlarm();

  // Timer
  void startTimer();
  void stopTimer();
  void resetTimer();
};

#endif