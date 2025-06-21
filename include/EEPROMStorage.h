#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "RTClock.h"
#include "Alarm.h"

class EEPROMStorage
{
private:
  static const int MAGIC_NUMBER = 0x1234;
  static const int MAGIC_ADDRESS = 0;
  static const int SETTINGS_ADDRESS = 4;

  struct Settings
  {
    Time time;
    Date date;
    AlarmData alarm;
  };

public:
  EEPROMStorage();

  // Settings storage
  void saveSettings(const Time &time, const Date &date, const AlarmData &alarm);
  bool loadSettings(Time &time, Date &date, AlarmData &alarm);

  // Utility methods
  bool hasValidSettings();
  void clearSettings();

private:
  void writeSettings(const Settings &settings);
  bool readSettings(Settings &settings);
};

#endif