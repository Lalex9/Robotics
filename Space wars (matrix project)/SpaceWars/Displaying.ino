void displayOnMatrix() {
  displayRockets();
  displayEnemies();
  displayShip();
}

void displayShip() {
  //lc.clearDisplay(0);
  lc.setLed(0, 6, pos, true);
  if (pos > 0)
    lc.setLed(0, 7, pos - 1, true);
  lc.setLed(0, 7, pos, true);
  if (pos < 7)
    lc.setLed(0, 7, pos + 1, true);
}

void displayRockets() {
  for (int i = 0; i < maximumNumberOfRockets; i++) {
    if (rockets[i].isValid)
      lc.setLed(0, rockets[i].posY, rockets[i].posX, true);
  }
}

void displayEnemies() {
  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      lc.setLed(0, i, j, mapOfEnemies[i][j]);
    }
  }
}


void displayCountdown() {
  localTime = millis() - freezeTime;
  localTime = 5000 - localTime;
  if (localTime > 3000) {
    if (level == 0) {
      lcd.setCursor(0, 0);
      lcd.print(menuMessages[0]);
      lcd.setCursor(0, 1);
      lcd.print(menuMessages[1]);
    } else {
      lcd.setCursor(0, 0);
      lcd.print(menuMessages[2]);
      lcd.setCursor(0, 1);
      lcd.print(menuMessages[3]);
      lcd.print(level + 1);
    }
  } else if (localTime > 2000) {
    displayDigit(three);
    return;
  } else if (localTime > 1000) {
    displayDigit(two);
    return;
  } else if (localTime > 0) {
    displayDigit(one);
    return;
  }
}

void displayDigit(byte digit[]) {
  for (int i = 0; i < matrixSize; i++) {
    lc.setRow(0, i, digit[i]);
  }
}
