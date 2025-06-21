#include "Timer.h"

Timer::Timer() : lastUpdate(0)
{
  reset();
}

void Timer::begin()
{
  lastUpdate = millis();
}

void Timer::start()
{
  if (!data.completed && (data.hour > 0 || data.minute > 0 || data.second > 0))
  {
    data.running = true;
    lastUpdate = millis();
  }
}

void Timer::stop()
{
  data.running = false;
}

void Timer::reset()
{
  data.running = false;
  data.completed = false;
  data.hour = 0;
  data.minute = 0;
  data.second = 0;
  lastUpdate = millis();
}

void Timer::update()
{
  if (data.running && !data.completed)
  {
    unsigned long currentMillis = millis();
    
    // Update every second
    if (currentMillis - lastUpdate >= 1000)
    {
      // Decrease timer
      if (data.second > 0)
      {
        data.second--;
      }
      else if (data.minute > 0)
      {
        data.minute--;
        data.second = 59;
      }
      else if (data.hour > 0)
      {
        data.hour--;
        data.minute = 59;
        data.second = 59;
      }
      else
      {
        data.completed = true;
        data.running = false;
      }
      
      lastUpdate = currentMillis;
    }
  }
}

void Timer::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  data.hour = hour;
  data.minute = minute;
  data.second = second;
  data.completed = false;
}

void Timer::adjustHour()
{
  data.hour = (data.hour + 1) % 24;
}

void Timer::adjustMinute()
{
  data.minute = (data.minute + 1) % 60;
}

void Timer::adjustSecond()
{
  data.second = (data.second + 1) % 60;
}

bool Timer::isRunning() const
{
  return data.running;
}

bool Timer::isCompleted() const
{
  return data.completed;
}

uint8_t Timer::getHour() const
{
  return data.hour;
}

uint8_t Timer::getMinute() const
{
  return data.minute;
}

uint8_t Timer::getSecond() const
{
  return data.second;
}

TimerData Timer::getTime() const
{
  return data;
}

char* Timer::getTimeString() const
{
  static char timerString[6];
  timerString[0] = '0' + data.hour / 10;
  timerString[1] = '0' + data.hour % 10;
  timerString[2] = '0' + data.minute / 10;
  timerString[3] = '0' + data.minute % 10;
  timerString[4] = '0' + data.second / 10;
  timerString[5] = '0' + data.second % 10;
  return timerString;
}