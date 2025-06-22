#include "SerialCommandHandler.h"
#include "Clock.h"

SerialCommandHandler::SerialCommandHandler()
    : clock(nullptr), waitingForTimeInput(false), waitingForDateInput(false)
{
}

void SerialCommandHandler::begin(Clock *clock)
{
  this->clock = clock;
  Serial.println(F("Type 'help' for available commands"));
}

void SerialCommandHandler::update()
{
  handleSerialInput();
}

void SerialCommandHandler::handleSerialInput()
{
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (waitingForTimeInput)
    {
      parseTimeInput(input);
      waitingForTimeInput = false;
      return;
    }

    if (waitingForDateInput)
    {
      parseDateInput(input);
      waitingForDateInput = false;
      return;
    }

    processCommand(input);
  }
}

void SerialCommandHandler::processCommand(const String &command)
{
  if (command.length() == 0)
    return;

  String cmd = command;
  cmd.toLowerCase();

  // Basic commands
  if (cmd == "help" || cmd == "h")
  {
    showHelp();
  }
  else if (cmd == "status" || cmd == "s")
  {
    showStatus();
  }
  // Time commands
  else if (cmd == "time" || cmd == "t")
  {
    setRTCTime();
  }
  else if (cmd.startsWith("time "))
  {
    handleTimeCommand(cmd.substring(5));
  }
  // Date commands
  else if (cmd == "date" || cmd == "d")
  {
    setRTCDate();
  }
  else if (cmd.startsWith("date "))
  {
    handleDateCommand(cmd.substring(5));
  }
  // Alarm commands
  else if (cmd == "alarm" || cmd == "a")
  {
    showAlarmHelp();
  }
  else if (cmd.startsWith("alarm "))
  {
    handleAlarmCommand(cmd.substring(6));
  }
  // Timer commands
  else if (cmd == "timer" || cmd == "tr")
  {
    showTimerHelp();
  }
  else if (cmd.startsWith("timer "))
  {
    handleTimerCommand(cmd.substring(6));
  }
  else
  {
    Serial.print(F("Unknown command: '"));
    Serial.print(command);
    Serial.println(F("'. Type 'help' for available commands."));
  }
}

void SerialCommandHandler::handleTimeCommand(const String &timeStr)
{
  if (timeStr.length() != 4 && timeStr.length() != 6)
  {
    Serial.println(F("Invalid time format. Use HHMM(SS)"));
    return;
  }

  Time time = parseTimeString(timeStr);
  if (isValidTime(time))
  {
    clock->setTime(time);
    Serial.print(F("Time set to: "));
    Serial.println(formatTime(time.hour, time.minute, time.second));
  }
  else
  {
    Serial.println(F("Invalid time values. Hour: 0-23, Minute: 0-59"));
  }
}

void SerialCommandHandler::handleDateCommand(const String &dateStr)
{
  if (dateStr.length() != 8)
  {
    Serial.println(F("Invalid date format. Use DDMMYYYY"));
    return;
  }

  Date date = parseDateString(dateStr);
  if (isValidDate(date))
  {
    clock->setDate(date);
    Serial.print(F("Date set to: "));
    Serial.println(formatDate(date.day, date.month, date.year));
  }
  else
  {
    Serial.println(F("Invalid date values. Day: 1-31, Month: 1-12, Year: 2000-2099"));
  }
}

void SerialCommandHandler::handleAlarmCommand(const String &alarmCmd)
{
  if (alarmCmd == "on")
  {
    clock->enableAlarm();
    Serial.println(F("Alarm enabled"));
  }
  else if (alarmCmd == "off")
  {
    clock->disableAlarm();
    Serial.println(F("Alarm disabled"));
  }
  else if (alarmCmd.startsWith("set "))
  {
    handleAlarmSetCommand(alarmCmd.substring(4));
  }
  else
  {
    Serial.println(F("Invalid alarm command. Use 'alarm' for help."));
  }
}

void SerialCommandHandler::handleAlarmSetCommand(const String &timeStr)
{
  if (timeStr.length() != 4)
  {
    Serial.println(F("Invalid time format. Use HHMM"));
    return;
  }

  int hour = timeStr.substring(0, 2).toInt();
  int minute = timeStr.substring(2, 4).toInt();

  if (isValidTimeValues(hour, minute, 0))
  {
    clock->setAlarmTime(hour, minute);
    Serial.print(F("Alarm set to: "));
    Serial.println(formatTime(hour, minute, 0));
  }
  else
  {
    Serial.println(F("Invalid time values. Hour: 0-23, Minute: 0-59"));
  }
}

void SerialCommandHandler::handleTimerCommand(const String &timerCmd)
{
  if (timerCmd == "start")
  {
    clock->startTimer();
    Serial.println(F("Timer started"));
  }
  else if (timerCmd == "stop")
  {
    clock->stopTimer();
    Serial.println(F("Timer stopped"));
  }
  else if (timerCmd == "reset")
  {
    clock->resetTimer();
    Serial.println(F("Timer reset"));
  }
  else if (timerCmd.startsWith("set "))
  {
    handleTimerSetCommand(timerCmd.substring(4));
  }
  else
  {
    Serial.println(F("Invalid timer command. Use 'timer' for help."));
  }
}

void SerialCommandHandler::handleTimerSetCommand(const String &timeStr)
{
  if (timeStr.length() != 6)
  {
    Serial.println(F("Invalid time format. Use HHMMSS"));
    return;
  }

  Time time = parseTimeString(timeStr);
  if (isValidTime(time))
  {
    clock->setTimerTime(time.hour, time.minute, time.second);
    Serial.print(F("Timer set to: "));
    Serial.println(formatTime(time.hour, time.minute, time.second));
  }
  else
  {
    Serial.println(F("Invalid time values. Hour: 0-23, Minute: 0-59, Second: 0-59"));
  }
}

Time SerialCommandHandler::parseTimeString(const String &timeStr)
{
  if (timeStr.length() == 4)
  {
    return {
        static_cast<uint8_t>(timeStr.substring(0, 2).toInt()),
        static_cast<uint8_t>(timeStr.substring(2, 4).toInt()),
        static_cast<uint8_t>(0)};
  }
  return {
      static_cast<uint8_t>(timeStr.substring(0, 2).toInt()),
      static_cast<uint8_t>(timeStr.substring(2, 4).toInt()),
      static_cast<uint8_t>(timeStr.substring(4, 6).toInt())};
}

Date SerialCommandHandler::parseDateString(const String &dateStr)
{
  return {
      static_cast<uint8_t>(dateStr.substring(0, 2).toInt()),
      static_cast<uint8_t>(dateStr.substring(2, 4).toInt()),
      static_cast<uint16_t>(dateStr.substring(4, 8).toInt())};
}

bool SerialCommandHandler::isValidTime(const Time &time)
{
  return isValidTimeValues(time.hour, time.minute, time.second);
}

bool SerialCommandHandler::isValidTimeValues(int hour, int minute, int second)
{
  return (hour >= 0 && hour <= 23 &&
          minute >= 0 && minute <= 59 &&
          second >= 0 && second <= 59);
}

bool SerialCommandHandler::isValidDate(const Date &date)
{
  return (date.day >= 1 && date.day <= 31 &&
          date.month >= 1 && date.month <= 12 &&
          date.year >= 2000 && date.year <= 2099);
}

void SerialCommandHandler::showHelp()
{
  Serial.println(F("Available commands:"));
  Serial.println(F("  help, h          - Show this help"));
  Serial.println(F("  status, s        - Show current time and sensor data"));
  Serial.println(F("  time, t          - Set RTC time"));
  Serial.println(F("  date, d          - Set RTC date"));
  Serial.println(F("  alarm, a         - Show alarm commands"));
  Serial.println(F("  timer, tr        - Show timer commands"));
  Serial.println(F(""));
  Serial.println(F("Time format: HHMMSS (24-hour)"));
  Serial.println(F("Date format: DDMMYYYY"));
}

void SerialCommandHandler::showAlarmHelp()
{
  Serial.println(F("Alarm commands:"));
  Serial.println(F("  alarm on - Enable alarm"));
  Serial.println(F("  alarm off - Disable alarm"));
  Serial.println(F("  alarm set HHMM - Set alarm time"));
}

void SerialCommandHandler::showTimerHelp()
{
  Serial.println(F("Timer commands:"));
  Serial.println(F("  timer start - Start timer"));
  Serial.println(F("  timer stop - Stop timer"));
  Serial.println(F("  timer reset - Reset timer"));
  Serial.println(F("  timer set HHMMSS - Set timer duration"));
}

void SerialCommandHandler::showStatus()
{
  Time currentTime = clock->getTime();
  Date currentDate = clock->getDate();
  int8_t temperature = clock->getTemperature();
  int8_t humidity = clock->getHumidity();

  Serial.println(F("=== Clock Status ==="));
  Serial.print(F("Time: "));
  Serial.println(formatTime(currentTime.hour, currentTime.minute, currentTime.second));

  Serial.print(F("Date: "));
  Serial.println(formatDate(currentDate.day, currentDate.month, currentDate.year));

  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C"));
  Serial.print(F("Humidity: "));
  Serial.print(humidity);
  Serial.println(F("%"));

  // Show alarm status
  AlarmData alarmData = clock->getAlarmTime();
  Serial.print(F("Alarm: "));
  if (alarmData.enabled)
  {
    Serial.println(formatTime(alarmData.hour, alarmData.minute, 0));
  }
  else
  {
    Serial.println(F("Disabled"));
  }

  // Show timer status
  TimerData timerData = clock->getTimerTime();
  Serial.print(F("Timer: "));
  if (timerData.running)
  {
    Serial.println(formatTime(timerData.hour, timerData.minute, timerData.second));
    Serial.println(F(" (Running)"));
  }
  else if (timerData.completed)
  {
    Serial.println(F("Completed"));
  }
  else
  {
    Serial.println(formatTime(timerData.hour, timerData.minute, timerData.second));
    Serial.println(F(" (Stopped)"));
  }
}

void SerialCommandHandler::setRTCTime()
{
  Serial.println(F("Enter time in HHMMSS format (24-hour):"));
  waitingForTimeInput = true;
}

void SerialCommandHandler::setRTCDate()
{
  Serial.println(F("Enter date in DDMMYYYY format:"));
  waitingForDateInput = true;
}

void SerialCommandHandler::parseTimeInput(const String &input)
{
  if (input.length() == 6 && isValidTimeFormat(input))
  {
    Time time = parseTimeString(input);
    clock->setTime(time);
    Serial.print(F("Time set to: "));
    Serial.println(formatTime(time.hour, time.minute, time.second));
  }
  else
  {
    Serial.println(F("Invalid time format. Use HHMMSS"));
  }
}

void SerialCommandHandler::parseDateInput(const String &input)
{
  if (input.length() == 8 && isValidDateFormat(input))
  {
    Date date = parseDateString(input);
    clock->setDate(date);
    Serial.print(F("Date set to: "));
    Serial.println(formatDate(date.day, date.month, date.year));
  }
  else
  {
    Serial.println(F("Invalid date format. Use DDMMYYYY"));
  }
}

bool SerialCommandHandler::isValidTimeFormat(const String &timeStr)
{
  if (timeStr.length() != 6)
    return false;

  Time time = parseTimeString(timeStr);
  return isValidTime(time);
}

bool SerialCommandHandler::isValidDateFormat(const String &dateStr)
{
  if (dateStr.length() != 8)
    return false;

  Date date = parseDateString(dateStr);
  return isValidDate(date);
}

String SerialCommandHandler::formatDate(int day, int month, int year)
{
  return String(day < 10 ? "0" : "") + String(day) + "." +
         String(month < 10 ? "0" : "") + String(month) + "." +
         String(year);
}

String SerialCommandHandler::formatTime(int hour, int minute, int second)
{
  return String(hour < 10 ? "0" : "") + String(hour) + ":" +
         String(minute < 10 ? "0" : "") + String(minute) + ":" +
         String(second < 10 ? "0" : "") + String(second);
}