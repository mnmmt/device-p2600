// Teensy Midi Controller
// Original by Philip Cunningham

// Edit device name before compiling:
// bin/contrib/arduino-1.6.12/hardware/teensy/avr/cores/usb_midi/usb_private.h

// noise tolerance
const int THRESHOLD = 3;

// The number of pots
const int pots = 2;

// look up each pot
//int pot_lookup[pots] = {A0, A1, A2, A3, A4, A5};
//int pot_lookup[pots] = {A0, A4, A5};
// int pot_lookup[pots] = {A0, A4, A5};

int ledPin = 11;
int btnState = 1;

int pot_lookup[pots] = {A0, A1};

// previous read value for thresholding
int lastreading[pots];

// Set midi channel to send to 1
int channel = 2;

void setup()
{
  for ( int p = 0; p < pots; p++ ) {
    lastreading[p] = 0;
  }

  // Standard midi rate
  Serial.begin(31250);
  
  pinMode(pot_lookup[0], INPUT); 
  digitalWrite(pot_lookup[0], LOW);

  pinMode(pot_lookup[1], INPUT); 
  digitalWrite(pot_lookup[1], HIGH);
  usbMIDI.sendControlChange(1, 127, channel);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}

void loop()
{
  // Get reading for all potentiometers
  //for (int i = 0; i < pots; i++ ) {

   // Read from the sensor
   int i = 0;
   int r = analogRead(pot_lookup[i]);

   // If reading is different from the previous
   if (abs(r - lastreading[i]) >= THRESHOLD && r / 8 != lastreading[i] / 8){
     // Send midi data
     usbMIDI.sendControlChange(i, r / 8, channel);
    // Set last array value to current value
    lastreading[i] = r;
   }
  //}

  i = 1;
  r = analogRead(pot_lookup[i]);
  
   // If reading is different from the previous
   if (abs(r - lastreading[i]) >= THRESHOLD && r / 8 != lastreading[i] / 8){
     // Send midi data
     if (r / 8 < 4 && lastreading[i] / 8 >= 4) {
       //usbMIDI.sendControlChange(i, r / 8, channel);
       btnState = !btnState;
       usbMIDI.sendControlChange(1, btnState ? 127 : 0, channel);
       digitalWrite(ledPin, btnState ? HIGH : LOW);
     }
    // Set last array value to current value
    lastreading[i] = r;
   }

  //if (r / 8 < 5 && lastreading[i] / 8 >= 5) {
  //  btnState = !btnState;
  //  usbMIDI.sendControlChange(1, btnState ? 127 : 0, channel);
  //  digitalWrite(ledPin, btnState ? HIGH : LOW);
  //  lastreading[i] = two;
  //}
  
  // Delay output
  delay(50);
}
