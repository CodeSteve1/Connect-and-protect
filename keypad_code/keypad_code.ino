#include <Keypad.h>

// Keypad setup
const byte ROWS = 4;
const byte COLS = 4;

// Mapping the keys
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connect row pins to these ESP32 pins
byte rowPins[ROWS] = {13, 12, 4, 19};  // Updated R3 and R4 pins

// Connect column pins to these ESP32 pins
byte colPins[COLS] = {26, 25, 33, 32};

// Keypad initialization
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Timing and button press logic
unsigned long lastPressTime = 0;
unsigned long pressTimeout = 1000; // Time to cycle letters, 1 second

int lastKey = -1;
int pressCount = 0;

const char *keyMap[10] = {
  ".,!",  // For '1'
  "ABC",  // For '2'
  "DEF",  // For '3'
  "GHI",  // For '4'
  "JKL",  // For '5'
  "MNO",  // For '6'
  "PQRS", // For '7'
  "TUV",  // For '8'
  "WXYZ", // For '9'
  " "     // For '0'
};

void setup() {
  Serial.begin(115200); // Start serial for debugging
}

void loop() {
  while(1){
  char key = keypad.getKey();

  // If a key is pressed
  if (key) {
    int keyIndex = -1;

    // Map keypad input to 0-9 for old phone-style input
    if (key >= '1' && key <= '9') {
      keyIndex = key - '1';
    } else if (key == '0') {
      keyIndex = 9;
    }

    // Handle multi-press input for letters
    if (keyIndex != -1) {
      if (lastKey == keyIndex && (millis() - lastPressTime) < pressTimeout) {
        // Still pressing the same key within timeout
        pressCount++;
        pressCount %= strlen(keyMap[keyIndex]); // Cycle through the letters
      } else {
        // New key or timeout passed, reset the counter
        pressCount = 0;
      }

      lastKey = keyIndex;
      lastPressTime = millis();

      // Print the current character to the Serial Monitor
      char currentChar = keyMap[keyIndex][pressCount];
      Serial.print("Selected Character: ");
      Serial.println(currentChar);
    }
  }

  // Check if no key is pressed and the timeout has passed
  if (lastKey != -1 && (millis() - lastPressTime) > pressTimeout) {
    lastKey = -1; // Reset key tracking after timeout
  }
}}
