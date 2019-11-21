const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;
const int segSize = 8;
const int xAxisPin = A0;
const int yAxisPin = A1;
const int joystickButtonPin = 1;
const int blinkingDelay = 250;
const int debounceDelay = 50;
const int noOfDigits = 10;
const int noOfDisplays = 4;
int currentNumber[noOfDisplays] = {0, 0, 0, 0};
int buttonState = LOW;
int cursorIndex = 3;
int dpState = HIGH;
int lastButtonState = LOW;
int lastDebounceTime = 0;
int maxThreshold = 600;
int minThreshold = 400;
int xValue = 0;
int yValue = 0;
bool isBlinking = false;
bool joyMoved = false;
unsigned long delayCounting = 50; // incrementing interval
unsigned long lastBlink = 0;
unsigned long lastIncreasing = 0;
// segments array, similar to before
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
};
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g 
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

void displayNumber(byte digit, int decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  // write the decimalPoint to DP pin
  if (decimalPoint == HIGH) {
    if (isBlinking && millis() - lastBlink > blinkingDelay) {
      lastBlink = millis();
      Serial.println(lastBlink);
      dpState = !dpState;
    }
  }
  digitalWrite(segments[segSize - 1], dpState & decimalPoint);
}

void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
}

void showNumber(int number[]) {
  int digit;
  int lastDigit;
  digit = 0;
  for (int i = 0; i < noOfDisplays; i++) {
    lastDigit = number[i]; // get the last digit
    showDigit(digit);
    if (cursorIndex == i) {
      displayNumber(lastDigit, HIGH);
    } else {
      displayNumber(lastDigit, LOW);
    }
    // increase this delay to see multiplexing in action. At about 100 it becomes obvious
    delay(5);
    digit++; // move to next display
  }
}

void buttonReading() {
  int buttonReading = digitalRead(joystickButtonPin);
  if (buttonReading != lastButtonState){
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (buttonReading != buttonState) {
      buttonState = buttonReading;
      if (buttonState == LOW) {
        isBlinking = !isBlinking;
        if (isBlinking)
          lastBlink = millis();
      }
    }
  }
  lastButtonState = buttonReading;
  if (!isBlinking) 
    dpState = HIGH;
}

void setup() {
  for (int i = 0; i < segSize; i++){
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++){
    pinMode(digits[i], OUTPUT);
  }
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
  pinMode(joystickButtonPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  showNumber(currentNumber);
  if (!isBlinking) {
    yValue = analogRead(yAxisPin);
    if (yValue < minThreshold && joyMoved == false) {
      cursorIndex++;
      if (cursorIndex > 3)
        cursorIndex = 0;
      joyMoved = true;
    }
    if (yValue > maxThreshold && joyMoved == false) {
      cursorIndex--;
      if (cursorIndex < 0)
        cursorIndex = 3;
      joyMoved = true;
    }
    if (yValue >= minThreshold && yValue <= maxThreshold) {
      joyMoved = false;
    }
    buttonReading();
  }
  else {
    xValue = analogRead(xAxisPin);
    if (xValue < minThreshold && joyMoved == false) {
      currentNumber[cursorIndex]--;
      if (currentNumber[cursorIndex] < 0)
        currentNumber[cursorIndex] = 9;
      joyMoved = true;
    }
    if (xValue > maxThreshold && joyMoved == false) {
      currentNumber[cursorIndex]++;  
      if (currentNumber[cursorIndex] > 9)
        currentNumber[cursorIndex] = 0;
      joyMoved = true;
    }
    if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;
    }
    buttonReading();
  }
}
