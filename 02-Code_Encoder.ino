#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

// Bit math macros 
#define TOG_BIT(VAR,BIT)  VAR ^=  (1 << (BIT))
#define GET_BIT(VAR,BIT)  ((VAR >> BIT) & 1  )
#define SET_BIT(VAR,BIT)  VAR |=  (1 << (BIT))
#define CLR_BIT(VAR,BIT)  VAR &= ~(1 << (BIT))

// define lcd 16 x 2pins 
#define d4 A4
#define d5 A3
#define d6 A2
#define d7 A1
#define en 6 
#define rs 7

// Switches 
#define  Hold   10
#define  D_M    11

// Analog read from pot
//#define  A0     8 

// Bluetooth Module
#define TxD 8
#define RxD 9

#define PinA  4
#define PinB  3

#define degree_symbol "\337"

//float adcVal,angle;
float degree,minute;
char Flag = 3;

int aVal;
int PinALast;
int EncoderPosCount = 0;

float angle;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(RxD, TxD); 

void FLAG_VAL(void);
float mapf(float x, float in_min, float in_max, float out_min, float out_max);


void setup() {

  //pinMode(A0,INPUT);

  pinMode (PinA,INPUT);
  pinMode (PinB,INPUT);
  
  pinMode(Hold,INPUT);
  pinMode(D_M,INPUT);
  
  // Initialization
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  
  PinALast = digitalRead(PinA); // Read Pin A Initial State
  attachInterrupt(digitalPinToInterrupt(PinB), update, CHANGE);

}

void loop() {
  angle = mapf(EncoderPosCount,0.00,30.00,0.00,360.00); // mapping float values from analog to pot values
 // angle = (float)EncoderPosCount;
  FLAG_VAL(); // call function to check flags 

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
    mySerial.print(degree);
    mySerial.print("°");
    mySerial.print(minute);
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
    FLAG_VAL(); // Keep checking the change in flags 
    if(GET_BIT(Flag,2))
    {
      Serial.print('H');
      lcd.print('H');
    }
    CLR_BIT(Flag,2);
    delay(100);

    };

 // delay(500);
  lcd.clear();
  lcd.setCursor(0,0);
}


void FLAG_VAL(void)
{
  if(!digitalRead(D_M))
  {
    TOG_BIT(Flag,0);
    delay(300);
  }
  if(!digitalRead(Hold))
  {
    TOG_BIT(Flag,1);
    SET_BIT(Flag,2);
    delay(300);
  }
}

// Function for mapping 
float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void update ()
{
    aVal = digitalRead(PinA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured

    if(aVal != PinALast) // if Pin A value doesn't equal last pos 
  // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
  {
    if(digitalRead(PinB) != aVal) // Rotating CW
    {
      EncoderPosCount ++;
    }
    else
    {
      EncoderPosCount --;
    }
  }
    PinALast = aVal;
}
