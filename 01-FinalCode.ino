#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Oversampling.h>

#define Adc             10    //res. of adc
#define oversampling    14   //no. of bits obtained through the oversampling process
#define Averaging       50  //no. of consecutive oversampled readings performed inside 'adc.read()' before returining the averaged value.

#if       oversampling    == 16
#define   Max   65472.0
#endif 
#if       oversampling    == 14
#define   Max   16368.0
#endif
#if     oversampling    == 10
#define   Max   1023.0
#endif

// Bit math Macros 
#define TOG_BIT(VAR,BIT)  VAR ^=  (1 << (BIT))
#define GET_BIT(VAR,BIT)  ((VAR >> BIT) & 1  )
#define SET_BIT(VAR,BIT)  VAR |=  (1 << (BIT))
#define CLR_BIT(VAR,BIT)  VAR &= ~(1 << (BIT))

// define lcd 16 x 2pins 
#define d4 7
#define d5 8
#define d6 9
#define d7 10
#define en 6
#define rs 5

// Switches 
#define  Hold   3
#define  D_M    2

//Analog read from pot
#define  A0     8 

//BTM SWSerial 
#define TxD 11
#define RxD 12

#define degree_symbol "\337"

// Vars
float adcVal,angle;
float degree,minute;
char Flag = 3;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(RxD, TxD); 
Oversampling  adc(Adc, oversampling, Averaging);


void PrintValues(void); // Print fun
void UpdateHold(void);
void UpdateD_M(void);
float mapf(float x, float in_min, float in_max, float out_min, float out_max); // float map fun


void setup() {
  
  pinMode(Hold,INPUT);
  pinMode(D_M,INPUT);
  
  // Initialization
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);

  attachInterrupt(digitalPinToInterrupt(Hold) , UpdateHold , FALLING );
  attachInterrupt(digitalPinToInterrupt(D_M ) ,UpdateD_M  , FALLING );
}

void loop() {
  adcVal = adc.read(0);
  angle = mapf(adcVal,0.0,Max,0.0,270.0); // mapping float values from analog to pot values 
  PrintValues();
}

void PrintValues(void)
{
      lcd.clear();
      lcd.setCursor(0,0);
      if(!GET_BIT(Flag,0)) // if D/M is pressed once 
    {
      degree = angle;
      minute = (angle - (int)degree ) * 60.0;
      // Print values to LCD
      lcd.print((int)degree);
      lcd.print(degree_symbol);
      lcd.print((int)minute);
      lcd.print("'");
      // Print values to serial monitor 
      Serial.println(" ");
      Serial.println("******* Value *******");
      Serial.print((int)degree);
      Serial.print("°");
      Serial.print((int)minute);
      Serial.print("'");
      Serial.println(" ");
      Serial.println("*********************");
      // Print values to BTM ( Bluetooth Module )
      mySerial.println(" ");
      mySerial.println("******* Value *******");
      mySerial.print((int)degree);
      mySerial.print("°");
      mySerial.print((int)minute);
      mySerial.print("'");
      mySerial.println(" ");
      mySerial.println("*********************");
      }
    else // (digitalRead(D_M) == 1) // if D/M pressed again
    {
      // Print values to LCD
      lcd.print(angle);
      lcd.print(degree_symbol);
      // Print values to Serial Monitor 
      Serial.println(" ");
      Serial.println("******* Value *******");
      Serial.print(angle);
      Serial.print("°");
      Serial.print(" ");
      Serial.println(" ");
      Serial.println("*********************");
      // Print values to BTM
      mySerial.println(" ");
      mySerial.println("******* Value *******");
      mySerial.print(angle);
      mySerial.print("°");
      mySerial.print(" ");
      mySerial.println(" ");
      mySerial.println("*********************");
    }
    
    while(!GET_BIT(Flag,1)) // If Hold is pressed 
    {
      if(GET_BIT(Flag,2))
      {
        Serial.print('H');
        lcd.print('H');
      }
      CLR_BIT(Flag,2);
      delay(100);
    };
}

void UpdateHold(void)
{
  TOG_BIT(Flag,1);
  SET_BIT(Flag,2);
}
void UpdateD_M(void)
{
  TOG_BIT(Flag,0);
}

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
