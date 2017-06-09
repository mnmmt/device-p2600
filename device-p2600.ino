// Teensy Midi Controller
// Original by Philip Cunningham

// Edit device name before compiling:
// bin/contrib/arduino-1.6.12/hardware/teensy/avr/cores/usb_midi/usb_private.h

// noise tolerance
const int THRESHOLD = 4;

// The number of pots
const int pots = 2;

int ledPin = 11;
int btnState = 1;

int pot_lookup[pots] = {A0, A1};

// previous read value for thresholding
int lastreading[pots];
int weighted[pots];

// Set midi channel to send to
int channel = 2;

void setup()
{
  for (int p = 0; p < pots; p++) {
    lastreading[p] = 0;
    weighted[p] = 0;
  }

  // Standard midi rate
  Serial.begin(31250);

  pinMode(pot_lookup[0], INPUT); 
  digitalWrite(pot_lookup[0], LOW);

  pinMode(pot_lookup[1], INPUT); 
  digitalWrite(pot_lookup[1], HIGH);
  usbMIDI.sendControlChange(1, 127, channel);

  // flash to indicate startup
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(250);
  digitalWrite(ledPin, HIGH);
  delay(250);
  digitalWrite(ledPin, LOW);
  delay(250);
  digitalWrite(ledPin, HIGH);
}

void update_button(int pin, int btnState) {
  usbMIDI.sendControlChange(pin, btnState ? 127 : 0, channel);
  digitalWrite(ledPin, btnState ? HIGH : LOW);
}

void loop()
{
  // Get reading for all potentiometers
  for (int i = 0; i < pots; i++ ) {
    // http://electronics.stackexchange.com/a/64699
    weighted[i] += (analogRead(pot_lookup[i]) - weighted[i]) / (i == 0 ? 8 : 4);
    int midival = weighted[i] / 8;
    int midiprev = lastreading[i] / 8;
    if (abs(lastreading[i] - weighted[i]) > THRESHOLD && midival != midiprev) {
      //usbMIDI.sendControlChange(2, 64, channel);
      if (i == 0) {
        usbMIDI.sendControlChange(i, 127 - midival, channel);
      } else if (i == 1) {
        if (midival < 4 && btnState == 0) {
          btnState = 1;
          update_button(i, btnState);
        } else if (midival > 4 && btnState == 1) {
          btnState = 0;
          update_button(i, btnState);
	}
      }
      lastreading[i] = weighted[i];
    }
  }
  // Delay output
  delay(10);
}
