#include <MUX74HC4067.h>
//#include <MIDI.h>
#include "MIDIUSB.h" // Pro Micro

//MIDI_CREATE_DEFAULT_INSTANCE();

MUX74HC4067 mux(2, 3, 4, 5, 6); // Multiplexer Pins

// Button and Potentiometer Count
const int bnum = 12;

// Potentiometer Pins
int pot1 = A1;
int pot2 = A2;
int vol = A0;

// Potentiometer State
int oldp = 0; 
int newp = 0;
int oldp1 = 0; 
int newp1 = 0;
int oldp2 = 0; 
int newp2 = 0;

// Encoder
int add = 0;

// Button State
int oldstate[bnum] = {0};
int newstate[bnum] = {0};

// MIDI Notes
int note[bnum] = {36, 37, 38, 39, 40, 41, 42, 43, 45, 46, 47, 48};
int oct = 0;
int ch = 0;
int bank = 0;

void setup() {
  // Mux SIG0 Pin Setup
  mux.signalPin(7, INPUT_PULLUP, DIGITAL);

  // Potentiometer Pin Setup
  pinMode(vol, INPUT_PULLUP);
  pinMode(pot1, INPUT_PULLUP);
  pinMode(pot2, INPUT_PULLUP);

  // Serial Port Setup
   Serial.begin(9600);

}


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
 // Pro Micro

void loop() {
  // Button Method
  button();

  // Potentiomter Method
  pot();
  
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

    // Octave Change
    if(oldstate[8] != newstate[8]) {
      if(newstate[8] == LOW) {
        oct = oct + 12;
        
      }
      oldstate[8] = newstate[8];

    } else if(oldstate[9] != newstate[9]) {
      if(newstate[9] == LOW) {
        oct = oct - 12;
        
      }
      oldstate[9] = newstate[9]; 
      
    }

    // Channel Selection
    if(oldstate[0] != newstate[0]) {
      if(newstate[0] == LOW) {
        ch = 0;
        
      }
      oldstate[0] = newstate[0];

    } else if(oldstate[1] != newstate[1]) {
      if(newstate[1] == LOW) {
        ch = 1;
        
      }
      oldstate[1] = newstate[1];

    } else if(oldstate[2] != newstate[2]) {
      if(newstate[2] == LOW) {
        ch = 2;
        
      }
      oldstate[2] = newstate[2];

    } else if(oldstate[3] != newstate[3]) {
      if(newstate[3] == LOW) {
        ch = 3;
        
      }
      oldstate[3] = newstate[3];

    }

    // Program Change
    if(oldstate[10] != newstate[10] && bank<16) {
      if(newstate[10] == LOW) {
        bank++;
        //MIDI.sendControlChange(12, bank, ch);
        controlChange(ch, 12, bank); 
        MidiUSB.flush(); // Pro Micro
        
      }
      oldstate[10] = newstate[10];

    } else if(oldstate[11] != newstate[11] && bank>0) {
      if(newstate[11] == LOW) {
        bank--;
        //MIDI.sendControlChange(12, bank, ch);
        controlChange(ch, 12, bank); 
        MidiUSB.flush(); // Pro Micro
        
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
          //MIDI.sendNoteOn(note[i] + oct, 127, ch);
          noteOn(ch, note[i] + oct, 127); 
          MidiUSB.flush(); // Pro Micro

        } 
        
        // Turning Off Midi Notes
        else {
          //MIDI.sendNoteOff(note[i] + oct, 0, ch);
          noteOff(ch, note[i] + oct, 0); 
          MidiUSB.flush(); // Pro Micro
          
        }

        oldstate[i] = newstate[i];
      }
      
    }
    
  }
  
}

void pot() {
  // Potentiomenter Reading
  newp = analogRead(vol)/8;
  newp1 = analogRead(pot1)/8;
  newp2 = analogRead(pot2)/8;

  // Sending CC7 to Midi
  if(oldp != newp) {
    //MIDI.sendControlChange(7, newp, ch);
    controlChange(ch, 6, newp); 
    MidiUSB.flush(); // Pro Micro
    
    oldp = newp;  
  }

  // Encoder Dependent CC
  if(oldp1 != newp1) {
    //MIDI.sendControlChange(8, newp, ch);
    controlChange(ch, 7 + add, newp1); 
    MidiUSB.flush(); // Pro Micro
    
    oldp1 = newp1;  
  }

  if(oldp2 != newp2) {
    //MIDI.sendControlChange(9, newp, ch);
    controlChange(ch, 8 + add, newp2); 
    MidiUSB.flush(); // Pro Micro
    
    oldp2 = newp2;  
  }
  
}
