#ifndef SERIAL_COMMAND_HANDLER_H
#define SERIAL_COMMAND_HANDLER_H

#include <Arduino.h>
#include "Clock.h"

class SerialCommandHandler
{
private:
  Clock *clock;

  // Input state variables
  bool waitingForTimeInput;
  bool waitingForDateInput;
  String timeInputBuffer;
  String dateInputBuffer;

  // Command processing
  void processCommand(const String &command);
  void showHelp();
  void showStatus();
  void setRTCTime();
  void setRTCDate();
  void parseTimeInput(const String &input);
  void parseDateInput(const String &input);
  bool isValidTimeFormat(const String &timeStr);
  bool isValidDateFormat(const String &dateStr);

  // New helper methods for DRY refactoring
  void handleTimeCommand(const String &timeStr);
  void handleDateCommand(const String &dateStr);
  void handleAlarmCommand(const String &alarmCmd);
  void handleAlarmSetCommand(const String &timeStr);
  void handleTimerCommand(const String &timerCmd);
  void handleTimerSetCommand(const String &timeStr);
  void showAlarmHelp();
  void showTimerHelp();
  
  // Parsing and validation helpers
  Time parseTimeString(const String &timeStr);
  Date parseDateString(const String &dateStr);
  bool isValidTime(const Time &time);
  bool isValidTimeValues(int hour, int minute, int second);
  bool isValidDate(const Date &date);

public:
  SerialCommandHandler();

  void begin(Clock *clock);
  void update();
  void handleSerialInput();
  String formatTime(int hour, int minute, int second);
  String formatDate(int day, int month, int year);
};

#endif // SERIAL_COMMAND_HANDLER_H