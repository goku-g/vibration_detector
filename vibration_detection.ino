#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define zpin A0
#define ypin A1
#define xpin A2

#define led 13
#define buzzer A3

double xsample = 0;
double ysample = 0;
double zsample = 0;

int RawMin = 0;
int RawMax = 1023;

double start;
int samples = 200;

// change with your need:
float limit = 0.06;

bool alt = false;
int buztime = 3000;

double xvalue, yvalue, zvalue;
double xchange, ychange, zchange;

long xScaled, yScaled, zScaled;
float xAccel, yAccel, zAccel;

void calibrate_now()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrating....");

  xsample = 0;
  ysample = 0;
  zsample = 0;
  
  for(int i=0; i<samples; i++)
  {
    xsample += analogRead(xpin);
    ysample += analogRead(ypin);
    zsample += analogRead(zpin);
  }

  xsample /= samples;
  ysample /= samples;
  zsample /= samples;

  // Convert raw values to 'milli-Gs"
  long xScale = map(xsample, RawMin, RawMax, -3000, 3000);
  long yScale = map(ysample, RawMin, RawMax, -3000, 3000);
  long zScale = map(zsample, RawMin, RawMax, -3000, 3000);

  // re-scale to fractional Gs
  xsample = xScale / 1000.0;
  ysample = yScale / 1000.0;
  zsample = zScale / 1000.0;
  
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrated !");
  delay(1000);
  lcd.clear();
}

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  // pinMode(buzzer, OUTPUT);

  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  
  //Serial.begin(9600);
  calibrate_now();

}

void loop() {
  // put your main code here, to run repeatedly:
  xvalue = analogRead(xpin);
  yvalue = analogRead(ypin);
  zvalue = analogRead(zpin);

  xScaled = map(xvalue, RawMin, RawMax, -3000, 3000);
  yScaled = map(yvalue, RawMin, RawMax, -3000, 3000);
  zScaled = map(zvalue, RawMin, RawMax, -3000, 3000);

  // re-scale to fractional Gs
  xAccel = xScaled / 1000.0;
  yAccel = yScaled / 1000.0;
  zAccel = zScaled / 1000.0;


  xchange = xsample-xAccel;
  ychange = ysample-yAccel;
  zchange = zsample-zAccel;
  
  // Vibration detection algorithm:
  if(abs(xchange) > limit || abs(ychange) > limit || abs(zchange) > limit)
  {
    if(!alt)
    start = millis();

    alt = true;
  }
  else if(alt)
  {
    lcd.setCursor(9,1);
    lcd.print("ALERT!!!");
    
    // Buz upto 5 second in earthquake:
    if(millis() >= start+buztime)
    {
      calibrate_now();
      alt = false;
      lcd.setCursor(9,1);
      lcd.print("           ");
    }
  }

  // LED and buzzer code:
  digitalWrite(led, alt);
  
  if(alt)
  {
    analogWrite(buzzer, 130);
  }
  else
  {
    analogWrite(buzzer, 0);
  }
  
  lcd.setCursor(0,0);
  lcd.print("X:");
  lcd.print(xchange, 2);
  lcd.setCursor(9,0);
  lcd.print("Y:");
  lcd.print(ychange, 2);
  lcd.setCursor(0,1);
  lcd.print("Z:");
  lcd.print(zchange, 2);

//  lcd.setCursor(0,0);
//  lcd.print("X: ");
//  lcd.print(int(xvalue));
//  lcd.setCursor(9,0);
//  lcd.print("Y: ");
//  lcd.print(int(yvalue));
//  lcd.setCursor(0,1);
//  lcd.print("Z: ");
//  lcd.print(int(zvalue));
}
