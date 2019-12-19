//Print the main menu on the LCD
void menuPrinting() {
  if (isFreezed && localTime > 3000)
    return;
  int timeLeft;
  int gameLength;
  long timePassed;
  if (level < 4) {
    gameLength = levelTimes[level] + 30 * currentDifficulty;
    timePassed = (millis() - gameStartTime) / 1000;
    if (isFreezed) {
      timeLeft = gameLength;
    } else {
      timeLeft = gameLength - timePassed;
    }
    if (timeLeft <= 0) {
      freezeTime = millis();
      isFreezed = true;
      score += 100;
      level++;
      initialiseGame();
      return;
    }
  } else if (level == 4) {
    timeLeft;
    timePassed = (millis() - gameStartTime) / 1000;
    if (isFreezed) {
      timeLeft = 0;
    } else {
      timeLeft = timePassed;
    }
  }
  int minutes = timeLeft / 60;
  int seconds = timeLeft % 60;
  lcd.setCursor(0, 0);
  lcd.print(playMenu[0]);
  lcd.print(" ");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10)
    lcd.print("0");
  lcd.print(seconds);
  lcd.print(" ");
  lcd.print(playMenu[1]);
  lcd.print(" ");
  lcd.print(level + 1);
  lcd.setCursor(0, 1);
  lcd.print(playMenu[2]);
  lcd.print("  ");
  lcd.setCursor(8, 1);
  lcd.print(score);
  lcd.print("  ");
  return;
}

//High Score menu printing
void printSaveHighScore() {
  bool exitSave = false;
  if (millis() - blinkTime > blinkDelay) {
    blinkValue = !blinkValue;
    blinkTime = millis();
  }
  lcd.setCursor(0, 0);
  lcd.print("Player name: ");
  lcd.setCursor(0, 1);
  if (highScoreIndex != 0)
    lcd.print(playerName[0]);
  else if (blinkValue)
    lcd.print(playerName[0]);
  else
    lcd.print(" ");
  if (highScoreIndex != 1)
    lcd.print(playerName[1]);
  else if (blinkValue)
    lcd.print(playerName[1]);
  else
    lcd.print(" ");
  if (highScoreIndex != 2)
    lcd.print(playerName[2]);
  else if (blinkValue)
    lcd.print(playerName[2]);
  else
    lcd.print(" ");
  lcd.setCursor(12, 1);
  if (highScoreIndex != 3)
    lcd.print("SAVE");
  else if (blinkValue)
    lcd.print("SAVE");
  else
    lcd.print("    ");
}

//Game over text printing
void gameOver() {
  readFromEEPROM();
  if (highScoreStruct.highScore < score) {
    if (millis() - gameOverTime < 2000) {
      lcd.setCursor(0, 0);
      lcd.print("  You set the   ");
      lcd.setCursor(0, 1);
      lcd.print("   highscore!   ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("Do you want to ");
      lcd.setCursor(0, 1);
      lcd.print("save it? Yes No");
      if (saveGame) {
        lcd.setCursor(8, 1);
      } else {
        lcd.setCursor(12, 1);
      }
      lcd.print("X");
    }
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Game over: ");
    lcd.print(score);
    lcd.setCursor(0, 1);
    lcd.print("Press Button");
  }
}

//Main game function for the gameplay and information printing
void playGame(bool refresh) {
  if (refresh) {
    lcd.clear();
  }
  if (isFreezed) {
    displayCountdown();
    menuPrinting();
    if (millis() - freezeTime > 5000) {
      isFreezed = false;
      initialiseGame();
    }
    return;
  }
  if (isSavingName) {
    printSaveHighScore();
    return;
  }
  if (isGameOver) {
    lc.clearDisplay(0);
    if (!isSavingName)
      gameOver();
    return;
  }
  if (isPlaying) {
    moveTheRockets();
    addEnemies();
    displayOnMatrix();
    menuPrinting();
    return;
  }
  if (isInHighScore) {
    readFromEEPROM();
    lcd.setCursor(0, 0);
    lcd.print("HighScore:");
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(i, 1);
      lcd.print(highScoreStruct.playerName[i]);
    }
    lcd.print(" ");
    lcd.setCursor(8, 1);
    lcd.print(highScoreStruct.highScore);
    return;
  }
  if (isInSettings) {
    if (millis() - blinkTime > blinkDelay) {
      blinkValue = !blinkValue;
      blinkTime = millis();
    }
    lcd.setCursor(0, 0);
    lcd.print("Difficulty:");
    lcd.setCursor(0, 1);
    if (settingsIndex != 1)
      lcd.print(difficulties[currentDifficulty]);
    else if (blinkValue)
      lcd.print(difficulties[currentDifficulty]);
    else
      lcd.print(" ");
    lcd.setCursor(12, 1);
    if (settingsIndex != 2)
      lcd.print(exitText);
    else if (blinkValue)
      lcd.print(exitText);
    else
      lcd.print(" ");
    return;
  }
  int stringLength = mainMenu[menuIndex].length();
  lcd.setCursor((lcdWidth - stringLength) / 2, 0);
  lcd.print(mainMenu[menuIndex]);
  stringLength = mainMenu[menuIndex + 1].length();
  lcd.setCursor((lcdWidth - stringLength) / 2 - 1, 1);
  lcd.print("<");
  lcd.print(mainMenu[menuIndex + 1]);

}
