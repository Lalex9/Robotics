/*
 *      SPACE WARS
 *  Arduino game for 8x8 LED Matrix with LCD display and joystick
 *  
 *  For game detalis please visit: 
 *  https://github.com/Lalex9/Robotics/tree/master/Space%20wars%20(matrix%20project)
 *  
 *  2019 Alexandru Lungu
 */

#include "VariablesAndDeclarations.h"

void setup() {
  // put your setup code here, to run once:
  lcd.begin(lcdWidth, 2);
  pinMode(V0_PIN, OUTPUT);
  analogWrite(V0_PIN, 90); 
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(joystickButtonPin, INPUT_PULLUP);
  pinMode(pushButtonPin, INPUT_PULLUP);
  lastRefresh = millis();
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  pinMode(xAxisPin , INPUT);
  pinMode(yAxisPin, INPUT);
  for (int i = 0; i < maximumNumberOfRockets; i++) {
    rockets[i] = { false, 0, 0, 0};
  }
  Serial.begin(9600);
  //clearEEPROM();
}

void loop() {
  bool buttonPressed = buttonReading();
  if (millis() - lastRefresh > 300) {
    refresh = true;
    lastRefresh = millis();
  }
  if (isPlaying) {
    joyMoved = false;
  }
  playGame(refresh);
  refresh = false;
  joystickControl();
  buttonControl(buttonPressed);
}
