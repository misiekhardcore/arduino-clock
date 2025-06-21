#include "Alarm.h"
#include "Buzzer.h"

Alarm::Alarm() : triggered(false), triggerStartTime(0), buzzer(nullptr)
{
  data = {7, 0, false};
}

void Alarm::begin(Buzzer *buzzer)
{
  this->buzzer = buzzer;
  triggered = false;
  triggerStartTime = 0;
}

void Alarm::enable()
{
  data.enabled = true;
}

void Alarm::disable()
{
  data.enabled = false;
  triggered = false;
}

void Alarm::stop()
{
  triggered = false;
  if (buzzer)
  {
    buzzer->stopAlarm();
  }
}

void Alarm::setTime(uint8_t hour, uint8_t minute)
{
  data.hour = hour;
  data.minute = minute;
}

void Alarm::adjustHour()
{
  data.hour = (data.hour + 1) % 24;
}

void Alarm::adjustMinute()
{
  data.minute = (data.minute + 1) % 60;
}

bool Alarm::isEnabled() const
{
  return data.enabled;
}

bool Alarm::isTriggered() const
{
  return triggered;
}

bool Alarm::shouldTrigger(const Time &currentTime) const
{
  return data.enabled &&
         !triggered &&
         currentTime.hour == data.hour &&
         currentTime.minute == data.minute &&
         currentTime.second < 30;
}

uint8_t Alarm::getHour() const
{
  return data.hour;
}

uint8_t Alarm::getMinute() const
{
  return data.minute;
}

AlarmData Alarm::getTime() const
{
  return data;
}

void Alarm::update(const Time &currentTime)
{
  // Check if alarm should trigger
  if (shouldTrigger(currentTime))
  {
    triggered = true;
    triggerStartTime = millis();
  }

  // Handle buzzer when triggered
  if (triggered && buzzer)
  {
    buzzer->playAlarm();
  }

  // Auto-stop alarm after duration
  if (triggered && millis() - triggerStartTime >= ALARM_DURATION)
  {
    triggered = false;
    if (buzzer)
    {
      buzzer->stopAlarm();
    }
  }

  // Update buzzer state
  if (buzzer)
  {
    buzzer->update();
  }
}