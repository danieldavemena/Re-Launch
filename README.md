# Re: Launch
**A Do-it-Yourself Arduino launchpad for music production and live performance.**
#
Re: Launch is a free and open-source software for Arduino microcontrollers such as Arduino Uno, Leonardo and Pro Micro. Musicians, including me, know how expensive a commercial equipment can be and in order for more people to have easier access to music production equipments, I have made the materials as cheap as possible to obtain.
#

**Requirements:**


Software Requirements:
- MIDIUSB (for Pro Micro)
- MIDI (for Uno)
- MUX74HC4067 - https://github.com/nlamprian/MUX74HC4067


Hardware Requirements:
- Arduino board
- Micro USB data cable
- Perfboard (size that fits your choice of arduino board)
- Female and male pin headers (optional)
- Solder & Soldering iron
- 12pcs 24mm arcade buttons
- 3pcs 10K ohm potentiometer
- 1pc rotary encoder
- 2pcs 6x6 inches arcylic glass
- 4pcs  M3 standoffs
- CD74HC4067 multiplexer board
- 4pcs volume knobs

#
**Build Instruction**
#
1. Preparing the enclosure
  > Print "drilltemplate.svg"
  - Attach the template on the acrylic with adhesive
  - Start drilling holes
  - NOTE: MAKE SURE TO DRILL A HOLE IN THE MIDDLE BEFOR PROCEEDING WITH HOLE SAW

2. Preparing the software
  - Download the necessary libraries
  - Upload the arduino code "Re_Launch.ino"
  - Test the connections on a breadboard, follow "Schematics.png"

3. Preparing the perfboard
  - If using a female and male pin headers, attach the female header to the perfboard then attach the multiplexer and arduino to its proper connections
  - If not, attach the wires and boards directly to the perfboard

4. Finishing
  - Attach the buttons, potentiometers, encoder, and the perfboard to the aryclic according to the template
  - Start soldering the parts
  - Close the case by using M3 standoffs
  - Connect Re: Launch with your PC and test with your favorite DAW
