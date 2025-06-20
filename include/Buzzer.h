#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer {
private:
  int pin;
  bool isPlaying;
  unsigned long alarmStartTime;

public:
  Buzzer(int pin);
  
  void begin();
  void update();
  
  // Control methods
  void playAlarm();
  void stopAlarm();
  bool isAlarmPlaying() const;
  
  // Utility methods
  void playTone(int frequency, unsigned long duration);
  void stopTone();
};

#endif 