# MIDI-Zapping
A DIY Arduino-Based Device for fast MIDI Channel Switching 

![schematic](https://github.com/d-maniatakos/MIDI-Zapping/blob/master/Photo.png?raw=true)

## The Idea
When performing live,musicians need to be able to switch fast and seamlessly between their sounds/presets without interrupting their performance.For musicians using
MIDI(acronym for Musical Instrument Digital Interface) Devices, an effective way to achieve this is by assigning each of their sounds to a different midi channel.
However,some MIDI Devices on the market don't offer buttons/controllers for fast channel change(but instead time-consuming combinations of buttons etc.)
and many musicians end up using their laptop's keyboard for changing sounds which can be quite uncomfortable and non-successful.The idea behind this DIY Arduino Device
is to give the performer the capability to switch between MIDI channels easily with the press of a button 

### Requirements

- Arduino UNO R3
- Push Buttons  (set of 3)
- Potentiometer
- 10KΩ Resistor (Set of 3, one for each Push Button)
- 7-Segment Display
- 1KΩ Resistor  (for the Segment Display)
- Breadboard
- Jumper Wires
- A Midi Device( e.g a Midi Keyboard) (Optional - For Testing)
- A Software for virtual midi ports ( like loopMIDI) (Optional - For Testing)
- Hairless MIDI (or an alternative if it exists) (Optional - For Testing)

### Schematic
![schematic](https://github.com/d-maniatakos/MIDI-Zapping/blob/master/Schematic.png?raw=true)

### How To Use

- Implement the schematic and load the code to your Arduino UNO

- Plug your MIDI Device to your Computer

- Install LoopMIDI (https://www.tobias-erichsen.de/software/loopmidi.html) and create a virtual MIDI port

- Run Hairless MIDI (https://projectgus.github.io/hairless-midiserial/) ,set Baud Rate to 9600, in MIDI In chose your MIDI Device, in MIDI Out chose the virtual port you created in step 3 and in Serial Port your Arduino
Device

- Start your favourite VST Host or DAW and use the virtual MIDI port you created in step 3 as the MIDI Input

### How it works

The device consists of three push buttons(left one for "Previous Midi Channel,middle one for "Next Midi Channel" and right one for "1st Midi Channel),a 7-Segment Display
which shows the current MIDI Channel and a potentiometer which is used as an ordinary Continuous MIDI Controller(can be assigned to control the volume of the current
sound via the virtual instrument or whatever other parameter).The push buttons work as digital inputs to Arduino,the Potentiometer as analog input and the 
7-Seg Display as set of digital outputs.There are also some resistors to control voltages.The "Brain" of the controller is, of course, the Arduino UNO R3.
The code,receives from the serial port(USB) the midi messages(which should be on channel 1 for the program to work)and "translates" them to the exact same messages 
but on another channel.Then,the "translated" midi message is sent back to the serial port and then can be outputted to a virtual midi port(using software like 
Hairless MIDI).The tricky part was switching to another channel/sound without the held notes from the previous one cutting or staying forever.To achieve this,
a 2-Dimensional array which keeps information ( on / off) about every note in every channel was used and,thus, it's possible to cut-off the held notes from a previous 
channel when the keys,for example, are released even if channel has changed (by sending the appropriate midi message).The exact same algorithm was used to cope
with held-sustain pedal from a previous channel.

### Limitations

In MIDI technology there are ,in total, 16 channels a device can send messages to.However,the current implementation of the code supports up to 9 channels because the 
2-Dimensional array used to store information about every note on every channel would cause global variable memory excess in Arduino UNO R3.A new optimised algorithm
is required to solve this problem

### Credits

- Dionysis Maniatakos - Conceptualisation,Programming,Implementation

- Giorgos Maniatakos   -  Project Name Suggestion,Ideas for improvement

- All the arduino tutorials out there (MIDI-related or not) for their valuable information 

##### Disclaimer

<sub>This is my first Arduino Project (my first GitHub repository as well) and by no means am I expert in the equipment and terminology used(there may be inaccuracies).
I cannot guarantee that the connections/resistors used are the best possible.Implement the schematic/code at your own risk!</sub>
