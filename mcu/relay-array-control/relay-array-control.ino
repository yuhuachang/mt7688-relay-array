
// led to indicate an action
#define LED 13

// total number of digital pins
#define PIN_COUNT 20

// digital pin list
int pin[PIN_COUNT];

// pin state
int pinState[PIN_COUNT];

bool testMode = false;

void setup() {

  // digital pin D17, D0, D1, cannot use.
  // digital pin D13 is for LED.

  // initialize pin index and pin number
  pin[0] = 14;
  pin[1] = 15;
  pin[2] = 16;
  pin[3] = 2;
  pin[4] = 3;
  pin[5] = 4;
  pin[6] = 5;
  pin[7] = 6;
  pin[8] = 7;
  pin[9] = 8;
  pin[10] = 9;
  pin[11] = 10;
  pin[12] = 11;
  pin[13] = 12;
  pin[14] = 18;
  pin[15] = 19;
  pin[16] = 20;
  pin[17] = 21;
  pin[18] = 22;
  pin[19] = 23;

  // reset pin state
  for (int i = 0; i < PIN_COUNT; i++) {
    pinState[i] = LOW;
  }

  // set pin mode
  pinMode(LED, OUTPUT);
  for (int i = 0; i < PIN_COUNT; i++) {
    pinMode(pin[i], OUTPUT);
  }

  Serial.begin(9600); // debug serial
  Serial1.begin(57600); // to MPU
}

void loop() {

  // Read signal from MPU.  LED is on while reading.
  if (Serial1.available()) {
    digitalWrite(LED, HIGH);

    // MPU should send request in exactly 3 bytes binary.
    uint8_t buffer[3];
    Serial1.readBytes(buffer, sizeof(buffer) / sizeof(byte));

    // Debug output
    Serial.print("received: 0x");
    Serial.print(buffer[0], HEX);
    Serial.print(" 0x");
    Serial.print(buffer[1], HEX);
    Serial.print(" 0x");
    Serial.print(buffer[2], HEX);
    Serial.println("");

    if (buffer[2] >> 1 & 0x01 == 0x01) {
      Serial.println("test mode on");
      testMode = true;
    } else if (buffer[2] & 0x01 == 0x01) {
      Serial.println("write mode");
      for (int i = 0; i < 8; i++) {
        pinState[i] = buffer[0] >> (7 - i) & 0x01 == 1 ? HIGH : LOW;
      }
      for (int i = 0; i < 8; i++) {
        pinState[i + 8] = buffer[1] >> (7 - i) & 0x01 == 1 ? HIGH : LOW;
      }
      for (int i = 0; i < 4; i++) {
        pinState[i + 16] = buffer[2] >> (7 - i) & 0x01 == 1 ? HIGH : LOW;
      }
      // reset pin state
      for (int i = 0; i < PIN_COUNT; i++) {
        if (pinState[i] == HIGH) {
          digitalWrite(pin[i], HIGH);
          Serial.print(i);
          Serial.println(" on");
        } else {
          digitalWrite(pin[i], LOW);
          Serial.println(i);
        }
      }
      Serial.println();
    } else {
      // read mode
      testMode = false;
    }

    // Clear input
    while (Serial1.available()) {
      Serial1.read();
    }

    // Return current pin status
    buffer[0] = 0x00;
    for (int i = 0; i < 8; i++) {
      buffer[0] |= pinState[i] << (7 - i);
    }
    buffer[1] = 0x00;
    for (int i = 0; i < 8; i++) {
      buffer[1] |= pinState[i + 8] << (7 - i);
    }
    buffer[2] = 0x00;
    for (int i = 0; i < 4; i++) {
      buffer[2] |= pinState[i + 16] << (7 - i);
    }

    Serial.print(buffer[0], HEX);
    Serial.print(" ");
    Serial.print(buffer[1], HEX);
    Serial.print(" ");
    Serial.print(buffer[2], HEX);
    Serial.println("");

    Serial1.write(buffer, 3);

    // Clear output
    Serial.flush();
    Serial1.flush();

    // Completed
    digitalWrite(LED, LOW);
  }

  // Execute the action
  if (testMode) {
      // Fast flashing.  This can be used to verify which board it is.
      digitalWrite(LED, HIGH);
      delay(100);
      digitalWrite(LED, LOW);
      delay(100);
  }
}

