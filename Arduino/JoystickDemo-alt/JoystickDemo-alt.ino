#include <Joystick.h>

#include <ezButton.h>

// uncomment following for internal info reporting
#define _DEBUG_ENCODER_

class KY040b
{
    const int Button1=0;    // Up Button
    const int Button2=1;    // Down Button  
    // pins
    int CLK;  // CK on encoder
    int DT;  // DT on encoder
    int PUSH; // push shaft on encoder (marked 'SW' on unit)
    // debounce time for all pins
    int debounce; // ms debounce time

  public:
    bool enc_pressed = false;
    bool moved = false;
    int position = 0;
    Joystick_ * Joystick; 
    
    // I exposed these for easy external access, rather than using delegates
    ezButton * buttonClk;
    ezButton * buttonDt;
    ezButton * buttonPush;

    // Contructor provides defaults for pins and debounce time - I am using a MEGA2560
    KY040b(int clkpin=2, int dtpin=4, int buttonpin=3, int deb=5 )
    // default construction using pro-forma parameters
       : CLK(clkpin), DT(dtpin), PUSH(buttonpin), debounce(deb)
    {
        buttonClk = new ezButton(CLK);
        buttonDt = new ezButton(DT);
        buttonPush = new ezButton(PUSH);

        buttonClk->setDebounceTime(debounce);
        buttonDt->setDebounceTime(debounce);
        buttonPush->setDebounceTime(debounce);

        pinMode (CLK,INPUT);
        pinMode (DT,INPUT);
        pinMode (PUSH, INPUT); 
    };

    ~KY040b()
    {
      delete buttonClk;
      delete buttonDt;
      delete buttonPush;
    };

    
void nullButton() {
  Joystick->setButton(Button1, 0);
  Joystick->setButton(Button2, 0);
}
 // Check if Rotary Encoder was moved
void tapButtonUp() {
  Joystick->setButton(Button1, 1);
  Joystick->setButton(Button2, 0);
  delay(200);
  
}

void tapButtonDown() {
  Joystick->setButton(Button1, 0);
  Joystick->setButton(Button2, 1);
  delay(200);  

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
      buttonPush->loop();

      enc_pressed = buttonPush->isPressed(); // test shaft 'pushed'
      #ifdef _DEBUG_ENCODER_
          if (enc_pressed)
              Serial.println("pressed");
      #endif
    
      value = buttonClk->getState();
       #ifdef _DEBUG_ENCODER_
              Serial.print("value: "); Serial.println(value);
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



const int Button1=0;    // Up Button
const int Button2=1;    // Down Button

KY040b encoder; // defaults to: 32,34,36,25




void setup() 
{
  encoder.Joystick = &Joystick;
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


 static int position = 0;
    encoder.loop();
    int pos = encoder.position;
    if (pos != position)
    {
        position = pos;
        Serial.print("Encoder Position: "); Serial.println(position);
        
    }
    bool pressed = encoder.enc_pressed;
    if (pressed)
        Serial.println("pressed");
    if (encoder.moved)
      encoder.nullButton();

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
