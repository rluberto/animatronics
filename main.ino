#include <Adafruit_TiCoServo.h>
#include <known_16bit_timers.h>


#include <Adafruit_NeoPixel.h>


//#include <Servo.h> //Include the servo library

#include <Stepper.h> //Include the stepper library

#include "Arduino.h" //Include all libraries for the DF Player Mini
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"



//Begin setup of the mp3 player
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);

//Attach objects to pins

const int throttleTrigger = A0; //Attach the throttle input to a pin
const int solTurtle = 40; //Attach the turtles solenoid to a pin
const int motWater = 41;

int idleAudioTrigger = 0; //Create a value to prevent a trigger loop of the idle audio track

int turtlePixelsShowTrigger = 0; //Create a variable to monitor and keep track of the state and prevent looping of the idle lights update
int barPixelsShowTrigger = 0;

Adafruit_TiCoServo alligatorMouthBottom; //Create the servo objects
Adafruit_TiCoServo alligatorMouthTop;
Adafruit_TiCoServo alligatorTail;
Adafruit_TiCoServo vineLeft;
Adafruit_TiCoServo vineRight;

const int stepsPerRevolution = 200; //How many steps the stepper can move for a rev

Stepper boatMotor(stepsPerRevolution, 50, 51, 52, 53); //Configure the stepper motor and attach it to the correct pins

Adafruit_NeoPixel turtlePixels(6, 48, NEO_GRB + NEO_KHZ800); //Create the pixel objects
Adafruit_NeoPixel barPixels(26, 49, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel alligatorPixels(6, 47, NEO_GRB + NEO_KHZ800);

void setup() {

  Serial.begin(115200); //Start the serial monitor for troubleshooting

  Serial.println("Animatronic National Park Ver. 1.0");
  Serial.println("Please Wait. Loading...");

  mySoftwareSerial.begin(9600);  //Open the serial port for the audio player communication

  while (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    mySoftwareSerial.begin(9600);
    delay(500);
  }

  pinMode(throttleTrigger, INPUT_PULLUP); //Set the throttle up as an input

  pinMode(solTurtle, OUTPUT); //Set the turtles air solenoid as an output
  pinMode(motWater, OUTPUT);

  alligatorMouthBottom.attach(22); //Associate each servo with a pin
  alligatorMouthTop.attach(23);
  alligatorTail.attach(24);
  vineLeft.attach(25);
  vineRight.attach(26);

  boatMotor.setSpeed(60); //Set the speed of the stepper motor

  turtlePixels.begin(); //Start the pixel strips
  barPixels.begin();
  alligatorPixels.begin();
  turtlePixels.clear(); //Clear the pixel strips
  barPixels.clear();
  alligatorPixels.clear();

  Serial.print("Ready: Moving to loop program.");

  delay(1000);
  
}

void loop() { 

  int triggerValue = analogRead(throttleTrigger);
  Serial.println(analogRead(throttleTrigger));

  delay(1000);
  
  if(triggerValue < 800){ //Checks for the trigger of the throttle
    mainAnimation(); //Call the main animation method when the project is triggered
  }
  else{
    if(turtlePixelsShowTrigger == 0){ //Set the turtle pixels
        setTurtlePixels(0, 0, 255, 255, 0);
      turtlePixelsShowTrigger = 1;
    }
    if(barPixelsShowTrigger == 0){ //Set the bar pixels
      setBarPixels(0, 26, 255, 255, 0);
      barPixelsShowTrigger = 1;
    }
    idleAnimation(); //Call the idle animation method
  }
  
}


/*//////////////////////////////////////////////
////             Idle Animation             ////     
//////////////////////////////////////////////*/

void idleAnimation(){ //Idle animation that runs when nothing is happening
  if(idleAudioTrigger == 0){ //Loop the idle audio if it hasn't already been triggered
    idleAudioTrigger = 1;
    //myDFPlayer.loop(1);
  }
  //Move all of the motors to their idle positions
  alligatorMouthTop.write(100);
  alligatorMouthBottom.write(80);
  alligatorTail.write(90);
  vineLeft.write(100); //100 - Normal Position 
  vineRight.write(0); //0 - Normal Position
  digitalWrite(motWater, LOW);
  return; //Return to the loop method
}




/*//////////////////////////////////////////////
////             Main Animation             ////     
//////////////////////////////////////////////*/

void mainAnimation(){ //Main animation that runs when trigger is moved

  //setBarPixels(0, 0, 0, 0, 0); //Turn all of the lights off
  setTurtlePixels(0, 0, 0, 0, 0);
  setAlligatorPixels(0, 0, 0, 0, 0);

  for(int i=255; i>0; i--){
    setTurtlePixels(0, 26, i, i, 0);
    setBarPixels(8, 26, i, i, 0);
    setBarPixels(0, 4, i, i, 0);
    setBarPixels(4, 8, 255, 255, 255-i);
    delay(20);
  }


    setBarPixels(0, 26, 0, 0, 0);
    setBarPixels(0, 4, 0, 0, 0);
    setBarPixels(4, 8, 255, 255, 255);

  delay(5000); //Wait 2 seconds
  
  myDFPlayer.play(2); //Play the main animation audio track

  setBarPixels(0, 0, 255, 0, 0);

  delay(1500);

  setTurtlePixels(0, 0, 0, 0, 255);

  delay(300);

  setAlligatorPixels(0, 0, 0, 0, 255);

  delay(1500);

  setBarPixels(0, 0, 0, 255, 255);

  delay(1500);

  setTurtlePixels(0, 0, 255, 0, 0);

  delay(300);

  setAlligatorPixels(0, 0, 255, 0, 0);

  delay(500);

  boatMotor.step(350);

  delay(1000);

  digitalWrite(solTurtle, HIGH);

  delay(100);

  digitalWrite(solTurtle, LOW);

  delay(2000);

  digitalWrite(solTurtle, HIGH);

  delay(5000);

  boatMotor.step(2500);

  delay(2000);

  alligatorMouthTop.write(80);
  alligatorMouthBottom.write(100);

  delay(100);

  boatMotor.step(-100);

  delay(300);

  alligatorMouthTop.write(100);
  alligatorMouthBottom.write(80);

  alligatorTail.write(100);

  delay(100);

  alligatorTail.write(80);

  delay(100);

  boatMotor.step(500);

  delay(1000);

  for(int x=0; x<5; x++){
    alligatorMouthTop.write(80);
    alligatorMouthBottom.write(100);
    delay(100);
    alligatorMouthTop.write(100);
    alligatorMouthBottom.write(80);
    delay(100);
  }

  idleAudioTrigger = 0; //Allow the idle audio to be triggered again
  turtlePixelsShowTrigger = 0;
  barPixelsShowTrigger = 0;

  boatMotor.step(-3250);

  setBarPixels(0, 0, 0, 0, 0); //Turn all of the lights off
  setTurtlePixels(0, 0, 0, 0, 0);
  setAlligatorPixels(0, 0, 0, 0, 0);

  delay(1000);
  
  return; //Return to the loop method  
}

void setTurtlePixels(int numB, int numE, int r, int g, int b){ //Set the pixels of the back light bar
  for(int x=numB; x<numE; x++){
    turtlePixels.setPixelColor(x, turtlePixels.Color(r, g, b));
  }
  if(numB == 0 && numE == 0){
    for(int x=0; x<6; x++){
      turtlePixels.setPixelColor(x, turtlePixels.Color(r, g, b));
    }
  }
  turtlePixels.show();
  return;
}

void setBarPixels(int numB, int numE, int r, int g, int b){ //Set the pixels of the front light bar
  for(int x=numB; x<numE; x++){
    barPixels.setPixelColor(x, barPixels.Color(r, g, b));
  }
  if(numB == 0 && numE == 0){
    for(int x=0; x<26; x++){
      barPixels.setPixelColor(x, barPixels.Color(r, g, b));
    }
  }
  barPixels.show();
  return;
}

void setAlligatorPixels(int numB, int numE, int r, int g, int b){ //Set the pixels of the front light bar
  for(int x=numB; x<numE; x++){
    alligatorPixels.setPixelColor(x, alligatorPixels.Color(r, g, b));
  }
  if(numB == 0 && numE == 0){
    for(int x=0; x<6; x++){
      alligatorPixels.setPixelColor(x, alligatorPixels.Color(r, g, b));
    }
  }
  alligatorPixels.show();
  return;
}

void setIdleLightsOn(){ //Set the lights on for the idle animation
  
}

void setIdleLightsOff(){ //Set the lights off for the idela animation
  
}


/*Program Notes
 * boatMotor.step(100); = move 1/2 inch
 * Do not step boatMotor more than 1800 in one direction
 */
