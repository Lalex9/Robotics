void initialiseGame() {
  for (int i = 0; i < matrixSize; i++)
    for (int j = 0; j < matrixSize; j++)
      mapOfEnemies[i][j] = 0;
  pos = random(3, 5);
  level = 0;
  lcd.clear();
  lc.clearDisplay(0);
  isPlaying = true;
  gameStartTime = millis();
  lastPossitionChange = millis() - 100;
  strcpy(playerName, "AAA");
}

void fireRocket() {
  Rocket newRocket;
  newRocket.isValid = true;
  newRocket.posX = pos;
  newRocket.posY = 6;
  newRocket.lastMoveTime = millis();
  for (int i = 0; i < maximumNumberOfRockets; i++) {
    if (!rockets[i].isValid) {
      rockets[i] = newRocket;
      break;
    }
  }
}

void moveTheRockets() {
  for (int i = 0; i < maximumNumberOfRockets; i++) {
    if (rockets[i].isValid)
      if (millis() - rockets[i].lastMoveTime > rocketSpeed) {
        rockets[i].posY--;
        if (mapOfEnemies[rockets[i].posY][rockets[i].posX] == 1) {
          mapOfEnemies[rockets[i].posY][rockets[i].posX] = 0;
          rockets[i].isValid = false;
          score += 1;
        }
        if (rockets[i].posY < 0) {
          rockets[i].isValid = false;
        }
        rockets[i].lastMoveTime = millis();
      }
  }
}

void addEnemies() {
  if (millis() - lastEnemyWave + 100 * currentDifficulty + 50 * level> newEnemiesTime) {
    lastEnemyWave = millis();
    for (int i = 0; i < matrixSize; i++) {
      if (mapOfEnemies[7][i] != 0) {
        isGameOver = true;
        isPlaying = false;
        gameOverTime = millis();
        return;
      }
    }
    for (int i = matrixSize - 1; i > 0; i--) {
      for (int j = 0; j < matrixSize ; j++) {
        mapOfEnemies[i][j] = mapOfEnemies[i - 1][j];
      }
    }
    if (mapOfEnemies[6][pos] == 1 || mapOfEnemies[7][max(pos + 1, 7)] == 1 || mapOfEnemies[7][max(pos - 1, 0)] == 1) {
      isGameOver = true;
      gameOverTime = millis();
      isPlaying = false;
    }
    for (int i = 0; i < matrixSize; i++) {
      mapOfEnemies[0][i] = 0;
    }
    int numberOfEnemies = random(4, 8);
    while (numberOfEnemies > 0) {
      int possition = random(8);
      if (mapOfEnemies[0][possition] == 0) {
        mapOfEnemies[0][possition] = 1;
        numberOfEnemies--;
      }
    }
  }
}

void exitGameOver() {
  score = 0;
  level = 0;
  isGameOver = false;
  isSavingName = false;
  saveGame = false;
  highScoreIndex = 0;
}
