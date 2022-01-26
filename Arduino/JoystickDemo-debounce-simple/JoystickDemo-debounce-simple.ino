#include <Joystick.h>
#include <Arduino.h>



// uncomment following for internal info reporting
#define _DEBUG_ENCODER_

class KY040b
{
    int Button1=0;    // Up Button
    int Button2=1;    // Down Button  
    int buttonDelay=200;
    // pins
    int CLK;  // CK on encoder
    int DT;  // DT on encoder


  public:
    bool moved = false;
    uint8_t prevNextCode = 0;
    uint16_t store=0;

// Flag from interrupt routine (moved=true)
    Joystick_ * Joystick; 
    


    // Contructor provides defaults for pins
    KY040b(int clkpin=2, int dtpin=4, int buttonUp=0, int buttonDown=1, int buttonDly=200 )
    // default construction using pro-forma parameters
       : CLK(clkpin), DT(dtpin), Button1(buttonUp), Button2(buttonDown),buttonDelay(buttonDly)
    {
  

        Button1 = buttonUp;
        Button2 = buttonDown;
        

        pinMode (CLK,INPUT);
        pinMode (DT,INPUT);

        pinMode(CLK, INPUT_PULLUP);
        pinMode(DT, INPUT_PULLUP);

       
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
     delay(200);
    
  }

  void tapButtonDown() {
    Joystick->setButton(Button1, 0);
    Joystick->setButton(Button2, 1);
    #ifdef _DEBUG_ENCODER_
                Serial.print("Button Down: "); Serial.println(Button2);
        #endif
     delay(200);  

  }
    
  // A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (digitalRead(DT)) prevNextCode |= 0x02;
  if (digitalRead(CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;
      Serial.print("prevNextCode:"); Serial.println(prevNextCode);


   // If valid then store as 16 bit data.
   if  (rot_enc_table[prevNextCode] ) {
      store <<= 4;
      store |= prevNextCode;
      //if (store==0xd42b) return 1;
      //if (store==0xe817) return -1;
      if ((store&0xff)==0x2b) return -1;
      if ((store&0xff)==0x17) return 1;
   }
   return 0;
}

  void loop()
  {
    Serial.print("Encoder loop:"); Serial.println(CLK);
    static int8_t c,val;
    moved = 0;

    if( val=read_rotary() ) {
        c +=val;
        Serial.print(c);Serial.print(" ");

        if ( prevNextCode==0x0b) {
          Serial.print("eleven ");
          Serial.println(store,HEX);
          moved = 1;
          tapButtonUp();
        }

        if ( prevNextCode==0x07) {
          Serial.print("seven ");
          Serial.println(store,HEX);
          moved = 1;
          tapButtonDown();
        }
    } else {
              Serial.print("no val ");Serial.print(c);Serial.print(" ");Serial.println("0");

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
KY040b encoder (2, 4,0,1,200); 
KY040b encoder2 (5,6,2,3,50);
KY040b encoder3 (7,8,4,5,50);





void setup() 
{
  encoder.Joystick = &Joystick;
  encoder2.Joystick = &Joystick;
  encoder3.Joystick = &Joystick;


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



 
    encoder.loop();
    


    if (encoder.moved)
      encoder.nullButton();

      //Encoder 2

    encoder2.loop();

   
    if (encoder2.moved)
      encoder2.nullButton();

//Encoder 3
    encoder3.loop();

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
