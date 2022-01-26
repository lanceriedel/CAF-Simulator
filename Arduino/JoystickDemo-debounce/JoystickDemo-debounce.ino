#include <Joystick.h>
#include <Arduino.h>



// uncomment following for internal info reporting
#define _DEBUG_ENCODER_

class KY040b
{
    int Button1=0;    // Up Button
    int Button2=1;    // Down Button  
    // pins
    int CLK;  // CK on encoder
    int DT;  // DT on encoder


  public:
    bool moved = false;
    int position = 0;
    // A turn counter for the rotary encoder (negative = anti-clockwise)
    int rotationCounter = 200;

// Flag from interrupt routine (moved=true)
    volatile bool rotaryEncoder = false;
    Joystick_ * Joystick; 
    


    // Contructor provides defaults for pins
    KY040b(int clkpin=2, int dtpin=4, int buttonUp=0, int buttonDown=1 )
    // default construction using pro-forma parameters
       : CLK(clkpin), DT(dtpin), Button1(buttonUp), Button2(buttonDown)
    {
  

        Button1 = buttonUp;
        Button2 = buttonDown;
        

        pinMode (CLK,INPUT);
        pinMode (DT,INPUT);

         #ifdef _DEBUG_ENCODER_
              Serial.print("Init Button1: "); Serial.println(Button1);
              Serial.print("Init Button2: "); Serial.println(Button2);
              Serial.print("Init DT: "); Serial.println(DT);
              Serial.print("Init DT: "); Serial.println(DT);


       #endif
    };

    ~KY040b()
    {

    };

    
  void nullButton() {
    Joystick->setButton(Button1, 0);
    Joystick->setButton(Button2, 0);
    #ifdef _DEBUG_ENCODER_
                Serial.print("Button Null: "); Serial.print(Button1);Serial.print(":");Serial.println(Button2);
        #endif
  }
  // Check if Rotary Encoder was moved
  void tapButtonUp() {
    Joystick->setButton(Button1, 1);
    Joystick->setButton(Button2, 0);
    #ifdef _DEBUG_ENCODER_
                Serial.print("Button Up: "); Serial.println(Button1);
        #endif
    // delay(200);
    
  }

  void tapButtonDown() {
    Joystick->setButton(Button1, 0);
    Joystick->setButton(Button2, 1);
    #ifdef _DEBUG_ENCODER_
                Serial.print("Button Down: "); Serial.println(Button2);
        #endif
    //delay(200);  

  }
    
    // Rotary encoder has moved (interrupt tells us) but what happened?
  // See https://www.pinteric.com/rotary.html
  int8_t checkRotaryEncoder()
  {
      // Reset the flag that brought us here (from ISR)
      rotaryEncoder = false;

      static uint8_t lrmem = 3;
      static int lrsum = 0;
      static int8_t TRANS[] = {0, -1, 1, 14, 1, 0, 14, -1, -1, 14, 0, 1, 14, 1, -1, 0};

      // Read BOTH pin states to deterimine validity of rotation (ie not just switch bounce)
      int8_t l = digitalRead(CLK);
      int8_t r = digitalRead(DT);

      Serial.print("Read clk: "); Serial.print(CLK);Serial.print(" : " ); Serial.println(l);
    Serial.print("Read dt: "); Serial.print(DT);Serial.print(" : " ); Serial.println(r);
      

      // Move previous value 2 bits to the left and add in our new values
      lrmem = ((lrmem & 0x03) << 2) + 2 * l + r;

      // Convert the bit pattern to a movement indicator (14 = impossible, ie switch bounce)
      lrsum += TRANS[lrmem];

      /* encoder not in the neutral (detent) state */
      if (lrsum % 4 != 0)
      {
          return 0;
      }

      /* encoder in the neutral state - clockwise rotation*/
      if (lrsum == 4)
      {
          lrsum = 0;
          return 1;
      }

      /* encoder in the neutral state - anti-clockwise rotation*/
      if (lrsum == -4)
      {
          lrsum = 0;
          return -1;
      }

      // An impossible rotation has been detected - ignore the movement
      lrsum = 0;
      return 0;
  }

  void loop()
{
  Serial.print("Encoder loop"); Serial.print(CLK);
    // Has rotary encoder moved?
    if (true)
    {
        // Get the movement (if valid)
        int8_t rotationValue = checkRotaryEncoder();
        Serial.print("rotation value:");System.println(rotationValue);

        // If valid movement, do something
        if (rotationValue != 0)
        {
            rotationCounter += rotationValue * 5;
            if (rotationValue<1) {
              tapButtonDown();
              moved = 1;
            }
            if (rotationValue>1) {
              tapButtonUp();
              moved = 1;
            }

            Serial.print(rotationValue < 1 ? "L" :  "R");
            Serial.println(rotationCounter);
        }
    }

    
}
    
}; // end KY040b
    

// Create Joystick
Joystick_ Joystick; 
int throttle1_ = 0;
int throttle2_ = 0;

int prop1_ = 0;
int prop2_ = 0;


//int clkpin=2, int dtpin=4, int deb=10, int buttonUp, int buttonDown
KY040b encoder (2, 4,0,1); 
KY040b encoder2 (5,6,2,3);
KY040b encoder3 (7,8,4,5);





void setup() 
{
  encoder.Joystick = &Joystick;
  Joystick.setXAxisRange(40, 981);
  Joystick.setYAxisRange(40, 981);

  Joystick.setRudderRange(40, 981);
  Joystick.setThrottleRange(40, 981);

    Serial.begin(2000000);


  pinMode(LED_BUILTIN, OUTPUT);     // built in LED on arduino board for debug and demonstration purposes
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);


  Joystick.begin(true);
   

 
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
  Serial.println("Main loop");
  throttle1_ = analogRead(A2);
  throttle1_ = map(throttle1_,0,1023, 45,980);
  throttle2_ = analogRead(A3);
  throttle2_ = map(throttle2_,0,1023, 45,980);
  prop1_ = analogRead(A0);
  prop1_ = map(prop1_,0,1023, 45,980);
  prop2_ = analogRead(A1);
  prop2_ = map(prop2_,0,1023, 45,980);
 
  Joystick.setXAxis(prop1_);
  Joystick.setYAxis(prop2_);
  Joystick.setThrottle(throttle1_);
  Joystick.setRudder(throttle2_);


 static int position = 0;
  static int position2 = 0;
 static int position3 = 0;

 
    encoder.loop();
    
    int pos = encoder.position;
    if (pos != position)
    {
        position = pos;
        Serial.print("Encoder Position1: "); Serial.println(position);
        
    }

    if (encoder.moved)
      encoder.nullButton();

      //Encoder 2

    encoder2.loop();


    int pos2 = encoder2.position;
    if (pos2 != position2)
    {
        position2 = pos2;
        Serial.print("Encoder Position2: "); Serial.println(position2);
        
    }
   
    if (encoder2.moved)
      encoder2.nullButton();

//Encoder 3
    encoder3.loop();

    int pos3 = encoder3.position;
    if (pos3 != position3)
    {
        position3 = pos3;
        Serial.print("Encoder Position3: "); Serial.println(position3);
        
    }
   
    if (encoder3.moved)
      encoder3.nullButton();

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
