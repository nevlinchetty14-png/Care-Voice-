/*
  Care Voice — Smart Home Automation
  -----------------------------------
  Board:  Arduino Nano 33 BLE Sense
  Module: Elechouse Voice Recognition V3

  What it does:
  - Listens for trained voice commands
  - Controls 3 relay-switched devices: Light, Fan, Buzzer/Alarm
  - Each device has an ON and OFF voice command (6 commands total)

  WIRING (Elechouse V3 -> Nano 33 BLE Sense):
    V3 TX  -> Nano RX1 (pin D0)
    V3 RX  -> Nano TX1 (pin D1)
    V3 VCC -> 3.3V or 5V (check your module's rating)
    V3 GND -> GND

  Relay Module (3-channel, active LOW typical — check your relay board):
    Relay IN1 (Light) -> D2
    Relay IN2 (Fan)   -> D3
    Relay IN3 (Buzzer/Alarm) -> D4
    Relay VCC -> 5V
    Relay GND -> GND

  IMPORTANT: Relays switch mains-voltage devices. If wiring actual mains
  appliances (lamp, fan), have someone experienced with mains wiring do
  that part, or use low-voltage devices (LED strip, DC fan, 5V buzzer)
  while learning. Mains voltage can be lethal.

  SETUP (one-time, before uploading this sketch):
  Train 6 voice commands into the V3 module using Elechouse's
  "vrtrain" example sketch. Suggested words and slot numbers below —
  you can rename/reorder as long as the CMD_ constants below match.

  Library required: "VoiceRecognitionV3" by Elechouse
  https://github.com/elechouse/VoiceRecognitionV3
*/

#include "VR.h"

VR myVR(&Serial1); // V3 module on hardware Serial1 (D0/D1)

uint8_t buf[64];

// --- Trained command slots (must match your vrtrain recordings) ---
#define CMD_LIGHT_ON   0   // e.g. "light on"
#define CMD_LIGHT_OFF  1   // e.g. "light off"
#define CMD_FAN_ON     2   // e.g. "fan on"
#define CMD_FAN_OFF    3   // e.g. "fan off"
#define CMD_ALARM_ON   4   // e.g. "alarm on"
#define CMD_ALARM_OFF  5   // e.g. "alarm off"

const uint8_t NUM_COMMANDS = 6;

// --- Relay pins ---
const int RELAY_LIGHT = 2;
const int RELAY_FAN   = 3;
const int RELAY_ALARM = 4;

// Most relay boards are "active LOW" (LOW = relay energized = device ON).
// Set this to false if your relay board is active HIGH instead.
const bool RELAY_ACTIVE_LOW = true;

void setup() {
  Serial.begin(115200);
  while (!Serial && millis() < 3000) { /* wait briefly for USB serial */ }

  Serial1.begin(9600);
  myVR.begin(9600);

  pinMode(RELAY_LIGHT, OUTPUT);
  pinMode(RELAY_FAN, OUTPUT);
  pinMode(RELAY_ALARM, OUTPUT);

  setRelay(RELAY_LIGHT, false);
  setRelay(RELAY_FAN, false);
  setRelay(RELAY_ALARM, false);

  Serial.println("Care Voice ready. Loading trained commands...");

  for (uint8_t i = 0; i < NUM_COMMANDS; i++) {
    if (myVR.load(i) >= 0) {
      Serial.print("Loaded command slot ");
      Serial.println(i);
    } else {
      Serial.print("WARNING: slot ");
      Serial.print(i);
      Serial.println(" not trained yet. Run vrtrain first.");
    }
  }

  Serial.println("Listening...");
}

void loop() {
  int ret = myVR.recognize(buf, 50);

  if (ret > 0) {
    uint8_t record = buf[1];
    handleCommand(record);
  }
}

void handleCommand(uint8_t record) {
  switch (record) {
    case CMD_LIGHT_ON:
      setRelay(RELAY_LIGHT, true);
      Serial.println("Light: ON");
      break;
    case CMD_LIGHT_OFF:
      setRelay(RELAY_LIGHT, false);
      Serial.println("Light: OFF");
      break;
    case CMD_FAN_ON:
      setRelay(RELAY_FAN, true);
      Serial.println("Fan: ON");
      break;
    case CMD_FAN_OFF:
      setRelay(RELAY_FAN, false);
      Serial.println("Fan: OFF");
      break;
    case CMD_ALARM_ON:
      setRelay(RELAY_ALARM, true);
      Serial.println("Alarm: ON");
      break;
    case CMD_ALARM_OFF:
      setRelay(RELAY_ALARM, false);
      Serial.println("Alarm: OFF");
      break;
    default:
      Serial.print("Unrecognized command slot: ");
      Serial.println(record);
      break;
  }
}

// Turns a relay on or off, accounting for active-LOW vs active-HIGH boards.
void setRelay(int pin, bool turnOn) {
  bool level = RELAY_ACTIVE_LOW ? !turnOn : turnOn;
  digitalWrite(pin, level ? HIGH : LOW);
}