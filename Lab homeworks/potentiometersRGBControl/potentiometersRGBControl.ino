const int redLedInput = A0;
const int greenLedInput = A1;
const int blueLedInput = A2;
const int redLedOutput = 11;
const int greenLedOutput = 10;
const int blueLedOutput = 9;
int redIntensity = 0;
int greenIntensity = 0;
int blueIntensity = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(redLedInput, INPUT);
  pinMode(greenLedInput, INPUT);
  pinMode(blueLedInput, INPUT);
  pinMode(redLedOutput, OUTPUT);
  pinMode(greenLedOutput, OUTPUT);
  pinMode(blueLedOutput, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  redIntensity = map(analogRead(redLedInput), 0, 1023, 0, 255);
  greenIntensity = map(analogRead(greenLedInput), 0, 1023, 0, 255);
  blueIntensity = map(analogRead(blueLedInput), 0, 1023, 0, 255);
  setColor(redIntensity, greenIntensity, blueIntensity);
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redLedOutput, redValue);
  analogWrite(greenLedOutput, greenValue);
  analogWrite(blueLedOutput, blueValue);
}
