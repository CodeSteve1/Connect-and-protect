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
byte rowPins[ROWS] = {13, 12, 15, 21};  // Updated R3 and R4 pins
byte colPins[COLS] = {26, 25, 33, 32};

// Keypad initialization
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Timing and button press logic
unsigned long lastPressTime = 0;
unsigned long pressTimeout = 800; // Time to cycle letters, 1 second

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

// String to store all typed characters
String typedText = "";

void setup() {
  Serial.begin(115200); // Start serial for debugging
}

void loop() {
  char key = keypad.getKey();
  if (key) {
    int keyIndex = -1;

    
    if (key == '*') {
      if (typedText.length() > 0) {
        typedText.remove(typedText.length() - 1); 
        Serial.print("Backspace pressed. Current text: ");
        Serial.println(typedText);
      }
      return; 
    }

    
    else if (key >= '1' && key <= '9') {
      keyIndex = key - '1';
    } else if (key == '0') {
      keyIndex = 9;
    }

    
    if (keyIndex != -1) {
      
      if (lastKey == keyIndex && (millis() - lastPressTime) < pressTimeout) {
        pressCount++; // Move to the next letter
        pressCount %= strlen(keyMap[keyIndex]); // Cycle through the letters
      } else {
        // New key or timeout passed, finalize the previous character
        if (lastKey != -1) {
          char finalChar = keyMap[lastKey][pressCount];
          typedText += finalChar;
        }
        // Reset for new key press
        pressCount = 0;
      }

      lastKey = keyIndex;
      lastPressTime = millis();

      // Show the current character being cycled but don't select it yet
      char currentChar = keyMap[keyIndex][pressCount];
      Serial.print("Cycling Character: ");
      Serial.println(currentChar);
    }
  }

  // Finalize the character if timeout has passed without further key presses
  if (lastKey != -1 && (millis() - lastPressTime) > pressTimeout) {
    char finalChar = keyMap[lastKey][pressCount];
    typedText += finalChar;
    Serial.print("Character automatically selected after timeout: ");
    Serial.println(finalChar);

    lastKey = -1; // Reset key tracking after finalizing the character

    // Print the current typed text after the timeout and selection
    Serial.print("Current Text: ");
    Serial.println(typedText);
  }
}
