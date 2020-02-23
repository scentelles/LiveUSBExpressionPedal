#include "MIDIUSB.h"

// Expression pedal pin
#define expPin A0


int lastVal = 0;
int tempAnalog = 0;

int MIDI_CC_EXPRESSION = 11;
int MIDI_CC_WHEEL = 1;

int MIDI_CC = MIDI_CC_WHEEL;

void setup() {
  Serial.begin(300);  
}

void controlChange(byte channel, byte control, byte value) {
   midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

void loop() {

  // Read analog value
  tempAnalog = analogRead(expPin);
  
  // Convert 10 bit to 7 bit
  tempAnalog = map(tempAnalog, 0, 1023, 0, 127);
  tempAnalog = constrain(tempAnalog, 0, 127);

  // Send pedal status
  if(tempAnalog != lastVal)
  {
    controlChange(1, MIDI_CC, tempAnalog);
    // !Important, Flush after send
    MidiUSB.flush();
  }

  // Store current value to be used later
  lastVal = tempAnalog;
  delay(5);  
}
