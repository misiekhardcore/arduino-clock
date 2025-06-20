#include "Clock.h"
#include "RTClock.h"
#include "HTSensor.h"
#include "EEPROMStorage.h"

Clock::Clock() : rtc(nullptr), dht11(nullptr), alarmTriggered(false), alarmStartTime(0)
{
  // Initialize with default values
  currentTime = {12, 0, 0};
  currentDate = {1, 1, 2024};
  alarmTime = {7, 0, false};
  timer = {0, 0, 0, false, false};
  temperature = 0.0;
  humidity = 0.0;
  lastUpdate = 0;
  lastTempUpdate = 0;
}

void Clock::begin(RTClock *rtc, HTSensor *dht11)
{
  this->rtc = rtc;
  this->dht11 = dht11;
  lastUpdate = millis();
  lastTempUpdate = millis();
}

void Clock::update()
{
  unsigned long currentMillis = millis();

  // Update time from RTC
  currentTime = rtc->getTime();
  currentDate = rtc->getDate();

  // Update temperature and humidity from sensor
  temperature = dht11->getTemperature();
  humidity = dht11->getHumidity();

  // Check alarm (matching old Arduino program exactly)
  if (alarmTime.enabled && !alarmTriggered)
  {
    if (currentTime.hour == alarmTime.hour && currentTime.minute == alarmTime.minute && currentTime.second < 30)
    {
      alarmTriggered = true;
      alarmStartTime = currentMillis;
    }
  }

  // Auto-stop alarm after duration
  if (alarmTriggered && currentMillis - alarmStartTime >= ALARM_DURATION)
  {
    alarmTriggered = false;
  }

  // Update timer
  if (timer.running && !timer.completed)
  {
    // Decrease timer
    if (timer.second > 0)
    {
      timer.second--;
    }
    else if (timer.minute > 0)
    {
      timer.minute--;
      timer.second = 59;
    }
    else if (timer.hour > 0)
    {
      timer.hour--;
      timer.minute = 59;
      timer.second = 59;
    }
    else
    {
      timer.completed = true;
      timer.running = false;
    }
  }

  lastUpdate = currentMillis;
}

Time Clock::getTime() const
{
  return currentTime;
}

Date Clock::getDate() const
{
  return currentDate;
}

AlarmTime Clock::getAlarmTime() const
{
  return alarmTime;
}

Timer Clock::getTimer() const
{
  return timer;
}

float Clock::getTemperature() const
{
  return temperature;
}

float Clock::getHumidity() const
{
  return humidity;
}

char *Clock::getTimeString() const
{
  static char timeString[6];
  timeString[0] = '0' + currentTime.hour / 10;
  timeString[1] = '0' + currentTime.hour % 10;
  timeString[2] = '0' + currentTime.minute / 10;
  timeString[3] = '0' + currentTime.minute % 10;
  timeString[4] = '0' + currentTime.second / 10;
  timeString[5] = '0' + currentTime.second % 10;
  return timeString;
}

char *Clock::getDateString() const
{
  static char dateString[6];
  dateString[0] = '0' + currentDate.day / 10;
  dateString[1] = '0' + currentDate.day % 10;
  dateString[2] = '0' + currentDate.month / 10;
  dateString[3] = '0' + currentDate.month % 10;
  dateString[4] = '0' + (currentDate.year % 100) / 10;
  dateString[5] = '0' + (currentDate.year % 100) % 10;
  return dateString;
}

char *Clock::getAlarmTimeString() const
{
  static char alarmString[6];
  alarmString[0] = '0' + alarmTime.hour / 10;
  alarmString[1] = '0' + alarmTime.hour % 10;
  alarmString[2] = '0' + alarmTime.minute / 10;
  alarmString[3] = '0' + alarmTime.minute % 10;
  alarmString[4] = '0';
  alarmString[5] = '0';
  return alarmString;
}

char *Clock::getTimerString() const
{
  static char timerString[6];
  timerString[0] = '0' + timer.hour / 10;
  timerString[1] = '0' + timer.hour % 10;
  timerString[2] = '0' + timer.minute / 10;
  timerString[3] = '0' + timer.minute % 10;
  timerString[4] = '0' + timer.second / 10;
  timerString[5] = '0' + timer.second % 10;
  return timerString;
}

char *Clock::getTemperatureString() const
{
  static char temperatureString[6];
  temperatureString[0] = ' ';
  temperatureString[1] = ' ';
  temperatureString[2] = '0' + (int)temperature / 10;
  temperatureString[3] = '0' + (int)temperature % 10;
  temperatureString[4] = '*';
  temperatureString[5] = 'C';
  return temperatureString;
}

char *Clock::getHumidityString() const
{
  static char humidityString[6];
  humidityString[0] = ' ';
  humidityString[1] = ' ';
  humidityString[2] = '0' + (int)this->humidity / 10;
  humidityString[3] = '0' + (int)this->humidity % 10;
  humidityString[4] = '*';
  humidityString[5] = 'H';
  return humidityString;
}

void Clock::adjustSetting(int setting, int part)
{
  switch (setting)
  {
  case 0: // Time
    switch (part)
    {
    case 0: // Hour
      currentTime.hour = (currentTime.hour + 1) % 24;
      if (rtc)
        rtc->setTime(currentTime);
      break;
    case 1: // Minute
      currentTime.minute = (currentTime.minute + 1) % 60;
      if (rtc)
        rtc->setTime(currentTime);
      break;
    case 2: // Second
      currentTime.second = (currentTime.second + 1) % 60;
      if (rtc)
        rtc->setTime(currentTime);
      break;
    }
    break;

  case 1: // Date
    switch (part)
    {
    case 0: // Day
      currentDate.day = (currentDate.day % 31) + 1;
      if (rtc)
        rtc->setDate(currentDate);
      break;
    case 1: // Month
      currentDate.month = (currentDate.month % 12) + 1;
      if (rtc)
        rtc->setDate(currentDate);
      break;
    case 2: // Year
      currentDate.year = (currentDate.year % 100) + 2024;
      if (rtc)
        rtc->setDate(currentDate);
      break;
    }
    break;

  case 2: // Alarm
    switch (part)
    {
    case 0: // Hour
      alarmTime.hour = (alarmTime.hour + 1) % 24;
      break;
    case 1: // Minute
      alarmTime.minute = (alarmTime.minute + 1) % 60;
      break;
    case 2: // Enable/Disable (not used in old program, but keeping for compatibility)
      alarmTime.enabled = !alarmTime.enabled;
      break;
    }
    break;
  }
}

void Clock::loadSettings()
{
  EEPROMStorage eeprom;
  if (eeprom.hasValidSettings())
  {
    eeprom.loadSettings(currentTime, currentDate, alarmTime, timer);
  }
}

void Clock::saveSettings()
{
  EEPROMStorage eeprom;
  eeprom.saveSettings(currentTime, currentDate, alarmTime, timer);
}

bool Clock::isAlarmTriggered() const
{
  return alarmTriggered;
}

void Clock::stopAlarm()
{
  alarmTriggered = false;
}

void Clock::startTimer()
{
  if (!timer.completed)
  {
    timer.running = true;
  }
}

void Clock::stopTimer()
{
  timer.running = false;
}

void Clock::resetTimer()
{
  timer.running = false;
  timer.completed = false;
  timer.hour = 0;
  timer.minute = 0;
  timer.second = 0;
}