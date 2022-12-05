#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#define TOG_BIT(VAR,BIT)  VAR ^=  (1 << (BIT))
#define GET_BIT(VAR,BIT)  ((VAR >> BIT) & 1  )
#define SET_BIT(VAR,BIT)  VAR |=  (1 << (BIT))
#define CLR_BIT(VAR,BIT)  VAR &= ~(1 << (BIT))

// define lcd 16 x 2pins 
#define d4 2
#define d5 3
#define d6 4
#define d7 5
#define en 6 
#define rs 7

// Switches 
#define  Hold   10
#define  D_M    11

//Analog read from pot
//#define  A0     8 

#define TxD 8
#define RxD 9

#define degree_symbol "\337"
#define degree_symbol2  (char)176

float degree,Minute;
char Flag = 3;
float angle;
const int MPU_addr=0x68;         //I2C MPU6050 Address
int16_t axis_X,axis_Y,axis_Z;    
int minVal=265;
int maxVal=402;
double x;
double y;
double z;
int pos = 0;  

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SoftwareSerial mySerial(RxD, TxD); 

void FLAG_VAL(void);
float mapf(float x, float in_min, float in_max, float out_min, float out_max);


void setup() {

  //pinMode(A0,INPUT);
  
  pinMode(Hold,INPUT);
  pinMode(D_M,INPUT);
  
  // Initialization
  Serial.begin(9600);
  mySerial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  Wire.begin();                        //Begins I2C communication
  Wire.beginTransmission(MPU_addr);    //Begins Transmission with MPU6050
  Wire.write(0x6B);                    //Puts MPU6050 in Sleep Mode
  Wire.write(0);                       //Puts MPU6050 in power mode 
  Wire.endTransmission(true);          //Ends Trasmission
  delay(1000);
}

void loop() {
 //adcVal = analogRead(A0);
 //angle = mapf(adcVal,0.001,1023.001,0.001,330.001); // mapping float values from analog to pot values 
  Wire.beginTransmission(MPU_addr); //Begins I2C transmission 
  Wire.write(0x3B);                 //Start with register 0x3B (ACCEL_XOUT_H)             
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr,14,true); //Request 14 Registers from MPU6050
  
  axis_X=Wire.read()<<8|Wire.read(); //Obtain 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L) 
  axis_Y=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_YOUT_H) & 0x3C (ACCEL_YOUT_L)
  axis_Z=Wire.read()<<8|Wire.read(); //0x3B (ACCEL_ZOUT_H) & 0x3C (ACCEL_ZOUT_L)
  
  int xAng = map(axis_X,minVal,maxVal,-90,90); 
  int yAng = map(axis_Y,minVal,maxVal,-90,90);
  int zAng = map(axis_Z,minVal,maxVal,-90,90);
  x = RAD_TO_DEG * (atan2(-yAng, -zAng)+PI);     //Formula to calculate x values in degree
  int pos = map(x,0,180,0,180); // As X value is from 0 to 360 deg
  angle = x;
  FLAG_VAL(); // call function to check flags 
    
    if(!GET_BIT(Flag,0)) // if D/M is pressed once 
    {
      degree = angle;
      Minute = (angle - (int)degree ) * 60.0;
      // Print values to LCD
      lcd.print((int)degree);
      lcd.print(degree_symbol);
      lcd.print((int)Minute);
      lcd.print("'");
      // Print values to serial monitor 
      Serial.println(" ");
      Serial.println("******* Value *******");
      Serial.print((int)degree);
      Serial.print("°");
      Serial.print((int)Minute);
      Serial.print("'");
      Serial.println(" ");
      Serial.println("*********************");
      // Print values to BTM ( Bluetooth Module )
      mySerial.println(" ");
      mySerial.println("******* Value *******");
      mySerial.print(degree);
      mySerial.print("°");
      mySerial.print(Minute);
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

  delay(500);
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

float mapf(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
