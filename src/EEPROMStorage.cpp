#include "EEPROMStorage.h"
#include <EEPROM.h>

EEPROMStorage::EEPROMStorage()
{
}

void EEPROMStorage::saveSettings(const Time &time, const Date &date, const AlarmData &alarm)
{
  Settings settings;
  settings.time = time;
  settings.date = date;
  settings.alarm = alarm;

  // Write magic number
  EEPROM.put(MAGIC_ADDRESS, MAGIC_NUMBER);

  // Write settings
  writeSettings(settings);
}

bool EEPROMStorage::loadSettings(Time &time, Date &date, AlarmData &alarm)
{
  if (!hasValidSettings())
  {
    return false;
  }

  Settings settings;
  if (readSettings(settings))
  {
    time = settings.time;
    date = settings.date;
    alarm = settings.alarm;
    return true;
  }

  return false;
}

bool EEPROMStorage::hasValidSettings()
{
  int magic;
  EEPROM.get(MAGIC_ADDRESS, magic);
  return magic == MAGIC_NUMBER;
}

void EEPROMStorage::clearSettings()
{
  for (int i = 0; i < (int)EEPROM.length(); i++)
  {
    EEPROM.write(i, 0);
  }
}

void EEPROMStorage::writeSettings(const Settings &settings)
{
  EEPROM.put(SETTINGS_ADDRESS, settings);
}

bool EEPROMStorage::readSettings(Settings &settings)
{
  EEPROM.get(SETTINGS_ADDRESS, settings);

  // Validate settings
  if (settings.time.hour > 23 || settings.time.minute > 59 || settings.time.second > 59)
  {
    return false;
  }

  if (settings.date.day > 31 || settings.date.month > 12 || settings.date.year < 2020)
  {
    return false;
  }

  if (settings.alarm.hour > 23 || settings.alarm.minute > 59)
  {
    return false;
  }

  return true;
}