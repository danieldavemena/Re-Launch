#include <MUX74HC4067.h>
#include <MIDI.h>
// #include "MIDIUSB.h" // Pro Micro

MIDI_CREATE_DEFAULT_INSTANCE();

MUX74HC4067 mux(2, 3, 4, 5, 6); // Multiplexer Pins

// Button and Potentiometer Count
const int bnum = 12;
const int pnum = 2;

//Button Mode Pins
int midi_mode = 10;
int key_mode = 11;

// Potentiometer Pins
int pots[pnum] = {A1, A2};
int vol = A0;

// Potentiometer State
int oldstatep[pnum] = {0}; 
int newstatep[pnum] = {0};
int oldp = 0; 
int newp = 0;

// Encoder
int add = 0;

// Button State
int oldstate[bnum] = {0};
int newstate[bnum] = {0};

// MIDI Notes
int note[bnum] = {36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48};
int oct = 0;
int ch = 1;
int bank = 0;

void setup() {
  // Mux SIG0 Pin Setup
  mux.signalPin(7, INPUT_PULLUP, DIGITAL);

  // Potentiometer Pin Setup
  for(int i=0; i<pnum; i++) {
    pinMode(pots[i], INPUT);
  }

  pinMode(midi_mode, INPUT_PULLUP);
  pinMode(key_mode, INPUT_PULLUP);
  pinMode(vol, INPUT);

  // Serial Port Setup
   Serial.begin(115200);

}

/* 
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
*/ // Pro Micro

void loop() {
  if (midi_mode == 0) { // Launchpad Mode
    // Button Method
    button();

    // Potentiomter Method
    pot();
  } else if (key_mode == 0) { // Keyboard Mode
    // Keyboard Method
    key();
  }
  
}

void button() {
  // Reading for Mode Button 
  int mode = mux.read(12);

  // Mode Select
  if(mode == 0) { 
    
    // Reading for Modes
    for(int i=0; i<bnum; i++) {
      newstate[i] = mux.read(i);
    }

    // Octave Up
    if(oldstate[8] != newstate[8]) {
      if(newstate[8] == LOW) {
        oct = oct + 12;
        
      }
      oldstate[8] = newstate[8];

    } 

    // Octave Down
    else if(oldstate[9] != newstate[9]) {
      if(newstate[9] == LOW) {
        oct = oct - 12;
        
      }
      oldstate[9] = newstate[9]; 
      
    }

    // Channel Selection
    if(oldstate[0] != newstate[0]) {
      if(newstate[0] == LOW) {
        ch = 1;
        
      }
      oldstate[0] = newstate[0];

    } else if(oldstate[1] != newstate[1]) {
      if(newstate[1] == LOW) {
        ch = 2;
        
      }
      oldstate[1] = newstate[1];

    } else if(oldstate[2] != newstate[2]) {
      if(newstate[2] == LOW) {
        ch = 3;
        
      }
      oldstate[2] = newstate[2];

    } else if(oldstate[3] != newstate[3]) {
      if(newstate[3] == LOW) {
        ch = 4;
        
      }
      oldstate[3] = newstate[3];

    }

    // Program Change
    if(oldstate[10] != newstate[10] && bank<16) {
      if(newstate[10] == LOW) {
        bank++;
        MIDI.sendControlChange(12, bank, ch);
        // controlChange(ch, 12, bank); 
        // MidiUSB.flush(); // Pro Micro
        
      }
      oldstate[10] = newstate[10];

    } else if(oldstate[11] != newstate[11] && bank>0) {
      if(newstate[11] == LOW) {
        bank--;
        MIDI.sendControlChange(12, bank, ch);
        // controlChange(ch, 12, bank); 
        // MidiUSB.flush(); // Pro Micro
        
      }
      oldstate[11] = newstate[11];

    }
    
  } 
  
  // Mode Button Not Pressed
  else if(mode != 0) {
    for(int i=0; i<bnum; i++) {
      newstate[i] = mux.read(i);
      
      if(oldstate[i] != newstate[i]) {

        // Sending Midi Notes
        if(newstate[i] == 0) {
          MIDI.sendNoteOn(note[i] + oct, 127, ch);
          // noteOn(ch, note[i] + oct, 127); 
          // MidiUSB.flush(); // Pro Micro

        } 
        
        // Turning Off Midi Notes
        else {
          MIDI.sendNoteOff(note[i] + oct, 0, ch);
          // noteOff(ch, note[i] + oct, 0); 
          // MidiUSB.flush(); // Pro Micro
          
        }

        oldstate[i] = newstate[i];
      }
      
    }
    
  }
  
}

void pot() {
  // CC7 Potentiomenter Reading
  int c = analogRead(vol)/8;
  newp = c;

  // Sending CC7 to Midi
  if(oldp != newp) {
    MIDI.sendControlChange(7, newp, ch);
    // controlChange(ch, 7, newp); 
    // MidiUSB.flush(); // Pro Micro
    
    oldp = newp;
    
  }

  for(int i=0; i<pnum; i++) {
    newstatep[i] = analogRead(pots[i])/8;

    if(oldstatep[i] != newstatep[i]) {
      MIDI.sendControlChange(8 + i + add, newstatep[i], ch);
      // controlChange(ch, 8 + i + add, newp); 
      // MidiUSB.flush(); // Pro Micro
    }
  }
}

void key() {

}