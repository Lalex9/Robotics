#include "VariablesAndDeclarations.h"

void setup() {
  // put your setup code here, to run once:
  lcd.begin(lcdWidth, 2);
  pinMode(V0_PIN, OUTPUT); // PWN in loc de POTENTIOMETRU
  analogWrite(V0_PIN, 90); // PWN in loc de POTENTIOMETRU
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
