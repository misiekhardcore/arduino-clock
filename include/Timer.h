#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

struct TimerData {
  uint8_t hour;
  uint8_t minute;
  uint8_t second;
  bool running;
  bool completed;
};

class Timer {
private:
  TimerData data;
  unsigned long lastUpdate;

public:
  Timer();
  
  // Initialization
  void begin();
  
  // Timer control
  void start();
  void stop();
  void reset();
  void update();
  
  // Timer setting
  void setTime(uint8_t hour, uint8_t minute, uint8_t second);
  void adjustHour();
  void adjustMinute();
  void adjustSecond();
  
  // Status queries
  bool isRunning() const;
  bool isCompleted() const;
  
  // Getters
  uint8_t getHour() const;
  uint8_t getMinute() const;
  uint8_t getSecond() const;
  TimerData getTime() const;
  
  // String representation
  char* getTimeString() const;
};

#endif // TIMER_H 