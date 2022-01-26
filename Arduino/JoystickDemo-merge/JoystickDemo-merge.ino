#include <Joystick.h>



/*
 * 
 * XPLDirect Demo
 * 
 * 
 */

#include <Joystick.h>

// Create Joystick
Joystick_ Joystick; 
int throttle1_ = 0;
int throttle2_ = 0;

int prop1_ = 0;
int prop2_ = 0;


// Rotary Encoder Module connections
const int PinDT=4;    // DATA signal
const int PinCLK=2;    // CLOCK signal

const int PinDT2=6;    // DATA signal
const int PinCLK2=5;    // CLOCK signal


const int PinDT3=8;    // DATA signal
const int PinCLK3=7;    // CLOCK signal

const int Button1=0;    // Up Button
const int Button2=1;    // Down Button
const int Button3=2;    // Up Button
const int Button4=3;    // Down Button
const int Button5=4;    // Down Button
const int Button6=5;    // Down Button

 int counter1 = 0; 
 int pinCLKState1;
 int pinCLKPreviousState1; 

 
 int counter2 = 0; 
 int pinCLKState2;
 int pinCLKPreviousState2;

   int counter3 = 0; 
 int pinCLKState3;
 int pinCLKPreviousState3;

 int counter = 0; 



void setup() 
{
  Joystick.setXAxisRange(40, 981);
  Joystick.setYAxisRange(40, 981);

  Joystick.setRudderRange(40, 981);
  Joystick.setThrottleRange(40, 981);

    Serial.begin(9600);


  pinMode(LED_BUILTIN, OUTPUT);     // built in LED on arduino board for debug and demonstration purposes
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  
   pinMode (PinCLK,INPUT);
   pinMode (PinDT,INPUT);

     
   pinMode (PinCLK2,INPUT);
   pinMode (PinDT2,INPUT);


  Joystick.begin(true);

  pinCLKPreviousState1 = digitalRead(PinCLK);  
  pinCLKPreviousState2 = digitalRead(PinCLK2); 
  pinCLKPreviousState3 = digitalRead(PinCLK3);   
  
 

   

 
  ErrorBlink(LED_BUILTIN, 5);       // let everyone know we are awake
   
 //Initialize serial and wait for port to open:
  while (!Serial)
  {
    ErrorBlink(LED_BUILTIN, 2);
    delay(300);
  }

  digitalWrite(LED_BUILTIN, LOW);
}



void loop() 
{
  throttle1_ = analogRead(A2);
  throttle1_ = map(throttle1_,0,1023, 45,980);
  throttle2_ = analogRead(A3);
  throttle2_ = map(throttle2_,0,1023, 45,980);
  prop1_ = analogRead(A0);
  prop1_ = map(prop1_,0,1023, 45,980);
  prop2_ = analogRead(A1);
  prop2_ = map(prop2_,0,1023, 45,980);
  Serial.println("Prop1a:"); Serial.println(prop1_);
  Serial.println("Prop2a:"); Serial.println(prop2_);
  Serial.println("Throttle1:"); Serial.println(throttle1_);
  Serial.println("Throttle2:"); Serial.println(throttle2_);

  Joystick.setXAxis(prop1_);
  Joystick.setYAxis(prop2_);
  Joystick.setThrottle(throttle1_);
  Joystick.setRudder(throttle2_);

    pinCLKState1 = digitalRead(PinCLK); // Reads the "current" state of the PinCLK
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (pinCLKState1 != pinCLKPreviousState1){     
     // If the PinDT state is different to the PinCLK state, that means the encoder is rotating clockwise
     if (digitalRead(PinDT) != pinCLKState1) { 
       counter ++;
       tapButtonUp();
     } else {
       counter --;
       tapButtonDown();

     }
    
   } 
   pinCLKPreviousState1 = pinCLKState1; // Updates the previous state of the PinCLK with the current state



   pinCLKState2 = digitalRead(PinCLK2); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (pinCLKState2 != pinCLKPreviousState2){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(PinDT2) != pinCLKState2) { 
       counter2 ++;
       tapButtonUp2();
     } else {
       counter2 --;
       tapButtonDown2();

     }
    
   } 
   pinCLKPreviousState2 = pinCLKState2; // Updates the previous state of the outputA with the current state

    pinCLKState3 = digitalRead(PinCLK3); // Reads the "current" state of the PinCLK3
   // If the previous and the current state of the PinCLK3 are different, that means a Pulse has occured
   if (pinCLKState3 != pinCLKPreviousState3){     
     // If the PinDT3 state is different to the PinCLK3 state, that means the encoder is rotating clockwise
     if (digitalRead(PinDT3) != pinCLKState3) { 
       counter3 ++;
       tapButtonUp3();
     } else {
       counter3 --;
       tapButtonDown3();

     }
    
   } 
   pinCLKPreviousState3 = pinCLKState3; // Updates the previous state of the outputA with the current state

}



 // Check if Rotary Encoder was moved
void tapButtonUp() {
  Joystick.setButton(Button1, 1);
  Joystick.setButton(Button2, 0);
  delay(150);
  Joystick.setButton(Button1, 0);
  Joystick.setButton(Button2, 0);
}

void tapButtonDown() {
  Joystick.setButton(Button1, 0);
  Joystick.setButton(Button2, 1);
  delay(150);  
  Joystick.setButton(Button1, 0);
  Joystick.setButton(Button2, 0);
}

 // Check if Rotary Encoder was moved
void tapButtonUp2() {
  Joystick.setButton(Button3, 1);
  Joystick.setButton(Button4, 0);
  delay(50);
  Joystick.setButton(Button3, 0);
  Joystick.setButton(Button4, 0);
}

void tapButtonDown2() {
  Joystick.setButton(Button3, 0);
  Joystick.setButton(Button4, 1);
  delay(50);  
  Joystick.setButton(Button3, 0);
  Joystick.setButton(Button4, 0);
}

 // Check if Rotary Encoder was moved
void tapButtonUp3() {
  Joystick.setButton(Button5, 1);
  Joystick.setButton(Button6, 0);
  delay(50);
  Joystick.setButton(Button5, 0);
  Joystick.setButton(Button6, 0);
}

void tapButtonDown3() {
  Joystick.setButton(Button5, 0);
  Joystick.setButton(Button6, 1);
  delay(50);  
  Joystick.setButton(Button5, 0);
  Joystick.setButton(Button6, 0);
}

void ErrorBlink(int pin, int count)
{
  for (int i = 0; i< count; i++)
  { 
   digitalWrite(pin, HIGH);
   delay(200);
   digitalWrite(pin, LOW);
   delay(100);
  }
}
