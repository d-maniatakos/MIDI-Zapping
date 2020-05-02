#define MAX_CHANNEL 9   //Desired number of midi channels/sounds to switch to(see README file -> Limitations)
#define KNOB_CC 10      //Midi Continuous Control Parameter to assign to the pontetiometer(see README file -> The Idea)       

int channel=1;   //current midi channel

/*Push Buttons*/
const int init_button_pin = 2; //button for "channel 1" - right push button
const int next_button_pin = 3; //button for "next channel" - middle push button
const int prev_button_pin = 4; //button for "previous channel" - left push button  

int knob_Pin = 0;     //potentiometer
int knob_value=0;     //potentiometer current value
int prev_knob_value=0;//potentiometer previous value

/*Button States (used to avoid having a button push recognised more than one times/in more than one loops)*/  
bool init_button_state=false;
bool next_button_state=false;
bool prev_button_state=false;  

/*Again used to avoid having a button press become recognised as duple press*/
unsigned long next_buttom_time;
unsigned long prev_buttom_time;

/*7 Segment Led Display*/
unsigned const int A = 6;
unsigned const int B = 5;
unsigned const int C = 9;
unsigned const int D = 10;
unsigned const int E = 11;
unsigned const int F = 7;
unsigned const int G = 8;

bool active_notes[MAX_CHANNEL][127]; //Used to store what notes are currently(on) pressed in what channel
bool active_sustain[MAX_CHANNEL];    //Used to store the sustain state(on/off) in each channel   

/*Midi Command Bytes(Each Midi Message includes 3 bytes)*/
byte firstByte;  //First Midi Byte
byte secondByte; //Second Midi Byte  
byte thirdByte;  //Third Midi Byte



void setup()
{
  /*Push Buttons*/
  pinMode(init_button_pin,INPUT);
  pinMode(next_button_pin,INPUT);
  pinMode(next_button_pin,INPUT);
  
  
  /*7 Segment Led Display*/
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(E, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(G, OUTPUT);
  
  Serial.begin(9600);
  seg_print(channel);
}


void loop(){
  unsigned long time = millis();  //current time


  knob_value=analogRead(knob_Pin)/8;;   //reads the gain value and adjusts it in the range of 0-127 which is the range of the midi message values

  if (knob_value >= prev_knob_value+2 || knob_value <= prev_knob_value-2){  //used to avoid over-sending midi messages when the pontetiometer is going up/down by one value by itself
    Send_Midi_Message(176+channel-1,KNOB_CC,knob_value);
    prev_knob_value=knob_value;
  }  

  
  /*If init button is pressed and is not already pressed*/
  if (digitalRead(init_button_pin)==HIGH && init_button_state==false){
    channel=1; 
    init_button_state=true;   //button is now pressed
    seg_print(channel);       //display the new current channel
            
  } 
  else if(digitalRead(init_button_pin)==LOW && init_button_state==true){
    init_button_state=false;  //button is now released 
  }
  
  /*If next button is pressed and is not already pressed*/
  if (digitalRead(next_button_pin)==HIGH && next_button_state==false && time-next_buttom_time>200){
    if(channel<MAX_CHANNEL)channel++;  //next midi channel
    else channel=1;
    next_button_state=true;
    seg_print(channel);       //display the new current channel
    next_buttom_time=time;
  } 
  else if(digitalRead(next_button_pin)==LOW && next_button_state==true){
    next_button_state=false;
  }
    
  /*If prev button is pressed and is not already pressed*/
  if (digitalRead(prev_button_pin)==HIGH && prev_button_state==false && time-prev_buttom_time>200){
    if(channel>1)channel--;  //previous midi channel
    else channel=9;
    prev_button_state=true;
    seg_print(channel);       //display the new current channel
    prev_buttom_time=time;
  } 
  
  else if(digitalRead(prev_button_pin)==LOW && prev_button_state==true){
    prev_button_state=false;
  }
  
  Read_Midi_Message();    //Read incoming midi message
} 

/*************************************************************/
/*                      MIDI Messages                        */
/*************************************************************/
void Send_Midi_Message(byte first_byte, byte second_byte, byte third_byte)
{
  Serial.write(first_byte);
  Serial.write(second_byte);
  Serial.write(third_byte);
}


void Read_Midi_Message(){
  if (Serial.available() > 2){
    firstByte=Serial.read();   //read first byte
    secondByte=Serial.read();      //read second byte
    thirdByte=Serial.read();  //read third byte
     
       
    firstByte=firstByte+channel-1;  /*"translates" the incoming command to the same command but in the desired channel*/
    Send_Midi_Message(firstByte,secondByte,thirdByte);
     
    if(firstByte>=175 && firstByte <=191 && secondByte==64 && thirdByte ==0){  /*If "sustatin pedal off" message is received,turn it off sustain in all channels*/
      for(int i=1;i<=MAX_CHANNEL;i++){
         Send_Midi_Message(175+i,secondByte,thirdByte);
      }
    }
      
    if(firstByte>=144 && firstByte<160){   /*If "note on message" is received,make the corresponding value of the active_notes array true*/ 
      active_notes[channel][secondByte]=true;
    }
    else if(firstByte>=128 && firstByte<144 ){ /*If "note off message" is received,make the corresponding value of the active_notes array false and stop the same note on other channels*/ 
      for(int i=1;i<MAX_CHANNEL;i++){
        if(active_notes[i][secondByte]==1){
          Send_Midi_Message(128+i-1,secondByte,0);
          active_notes[i][secondByte]=0;
        }
      }
    }
      
  }
}


/*************************************************************/
/*                    7-Segment Display                      */
/*************************************************************/

/*Used to display digits on the 7 segment display*/
void seg_print(int number) {
  clean_seg();  //all leds are now off
  
  if(number==1){
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    
  }
  else if(number==2){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(G, HIGH);
  }
  else if(number==3){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
  }
  else if(number==4){
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
  }
  else if(number==5){
    digitalWrite(A, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
  } 
  else if(number==6){
    digitalWrite(A, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
  } 
  else if(number==7){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
  } 
  else if(number==8){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(E, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  } 
  else if(number==9){
    digitalWrite(A, HIGH);
    digitalWrite(B, HIGH);
    digitalWrite(C, HIGH);
    digitalWrite(D, HIGH);
    digitalWrite(F, HIGH);
    digitalWrite(G, HIGH);
  } 
}

/*Used to clear the 7 segment display*/
void clean_seg(){
  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
  digitalWrite(E, LOW);
  digitalWrite(F, LOW);
  digitalWrite(G, LOW);   
}  
