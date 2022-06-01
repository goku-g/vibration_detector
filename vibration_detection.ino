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

double start;
int samples = 200;
int limit = 20;
bool alt = false;
int buztime = 3000;

double xvalue, yvalue, zvalue;
double xchange, ychange, zchange;


void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  // pinMode(buzzer, OUTPUT);

  lcd.init();                      // initialize the lcd 
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  
  //Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("Calibrating....");

  for(int i=0; i<samples; i++)
  {
    xsample += analogRead(xpin);
    ysample += analogRead(ypin);
    zsample += analogRead(zpin);
  }

  xsample /= samples;
  ysample /= samples;
  zsample /= samples;
  
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Calibrated !");
  delay(1000);
  lcd.clear();

}

void loop() {
  // put your main code here, to run repeatedly:
  xvalue = analogRead(xpin);
  yvalue = analogRead(ypin);
  zvalue = analogRead(zpin);

  xchange = xsample-xvalue;
  ychange = ysample-yvalue;
  zchange = zsample-zvalue;

  // Vibration detection algorithm:
  if(abs(xchange) > limit || abs(ychange) > limit || abs(zchange) > limit)
  {
    if(!alt)
    start = millis();

    alt = true;
  }
  else if(alt)
  {
    lcd.setCursor(7,1);
    lcd.print("ALERT!!!");
    
    // Buz upto 5 second in earthquake:
    if(millis() >= start+buztime)
    {
      alt = false;
      lcd.setCursor(7,1);
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
  lcd.print("X: ");
  lcd.print(int(xchange));
  lcd.setCursor(9,0);
  lcd.print("Y: ");
  lcd.print(int(ychange));
  lcd.setCursor(0,1);
  lcd.print("Z: ");
  lcd.print(int(zchange));

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
