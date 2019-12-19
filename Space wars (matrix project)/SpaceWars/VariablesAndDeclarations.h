#ifndef VARSDECL_H
#define VARSDECL_H

#include <LiquidCrystal.h>
#include <LedControl.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"

//Object to control the Led Matrix
LedControl lc = LedControl(12, 11, 10, 1);

//pins declarations
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
const int enable = 6;
const int joystickButtonPin = A4;
const int pushButtonPin = A5;
const int RS = 7;
const int V0_PIN = 9;
const int xAxisPin = A0;
const int yAxisPin = A1;

//Object to control the LCD display
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

//constants 
const int blinkDelay = 300;
const int debounceDelay = 50;
const int lcdWidth = 16;
const int matrixSize = 8;
const int newEnemiesTime = 2000;
const int rocketLaunchSpeed = 250;
const int rocketSpeed = 50;

//state variables
int buttonStateJ = LOW;
int buttonStateB = LOW;
int currentDifficulty = 0;
int highScoreIndex = 0;
int lastButtonStateB = LOW;
int lastButtonStateJ = LOW;
int level = 0;
int lives = 3;
int localTime = 0;
int maxThreshold = 650;
int menuIndex = 0;
int minThreshold = 350;
int pos = 3;
int score = 0;
int settingsIndex = 0;
int xValue = 0;
int yValue = 0;

bool blinkValue = true;
bool isGameOver = false;
bool isFreezed = false;
bool isInHighScore = false;
bool isInSettings = false;
bool isPlaying = false;
bool isSavingHighscore = false;
bool isSavingName = false;
bool joyMoved = false;
bool refresh = false;
bool saveGame = false;

char playerName[3] = "AAA";
String exitText = "EXIT";

unsigned long blinkTime = 0;
unsigned long freezeTime = 0;
unsigned long gameStartTime = 0;
unsigned long gameOverTime = 0;
unsigned long lastDebounceTimeB = 0;
unsigned long lastDebounceTimeJ = 0;
unsigned long lastEnemyWave = 0;
unsigned long lastPossitionChange = 0;
unsigned long lastRefresh = 0;
unsigned long lastRocketLaunch = 0;

//length of each level in seconds
const int levelTimes[] = {60, 120, 180, 240};

String mainMenu[] = {
  "Space Wars",
  "Start",
  "High Score",
  "Settings"
};

String playMenu[] = {
  "Time",
  "Lvl",
  "Score"
};

String difficulties[] = {
  "Easy",
  "Normal",
  "Hard"
};

String menuMessages[] = {
  "    The game    ",
  "  is starting   ",
  "Congratulations!",
  "Starting level "
};

byte mapOfEnemies[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0 },
  {0, 0, 0, 0, 0, 0, 0, 0 }
};

//Digits drawn on the LED Matrix
const byte one[matrixSize] =
{
  B00001000,
  B00011000,
  B00101000,
  B01001000,
  B00001000,
  B00001000,
  B00001000,
  B00001000
};

const byte two[matrixSize] =
{
  B00111100,
  B01000010,
  B00000100,
  B00001000,
  B00010000,
  B00100000,
  B01000000,
  B11111110
};

const byte three[matrixSize] =
{
  B00000000,
  B01111100,
  B00000010,
  B00000010,
  B01111100,
  B00000010,
  B00000010,
  B00111100
};

//Struct for saving rocket data
typedef struct Rocket {
  bool isValid;
  int posX, posY;
  unsigned long lastMoveTime;
};

//Struct used for highScore reading and saving
struct highScoreTemplate {
  int highScore;
  char playerName[3];
} highScoreStruct;

const int maximumNumberOfRockets = 20;
Rocket rockets[maximumNumberOfRockets];

#endif
