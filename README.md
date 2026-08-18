Care Voice — Smart Home Automation

Voice-controlled home automation built on the Arduino Nano 33 BLE Sense and an Elechouse Voice Recognition V3 module. Speak a command and the board switches relay-connected devices — light, fan, and buzzer/alarm — on or off.

Features
Voice-controlled ON/OFF for 3 devices: Light, Fan, Alarm/Buzzer
Runs entirely offline — no WiFi or cloud service required for recognition
Simple relay abstraction that supports both active-LOW and active-HIGH relay boards
Hardware
Component	Notes
Arduino Nano 33 BLE Sense	Main controller
Elechouse Voice Recognition V3	Offline voice command recognition
3-channel relay module	Switches connected devices
Light, fan, buzzer (or LEDs for testing)	The controlled devices
Wiring

Voice module (V3) → Nano 33 BLE Sense

V3 Pin	Nano Pin
TX	D0 (RX1)
RX	D1 (TX1)
VCC	3.3V / 5V (check module rating)
GND	GND

Relay module → Nano 33 BLE Sense

Relay Channel	Device	Nano Pin
IN1	Light	D2
IN2	Fan	D3
IN3	Buzzer/Alarm	D4

⚠️ Safety note: If wiring actual mains-voltage appliances (a lamp, a fan) through the relay, have someone experienced with mains wiring handle that step, or use a low-voltage/DC device while prototyping. Mains voltage can be lethal if wired incorrectly.

Setup
Install the VoiceRecognitionV3 library (Elechouse) via the Arduino Library Manager, or from github.com/elechouse/VoiceRecognitionV3.
Wire the V3 module and relay board as described above.
Train 6 voice commands into the V3 module using Elechouse's vrtrain example sketch (one-time step, run separately from this project):
Slot	Suggested phrase
0	"light on"
1	"light off"
2	"fan on"
3	"fan off"
4	"alarm on"
5	"alarm off"
You can use any words you like — just make sure the slot numbers match the CMD_* constants in care_voice.ino.
Upload care_voice.ino to the Nano 33 BLE Sense.
Open the Serial Monitor (115200 baud) to see recognized commands and relay state changes.
Project structure
care-voice/
├── care_voice.ino   # Main sketch
└── README.md
Roadmap / ideas
Add BLE app control alongside voice (Nano 33 BLE Sense supports this natively)
Add feedback tones (buzzer beep) confirming a command was heard
Expand to more devices/relays
Log commands with timestamps over serial or to an SD card
