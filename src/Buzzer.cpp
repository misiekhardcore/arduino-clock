#include "Buzzer.h"

Buzzer::Buzzer(int pin) : pin(pin), isPlaying(false), alarmStartTime(0) {
}

void Buzzer::begin() {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void Buzzer::update() {
  if (isPlaying) {
    unsigned long currentMillis = millis();
    unsigned long elapsed = currentMillis - alarmStartTime;
    
    // Match old Arduino program's alarm pattern exactly:
    // 0-200ms: tone on
    // 200-300ms: tone off  
    // 300-500ms: tone on
    // 500-600ms: tone off
    // 600-800ms: tone on
    // 800-1200ms: tone off (then repeat)
    
    if ((elapsed < 200) || ((elapsed > 300) && (elapsed < 500)) || ((elapsed > 600) && (elapsed < 800))) {
      tone(pin, 1000); // 1000Hz like old program
    } else {
      noTone(pin);
    }
    
    // Reset cycle after 1200ms
    if (elapsed > 1200) {
      alarmStartTime = currentMillis;
    }
  }
}

void Buzzer::playAlarm() {
  isPlaying = true;
  alarmStartTime = millis();
}

void Buzzer::stopAlarm() {
  isPlaying = false;
  stopTone();
}

bool Buzzer::isAlarmPlaying() const {
  return isPlaying;
}

void Buzzer::playTone(int frequency, unsigned long duration) {
  if (frequency > 0) {
    tone(pin, frequency, duration);
  }
}

void Buzzer::stopTone() {
  noTone(pin);
  digitalWrite(pin, LOW);
} 