int readFromEEPROM() {
  EEPROM_readAnything(0, highScoreStruct);
}

void writeToEEPROM() {
  EEPROM_writeAnything(0, highScoreStruct);
}

void clearEEPROM() {
  for (int i = 0 ; i < EEPROM.length() ; i++) {
    EEPROM.write(i, 0);
  }
}
