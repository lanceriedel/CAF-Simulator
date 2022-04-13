#include <Joystick.h>

#include <ezButton.h>

// uncomment following for internal info reporting
#define _DEBUG_ENCODER_

class KY040b
{
    int Button1=0;    // Up Button
    int Button2=1;    // Down Button  
    // pins
    int CLK;  // CK on encoder
    int DT;  // DT on encoder
    // debounce time for all pins
    int debounce; // ms debounce time

  public:
    bool moved = false;
    int position = 0;
    Joystick_ * Joystick; 
    
    // I exposed these for easy external access, rather than using delegates
    ezButton * buttonClk;
    ezButton * buttonDt;

    // Contructor provides defaults for pins and debounce time - I am using a MEGA2560
    KY040b(int clkpin=2, int dtpin=4, int deb=10, int buttonUp=0, int buttonDown=1 )
    // default construction using pro-forma parameters
       : CLK(clkpin), DT(dtpin), debounce(deb), Button1(buttonUp), Button2(buttonDown)
    {
        buttonClk = new ezButton(CLK);
        buttonDt = new ezButton(DT);

        buttonClk->setDebounceTime(debounce);
        buttonDt->setDebounceTime(debounce);

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
      delete buttonClk;
      delete buttonDt;
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
    
    void loop(void)
    {
      static uint8_t accum = 0; // Not really required - used this in tests
      moved = false;
    
      static int8_t count = 0; // each detent position change generates TWO events
      static int rotation = buttonClk->getState(); 
      int value, dt;

      buttonClk->loop(); // MUST call the loop() func for each pin
      buttonDt->loop(); //

      value = buttonClk->getState();
       #ifdef _DEBUG_ENCODER_
              Serial.print("value CLk State: ");Serial.print(value); Serial.print(" CLK: "); Serial.print(CLK); Serial.print(" DT: "); Serial.println (DT);;
       #endif
      if (value != rotation) // compare with previous reading
      { 
        dt = buttonDt->getState();
        if (dt != value) 
        {  
          // Clockwise
          if (count == 1) 
          {
            position++; // update the global
            tapButtonUp();
            moved = true;

            #ifdef _DEBUG_ENCODER_
                Serial.println ("clockwise");
            #endif
          }
        } 
        else 
        { 
          //Counterclockwise
          if (count == 1)
          {
            position--;
            tapButtonDown();
            moved = true;

            #ifdef _DEBUG_ENCODER_
                Serial.println("counterclockwise");
            #endif
          }
        }
        rotation = value; // update last reading
        
        // the following is for interest only, to see the coded sequences
        accum = accum << 1;
        accum |= value;
        accum = accum << 1;
        accum |= dt;
        
        count++; // required
        if (count == 2)
        {
           #ifdef _DEBUG_ENCODER_
              Serial.print("accum: "); Serial.println(accum, BIN);
              Serial.print("Encoder Position: "); Serial.println(position);
           #endif
           count = 0; // reset!
           accum = 0;
        }
      } 
    };
    
}; // end KY040b
    

// Create Joystick
Joystick_ Joystick; 
int throttle1_ = 0;
int throttle2_ = 0;

int prop1_ = 0;
int prop2_ = 0;


//int clkpin=2, int dtpin=4, int deb=10, int buttonUp, int buttonDown
KY040b encoder (2, 4, 11,0,1); 
KY040b encoder2 (5,6,15,2,3);
KY040b encoder3 (7,8,15,4,5);





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
