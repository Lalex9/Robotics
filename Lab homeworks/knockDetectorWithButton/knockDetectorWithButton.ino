#include "pitches.h"
const int buttonPin = 2;
const int buzzerOutputPin = 11;
const int detectorPin = A0;
const int threshold = 2;
int arraySize = 0;
int buttonReadingValue = 0;
int buttonState = 0;
int detectorValue = 0;
int lastButtonState = 0;
bool isPlaying = false;
bool isWaiting = false;
unsigned long currentIteration = 0;
unsigned long knockTime = 0;
unsigned long lastDebounceTime = 0;
unsigned long noteTime = 0; 
const unsigned long debounceDelay = 50;
const unsigned long waitTime = 5000;

void setup() {
  pinMode(buzzerOutputPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(detectorPin, INPUT);
  Serial.begin(9600);
  arraySize = sizeof(durations)/sizeof(int);
}

void loop() {
  if (!isWaiting && !isPlaying) {
    detectorValue = analogRead(detectorPin);
    if (detectorValue > threshold) {
      Serial.println(detectorValue);
      knockTime = millis();
      isWaiting = true;
    }
  }
  if (isWaiting) {
    if (millis() - knockTime > waitTime) {
      isPlaying = true;
      isWaiting = false;
      currentIteration = 0;
    }
  }
  if (isPlaying) {
    playTune(currentIteration % arraySize );
    currentIteration++;
  }
  
}
void playTune(int iteration) {
  tone(buzzerOutputPin, melody[iteration], durations[iteration]);
  noteTime = millis();
  bool buttonWasPressed = false;
  while ( millis() - noteTime < durations[iteration] && !buttonWasPressed) {
    buttonWasPressed = buttonStatus();
  }
  noTone(buzzerOutputPin);
  if (buttonWasPressed) {
    isPlaying = false;
  }
}
bool buttonStatus() {
  bool returnValue = false;
  buttonReadingValue = !digitalRead(buttonPin);
  if (buttonReadingValue != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReadingValue != buttonState) {
      buttonState = buttonReadingValue;
      if (buttonState == HIGH) {
        returnValue = true;
      }
    }
  }
  lastButtonState = buttonReadingValue;
  return returnValue;
}
