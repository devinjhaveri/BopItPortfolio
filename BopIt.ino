//Libraries
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

void buttonPush(int x, int s); 

//Constants
//Push button Pins
const int startButtonPin = A4;    
const int buttonPin[] = {A1, A5, A3}; //the bop it, pull it, twist it button pins

//RGB LED Pins
const int startLedPinRed =  6;      
const int startLedPinGreen =  7;      

//Normal LED Pins
const int ledPin[] = {8, 4, 5}; //the bop it, pull it, twist it LED's

// variables will change:
unsigned long previousTime = 0;
unsigned long interval = 0;
int startButtonState = 0;         
int buttonState = 0;
int startButtonToggle = 0;
int correct = 0;
int speed = 150;
int randNumber;



void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

 //DF Player mini setup (All serial prints can be removed for final product but keep now for testing purposes)
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(24);  //Set volume value. From 0 to 30
  
  //Pin Setup
  pinMode(startLedPinRed, OUTPUT);
  pinMode(startLedPinGreen, OUTPUT);
  digitalWrite(startLedPinRed, HIGH);
  pinMode(startButtonPin, INPUT);
  for (int i = 0; i<=2; i++){
    pinMode(ledPin[i], OUTPUT);
    pinMode(buttonPin[i], INPUT);
  }

  randomSeed(analogRead(0));
}



void loop() {
  // read the state of the pushbutton value:
  startButtonState = digitalRead(startButtonPin);
  
  //Checks to see if the start buttons has been pressed and if so starts the game
  if (startButtonState == HIGH) {
    if(startButtonToggle == 0){
      digitalWrite(startLedPinRed, LOW);
      digitalWrite(startLedPinGreen, HIGH);
      startButtonToggle = 1;
      speed = 150; //Initial speed of the game, will slowly ramp up throughout the game
    } 
    delay(500);
  }
  //The actual game loop
  if(startButtonToggle == 1){
    randNumber = random(0,3); 
    correct = 0;
    if(speed>50){ //This ensures that speed caps out to 50
      speed -= 5;
    }
    
    buttonPush(randNumber, speed); //main game function
    
    if(correct==0){ //Check to see if the game has ended
      //sound clip and Red LED to let you know you lost
      myDFPlayer.play(4); 
      digitalWrite(startLedPinGreen, LOW);
      digitalWrite(startLedPinRed, HIGH);
      startButtonToggle = 0;
    }
    delay(speed*10);
  }
}



void buttonPush(int x, int s){
      //play the corresponding sound clip and light the correct LED determined by the random generator
      myDFPlayer.play(x+1); 
      digitalWrite(ledPin[x],HIGH); 

      //Wait for user to input the correct button and cut the game off when the incorrect button is pressed
      for(int i = 0;i<s;i++){
        if((digitalRead(buttonPin[0])==HIGH)&&(buttonPin[0]!=buttonPin[x])){
          break;
        }else if ((digitalRead(buttonPin[1])==HIGH)&&(buttonPin[1]!=buttonPin[x])){
          break;
        }else if ((digitalRead(buttonPin[2])==HIGH)&&(buttonPin[2]!=buttonPin[x])){
          break;
        }else if (digitalRead(buttonPin[x])==HIGH) {
          correct = 1;
          break;
        }
        delay(10);
      }
      digitalWrite(ledPin[x],LOW); //Turn the LED off before returning
}
