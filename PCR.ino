#include <Adafruit_GFX.h>     // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_PCD8544.h>  // https://github.com/adafruit/Adafruit-PCD8544-Nokia-5110-LCD-library
#include "Adafruit_MAX31855.h" // https://github.com/adafruit/Adafruit-MAX31855-library

#define Relay1 8
#define Relay2 12
#define thermoDO 7
#define thermoCS 10
#define thermoCLK 11
#define lid 3 //pin for heated lid
#define buzzer A2
#define thermistor A1


//pid control (proportional and integral)
#define Kp 2.0
#define Ki 0.07

#define startPin A4 
#define stopPin 13

#define alarmThreshold 100

#define holdThresholdUpper 0
#define holdThresholdLower 1

#define NUMTEMPS 20 //number of temperatures in the lookup table
short temptable[NUMTEMPS][2] = {
  {1, 916},
  {54, 265},
  {107, 216},
  {160, 189},
  {213, 171},
  {266, 157},
  {319, 146},
  {372, 136},
  {425, 127},
  {478, 118},
  {531, 110},
  {584, 103},
  {637, 95},
  {690, 88},
  {743, 80},
  {796, 71},
  {849, 62},
  {902, 50},
  {955, 34},
  {1008, 2},
};
//temperature table for thermistor


/*PCR state defined as: 
 0 = standby
 1 = denature
 2 = anneal
 3 = extend
 4 = hold
 */

int pcrState = 0;  

int cycleCount = 0;

//code for buttons, debouncing
int startState; //the state of the start button
int stopState;
int lastStartState = LOW; //previous button state
int lastStopState = LOW;

long lastDebounceTime = 0; 
long debounceDelay = 50;  

//variable that stores run time
int startTime = 0;

//variables that keep track of hold time
long holdStart = 0;
long holdMillis = 0;
long holdInterval = 50;
double holdTemp = 0;


//stores last PCR state
int intpcrState = 0;

// pin 9- Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin A0 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(9, 6, 5, 4, A0);


Adafruit_MAX31855 thermocouple(thermoCLK, thermoCS, thermoDO);

//heated lid code
double lidSetpoint = 350; //optimal reading for the heated lid thermistor
double integral = 0; //pid control for heated lid
int lidPower = 0;
double lidError = 0;

//code for temperature slope
long interval = 1000; //interval at which temperature is measures
long previousMillis = 0;
double previousTemp = 0;
double deltaTemp[3]={
  0,0,0}; //average of last three seconds
//to be changed to amount of time needed for temperature to change one degree

//temperature of the thermal block, measured by thermocouple
double blockTemp()
{
  return (thermocouple.readCelsius()); 
}

void setup()   
{
  Serial.begin(9600); 
  pinMode(startPin, INPUT);
  pinMode(stopPin, INPUT);

  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(3,OUTPUT); //pin for encoder
  pinMode(buzzer, INPUT);

  // Serial.println("MAX31855 test");
  // wait for MAX chip to stabilize
  delay(500);

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(50);

}

void loop() 
{
  alarm();
  if (stopbutton()) 
  {
    off();
    cycleCount = 0;
    startTime = 0;
  }
  screen();
  //debugLid();
  //debugHold();
  switch (pcrState)
  {
  case 0:
    if (startbutton()) 
    {
      startTime = millis();
    }
    break;
  case 1:
    heat();
    if (blockTemp() >= 95) 
    {
      pcrState = 2;
      hold(2);
    }
    break;
  case 2:
    cool();
    if (blockTemp() <= 55)
    {
      pcrState = 3;
      hold(3);
    }
    break;
  case 3:
  heat();
    if (blockTemp() == 75)
    {
      heat();
      cycleCount++;
      if (cycleCount == 30) pcrState = 5;
      else 
      {
        hold(1);
      }
    }
    break;
  case 4:
    {
      if(holdTemp - blockTemp() >= holdThresholdLower) 
      {
        heat();
      }
      if(blockTemp() - holdTemp >= holdThresholdUpper) 
      {
        off();
      }
      
//      if (millis() - holdStart < holdInterval)
//      {
//        heat();
//        if (blockTemp() >= holdTemp)
//        {
//          off;
//        }
//      }
      if ((millis() - holdStart) > 30000) pcrState = intpcrState;
    }
    break;
  case 5:
    if (cycleCount == 30)
    {

      off();
      pcrState = 0;
    }
    break;
    //install counter and turn relays off until it expires
  }    

  Serial.println(blockTemp());
  //pid();

  //heat();
  /*while(blockTemp() < 95)
   {
   Serial.println(blockTemp());
   screen();
   display.display();
   delay(100);
   }
   cool();
   while(blockTemp() > 55)
   {
   Serial.println(blockTemp());
   screen();
   display.display();
   delay(100);
   }
   heat();
   while(blockTemp() < 70)
   {
   Serial.println(blockTemp());
   screen();
   display.display();
   delay(100);
   }
   */
  //off();
  //   delay(1000);
  //display.print("P value = ");

  display.display();
}
