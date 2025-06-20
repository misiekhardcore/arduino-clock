#ifndef EEPROM_STORAGE_H
#define EEPROM_STORAGE_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Clock.h"

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
    AlarmTime alarm;
    Timer timer;
  };

public:
  EEPROMStorage();

  // Settings storage
  void saveSettings(const Time &time, const Date &date, const AlarmTime &alarm, const Timer &timer);
  bool loadSettings(Time &time, Date &date, AlarmTime &alarm, Timer &timer);

  // Utility methods
  bool hasValidSettings();
  void clearSettings();

private:
  void writeSettings(const Settings &settings);
  bool readSettings(Settings &settings);
};

#endif