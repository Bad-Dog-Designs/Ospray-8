byte incomingByte;
byte note;
byte velocity;

// OSPRAY-8 Midi Decoding 
// Bank select and Aux triggers not implemented
// 29/7/25

#define MIDI_CHANNEL_1 A5
#define MIDI_CHANNEL_2 A4
#define MIDI_CHANNEL_3 A3
#define MIDI_CHANNEL_4 A2

// Returns the MIDI channel selected via the DIP switches
int getMidiChannel() {
  int channel{1};
  int pins[]{MIDI_CHANNEL_4, MIDI_CHANNEL_3, MIDI_CHANNEL_2, MIDI_CHANNEL_1};

  for (int i = 0; i < 4; i++) {
    pinMode(pins[i], INPUT_PULLUP);
    
    channel += (1 << i) * !digitalRead(pins[i]);
    
    // Release internal pull-up resistor
    pinMode(pins[i], INPUT);
  }

  return channel;
}



int noteDown = LOW; 
int state=0;  // state machine variable 0 = command waiting : 1 = note waitin : 2 = velocity waiting

int channel = 0; // Initial Midi channel, prior to reading DIP switches

//setup: declaring iputs and outputs and begin serial 
void setup() { 
  pinMode(2,OUTPUT);        
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  state = 0;  // initilise state machine variable
  //start serial with MIDI baudrate 31250 or 38400 for debugging
  Serial.begin(31250);        
  channel = (getMidiChannel()-1); // Read Midi Channel from hardware switches and assign
}

//loop: wait for serial data, and interpret the message 
void loop () {

  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    switch (state){
    case 0:
      // look for as status-byte, our channel, note on
      if (incomingByte== (144 | channel)){ 
        noteDown = HIGH;
        state=1;
      }
      // look for as status-byte, our channel, note off
      if (incomingByte== (128 | channel)){ 
        noteDown = LOW;
        state=1;
      }

    case 1:
      // get the note to play or stop
      if(incomingByte < 128) {
        note=incomingByte;
        state=2;
      }
      else{
        state = 0;  // reset state machine as this should be a note number
      }
      break;

    case 2:
      // get the velocity
      if(incomingByte < 128) {
        playNote(note, incomingByte, noteDown); // fire off the output
      }
      state = 0;  // reset state machine to start            
    }
  }
}

void playNote(byte note, byte velocity, int down){
  // if velocity = 0 on a 'Note ON' command, treat it as a note off
  if ((down == HIGH) && (velocity == 0)){
    down = LOW; 
  }
  int note2 = note;
  if (note2 == 60) {
    digitalWrite(2, down);
  } 
  if (note2 == 62) {
    digitalWrite(3, down);
  }
  if (note2 == 64) {
    digitalWrite(4, down);
  }
  if (note2 == 65) {
    digitalWrite(5, down);
  }
  if (note2 == 67) {
    digitalWrite(6, down);
  } 
  if (note2 == 69) {
    digitalWrite(7, down);
  }
  if (note2 == 71) {
    digitalWrite(8, down);
  }
  if (note2 == 72) {
    digitalWrite(9, down);
  }
}

