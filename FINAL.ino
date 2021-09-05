//Final Program:
#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 4 //attach pin D3 Arduino to pin Trig of HC-SR04
#include <TFT.h>  // Incldue the TFT library
#include <SPI.h>  // Include the SPI comms library for the TFT screen

// Pin definition for Arduino Nano.
#define cs   10
#define dc   9
#define rst  8

// Motor A connections, as seen in the schematic.
int enA = A2;
int in1 = A0;
int in2 = A1;
// Motor B connections, as seen in the schematic.
int enB = A3;
int in3 = A5;
int in4 = A4;

long duration; int distance; //Terms for the SR04 usage.

const int BUTTON_PIN = 0; const int REDLED_PIN = 1; const int YELLOWLED_PIN = 2;  //State toggles
int RedledState = HIGH; int YellowledState = LOW; int lastButtonState; int currentButtonState;

TFT TFTscreen = TFT(cs, dc, rst); //Creation of a TFT object.
     
void setup() {                
  pinMode(BUTTON_PIN, INPUT_PULLUP); 
  pinMode(REDLED_PIN, OUTPUT); pinMode(YELLOWLED_PIN, OUTPUT);          
  currentButtonState = digitalRead(BUTTON_PIN);
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  
  TFTscreen.begin();
  TFTscreen.background(0, 0, 0);
  TFTscreen.setTextSize(2);

   // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT); pinMode(enB, OUTPUT); pinMode(in1, OUTPUT);  pinMode(in2, OUTPUT); pinMode(in3, OUTPUT); pinMode(in4, OUTPUT);
  // Turn off motors - Initial state
  digitalWrite(in1, LOW); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, LOW);  
}

void loop() {
  lastButtonState    = currentButtonState;      //Toggles the between the RED and the Yellow LED
  currentButtonState = digitalRead(BUTTON_PIN); 
  if(lastButtonState == HIGH && currentButtonState == LOW) {
    RedledState = !RedledState; YellowledState = !YellowledState; 
    digitalWrite(REDLED_PIN, RedledState); digitalWrite(YELLOWLED_PIN, YellowledState);
  } delay(100);
  if(YellowledState){   //Functions called based on LED state.
    activity();
  }
  if(RedledState){
    standby();
  }
  else{
      int redRandom = random(0, 255);         //TFT screen functioning.
      int greenRandom = random (0, 255);
      int blueRandom = random (0, 255);
      TFTscreen.background(0, 0, 0);
      TFTscreen.stroke(redRandom, greenRandom, blueRandom);  TFTscreen.text("Power", 6, 57);
  }
}

void activity(){
  
  digitalWrite(trigPin, LOW); //Send a 10 uS pulse and receive it, calculate the distance in centimeters.
  delay(1000);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; 
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm"); 

    //generate a random color
  int redRandom = random(0, 255);
  int greenRandom = random (0, 255);
  int blueRandom = random (0, 255);
  
  // set a random font color
  TFTscreen.stroke(redRandom, greenRandom, blueRandom);
  
  // Print distance in the middle of the screen
  TFTscreen.background(0, 0, 0);
  TFTscreen.text("Active", 6, 57);
  
  // wait 200 miliseconds until change to next color
  delay(200);

  //Linking motors with the SR04
  if(distance > 20){
    analogWrite(enA, 255); analogWrite(enB, 255);
    digitalWrite(in1, HIGH); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, HIGH);
  }
  else{
     analogWrite(enA, 0); analogWrite(enB, 0);
     digitalWrite(in1, LOW); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, LOW);
  }
}


void standby(){   //Standby mode
  int redRandom = random(0, 255);
  int greenRandom = random (0, 255);
  int blueRandom = random (0, 255);

  TFTscreen.stroke(redRandom, greenRandom, blueRandom);

  TFTscreen.background(0, 0, 0);
  TFTscreen.text("Standby", 6, 57);
  analogWrite(enA, 0); analogWrite(enB, 0);
  digitalWrite(in1, LOW); digitalWrite(in2, LOW); digitalWrite(in3, LOW); digitalWrite(in4, LOW);
}
