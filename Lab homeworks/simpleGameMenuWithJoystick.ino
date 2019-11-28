#include <LiquidCrystal.h>
#include <EEPROM.h>
const int V0_PIN = 9; // PWN in loc de POTENTIOMETRU
const int RS = 12;
const int enable = 11;
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int xAxisPin = A0;
const int yAxisPin = A1;
const int joystickButtonPin = 1;
const int lcdWidth = 16;
int menuIndex = 0;
int settingsIndex = 0;
int xValue = 0;
int yValue = 0;
int maxThreshold = 650;
int minThreshold = 350;
int lastButtonState = LOW;
int lastDebounceTime = 0;
int startingLevelValue = 0;
int lives = 3;
int score = 0;
int level = 0;
int highScore = 0;
int buttonState = LOW;
bool blinkValue = true;
bool isPlaying = false;
bool isInHighScore = false;
bool isInSettings = false;
bool refresh = false;
bool joyMoved = false;
char playerName[3] = "AAA";
char highscoreName[3] ;
String exitText = "EXIT";
const int debounceDelay = 100;
const int blinkDelay = 300;
unsigned long lastRefresh = 0;
unsigned long gameStartTime = 0;
unsigned long blinkTime = 0;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
String mainMenu[] = {
  "Sample Game Menu",
  "Start",
  "High Score",
  "Settings"
};

String playMenu[] = {
  "Lives",
  "Level",
  "Score"
};

void accessCurrentMenu() {
  if (menuIndex == 0 && !isPlaying) {
    lcd.clear();
    isPlaying = true;
    level = startingLevelValue;
    gameStartTime = millis();
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
      settingsIndex++;
      if (settingsIndex > 4)
        settingsIndex = 4;
    }
    isInSettings = true;
    blinkTime = millis();
    return;
  }
}

void exitCurrentMenu() {
  if (isPlaying) {
    return;
  }
  if (isInHighScore) {
    isInHighScore = false;
    return;
  }
  if (isInSettings) {
    if (isInSettings) {
      settingsIndex--;
      if (settingsIndex < 0)
        settingsIndex = 0;
    }
    return;
  }
}

void upCurrentMenu() {
  if (isPlaying) {
    return;
  }
  if (isInHighScore) {
    return;
  }
  if (isInSettings) {
    if (settingsIndex == 0) {
      startingLevelValue--;
      if (startingLevelValue < 0)
        startingLevelValue = 0;
    } else if (settingsIndex < 4) {
      playerName[settingsIndex - 1]--;
      if (char(playerName[settingsIndex - 1]) - 'A' <= 0)
        playerName[settingsIndex - 1] = 'Z';
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
  if (isInHighScore) {
    return;
  }
  if (isInSettings) {
    if (settingsIndex == 0) {
      startingLevelValue++;
      if (startingLevelValue > 9)
        startingLevelValue = 9;
    }  else if (settingsIndex < 4) {
      playerName[settingsIndex - 1]++;
      if (char(playerName[settingsIndex - 1]) - 'Z' >= 0)
        playerName[settingsIndex - 1] = 'A';
    }
    return;
  }
  menuIndex--;
  if (menuIndex < 0)
    menuIndex = 0;
}

void displayCurrentMenu(bool refresh) {
  if (refresh) {
    lcd.clear();
  }
  if (isPlaying) {
    if (level - startingLevelValue > 5) {
      lcd.clear();
      highScore = readFromEEPROM();
      while (!buttonReading()) {
        lcd.setCursor(0, 0);
        lcd.print("Game over: ");
        lcd.print(score);
        lcd.setCursor(0, 1);
        lcd.print("Press Button");
      }
      if (highScore < score) {
        highScore = score;
        writeToEEPROM();
      }
      isPlaying = !isPlaying;
      score = 0;
      level = 0;
      return;
    }
    level = startingLevelValue + (millis() - gameStartTime) / 2000;
    lcd.setCursor(0, 0);
    lcd.print(playMenu[0]);
    lcd.print(" ");
    lcd.print(lives);
    lcd.print(" ");
    lcd.print(playMenu[1]);
    lcd.print(" ");
    lcd.print(level);
    lcd.setCursor(0, 1);
    lcd.print(playMenu[2]);
    lcd.print("  ");
    lcd.setCursor(8, 1);
    score = level * 3;
    lcd.print(score);
    lcd.print("  ");
    return;
  }
  if (isInHighScore) {
    highScore = readFromEEPROM();
    lcd.setCursor(0, 0);
    lcd.print("HighScore:");
    for (int i = 0; i < 3; i++) {
      lcd.setCursor(i, 1);
      lcd.print(highscoreName[i]);
    }
    lcd.print(" ");
    lcd.setCursor(8, 1);
    lcd.print(highScore);
    return;
  }
  if (isInSettings) {
    if (millis() - blinkTime > blinkDelay) {
      blinkValue = !blinkValue;
      blinkTime = millis();
    }
    lcd.setCursor(0, 0);
    lcd.print("Starting level:");
    if (settingsIndex != 0)
      lcd.print(startingLevelValue);
    else if (blinkValue)
      lcd.print(startingLevelValue);
    lcd.setCursor(0, 1);
    lcd.print("Name: ");
    if (settingsIndex != 1)
      lcd.print(playerName[0]);
    else if (blinkValue)
      lcd.print(playerName[0]);
    else
      lcd.print(" ");
    if (settingsIndex != 2)
      lcd.print(playerName[1]);
    else if (blinkValue)
      lcd.print(playerName[1]);
    else
      lcd.print(" ");
    if (settingsIndex != 3)
      lcd.print(playerName[2]);
    else if (blinkValue)
      lcd.print(playerName[2]);
    else
      lcd.print(" ");
    lcd.setCursor(12, 1);
    if (settingsIndex != 4)
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

bool buttonReading() {
  bool returnValue = false;
  int buttonReading = !digitalRead(joystickButtonPin);
  if (buttonReading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;
      if (buttonState == HIGH) {
        returnValue = true;
      }
    }
  }
  lastButtonState = buttonReading;
  return returnValue;
}

int readFromEEPROM() {
  char data[3];
  int len = 0;
  unsigned char c;
  for (len = 0; len < 3; len++){
    c = EEPROM.read(len);
    data[len] = c;
  }
  len = 3;
  EEPROM.read(len);
  len++;
  long two = EEPROM.read(len);
  long one = EEPROM.read(len + 1);
  int localScore = ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
  for (len = 0; len < 3; len++)
    highscoreName[len] = data[len];
  return localScore;
}

void writeToEEPROM() {
  int i;
  for (i = 0; i < strlen(playerName); i++) {
    EEPROM.update(i, playerName[i]);
  }
  EEPROM.update(strlen(playerName), ' ');
  byte two = (highScore & 0xFF);
  byte one = ((highScore >> 8) & 0xFF);
  EEPROM.update(strlen(playerName) + 1, two);
  EEPROM.update(strlen(playerName) + 2, one);
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(lcdWidth, 2);
  pinMode(V0_PIN, OUTPUT); // PWN in loc de POTENTIOMETRU
  analogWrite(V0_PIN, 90); // PWN in loc de POTENTIOMETRU
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(joystickButtonPin, INPUT_PULLUP);
  lastRefresh = millis();
  Serial.begin(9600);
}

void loop() {
  bool buttonPressed = buttonReading();
  if (millis() - lastRefresh > 150) {
    refresh = true;
    lastRefresh = millis();
  }
  displayCurrentMenu(refresh);
  refresh = false;
  xValue = analogRead(xAxisPin);
  if (xValue < minThreshold && joyMoved == false) {
    upCurrentMenu();
    joyMoved = true;
  }
  if (xValue > maxThreshold && joyMoved == false) {
    downCurrentMenu();
    joyMoved = true;
  }
  yValue = analogRead(yAxisPin);
  if (yValue < minThreshold && joyMoved == false) {
    exitCurrentMenu();
    joyMoved = true;
  }
  if (yValue > maxThreshold && joyMoved == false) {
    accessCurrentMenu();
    joyMoved = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold && xValue >= minThreshold && xValue <= maxThreshold) {
    joyMoved = false;
  }
  if (buttonPressed) {
    if (settingsIndex == 4 && isInSettings) {
      if (buttonPressed) {
        isInSettings = false;
        settingsIndex = 0;
      }
    }
    else if (!isInSettings)
      accessCurrentMenu();
  }
}
