void accessCurrentMenu() {
  if (isFreezed)
    return;
  if (isPlaying) {
    if (millis() - lastPossitionChange > 100) {
      lastPossitionChange = millis();
      pos++;
      if (pos > 7)
        pos = 7;
      if (mapOfEnemies[6][pos] == 1 || mapOfEnemies[7][max(pos + 1, 7)] == 1) {
        isPlaying = false;
        isGameOver = true;
        lcd.clear();
        lc.clearDisplay(0);
        gameOverTime = millis();
      }
    }
    return;
  }
  if (isSavingName) {
    highScoreIndex++;
    if (highScoreIndex > 3)
      highScoreIndex = 3;
    return;
  }
  if (isGameOver) {
    saveGame = !saveGame;
    return;
  }
  if (menuIndex == 0 && !isPlaying && !isFreezed) {
    freezeTime = millis();
    isFreezed = true;
    level = 0;
    return;
  }
  if (menuIndex == 1) {
    lcd.clear();
    isInHighScore = true;
    return;
  }
  if (menuIndex == 2) {
    lcd.clear();
    if (isInSettings) {
      if (settingsIndex == 2) {
        isInSettings = false;
        settingsIndex = 0;
        return;
      }
      settingsIndex++;
    } else {
      isInSettings = true;
      settingsIndex = 1;
    }
    blinkTime = millis();
    return;
  }
}

void exitCurrentMenu() {
  if (isPlaying) {
    if (millis() - lastPossitionChange > 100) {
      lastPossitionChange = millis();
      pos--;
      if (pos < 0)
        pos = 0;
      if (mapOfEnemies[6][pos] == 1 || mapOfEnemies[7][max(pos - 1, 0)] == 1) {
        isPlaying = false;
        isGameOver = true;
        gameOverTime = millis();
        lcd.clear();
        lc.clearDisplay(0);
      }
    }
    return;
  }
  if (isSavingName) {
    highScoreIndex--;
    if (highScoreIndex < 0)
      highScoreIndex = 0;
    return;
  }
  if (isGameOver) {
    saveGame = !saveGame;
    return;
  }
  if (isInHighScore) {
    isInHighScore = false;
    return;
  }
  if (isInSettings) {
    settingsIndex--;
    if (settingsIndex < 1)
      isInSettings = false;
    return;
  }
}

void upCurrentMenu() {
  if (isPlaying) {
    return;
  }
  if (isGameOver && saveGame) {
    playerName[highScoreIndex]--;
    if (char(playerName[highScoreIndex]) - 'A' < 0)
      playerName[highScoreIndex] = 'Z';
    return;
  }
  if (isInHighScore) {
    return;
  }
  if (isInSettings) {
    if (settingsIndex == 1) {
      currentDifficulty--;
      if (currentDifficulty < 0)
        currentDifficulty = 2;
    }
    return;
  }
  menuIndex++;
  if (menuIndex > 2)
    menuIndex = 2;
}


void downCurrentMenu() {
  if (isPlaying) {
    return;
  }
  if (isGameOver && saveGame) {
    playerName[highScoreIndex]++;
    if (char(playerName[highScoreIndex]) - 'Z' > 0)
      playerName[highScoreIndex] = 'A';
    return;
  }
  if (isInHighScore) {
    return;
  }
  if (isInSettings) {
    if (settingsIndex == 1) {
      currentDifficulty++;
      if (currentDifficulty > 2)
        currentDifficulty = 0;
    }
    return;
  }
  menuIndex--;
  if (menuIndex < 0)
    menuIndex = 0;
}

bool buttonReadingPinJ() {
  bool returnValue = false;
  int buttonReading = !digitalRead(joystickButtonPin);
  if (isPlaying)
    return buttonReading;
  if (buttonReading != lastButtonStateJ) {
    lastDebounceTimeJ = millis();
  }
  if ((millis() - lastDebounceTimeJ) > debounceDelay) {
    if (buttonReading != buttonStateJ) {
      buttonStateJ = buttonReading;
      if (buttonStateJ == HIGH) {
        returnValue = true;
      }
    }
  }
  lastButtonStateJ = buttonReading;
  return returnValue;
}
bool buttonReadingPinB() {
  bool returnValue = false;
  int buttonReading = !digitalRead(pushButtonPin);
  if (isPlaying)
    return buttonReading;
  if (buttonReading != lastButtonStateB) {
    lastDebounceTimeB = millis();
  }
  if ((millis() - lastDebounceTimeB) > debounceDelay) {
    if (buttonReading != buttonStateB) {
      buttonStateB = buttonReading;
      if (buttonStateB == HIGH) {
        returnValue = true;
      }
    }
  }
  lastButtonStateB = buttonReading;
  return returnValue;
}

bool buttonReading() {
  return (buttonReadingPinJ() || buttonReadingPinB());
}
void buttonControl(bool buttonPressed) {
  if (buttonPressed) {
    if (isPlaying) {
      if (millis() - lastRocketLaunch > rocketLaunchSpeed) {
        fireRocket();
        lastRocketLaunch = millis();
      }
    } else if (isGameOver) {
      if (saveGame) {
        isSavingName = true;
        printSaveHighScore();
        if (highScoreIndex == 3) {
          Serial.println("here");
          highScoreStruct.highScore = score;
          strcpy(highScoreStruct.playerName, playerName);
          writeToEEPROM();
          exitGameOver();
        }
      } else {
        exitGameOver();
      }
    } else if (settingsIndex == 2 && isInSettings) {
      if (buttonPressed) {
        isInSettings = false;
        settingsIndex = 0;
      }
    }
    else if (!isInSettings)
      accessCurrentMenu();
  }
}

void joystickControl() {
  xValue = analogRead(xAxisPin);
  if (xValue < minThreshold && joyMoved == false) {
    downCurrentMenu();
    joyMoved = true;
  }
  if (xValue > maxThreshold && joyMoved == false) {
    upCurrentMenu();
    joyMoved = true;
  }
  yValue = analogRead(yAxisPin);
  if (yValue < minThreshold && joyMoved == false) {
    accessCurrentMenu();
    joyMoved = true;
  }
  if (yValue > maxThreshold && joyMoved == false) {
    exitCurrentMenu();
    joyMoved = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold && xValue >= minThreshold && xValue <= maxThreshold) {
    joyMoved = false;
  }
}
