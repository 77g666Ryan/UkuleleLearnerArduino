#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int s_pin_00 = A0;
int s_pin_01 = A1;
int s_pin_02 = A2;

int sensor_value_00 = 0;  // value read from the pot
int sensor_value_01 = 0;  // value read from the pot
int sensor_value_02 = 0; 
bool has_start = false; // value whether a song begins

void setup() {
  // put your setup code here, to run once:
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.backlight ();
  lcd.setCursor (1,0);
  lcd.print ("Hello Musician!"); // LED print Hello Musician!
  lcd.setCursor (0,1);
  lcd.print ("Shall we start?"); 
  Serial.begin(9600); // initialize serial communications at 9600 bps:
  pinMode(s_pin_00,INPUT); // set the digital pin as input
  pinMode(s_pin_01,INPUT);
  pinMode(s_pin_02,INPUT);

}

void loop() {
  // put your main code here, to run repeatedly
  // sensor values range [0, 1023].   1023 = 2^10 - 1
  sensor_value_00 = analogRead(s_pin_00);  // A0
  sensor_value_01 = analogRead(s_pin_01);  // A1
  sensor_value_02 = analogRead(s_pin_02);  // A2

  Serial.println(sensor_value_00);
  detectChord();
  
}

// function of detecting chords.
String detectChord() {
  if(sensor_value_00 == 1023) {
    has_start = true;
    lcd.clear();
    lcd.setCursor (2,0);
    lcd.print ("Current Chord");
    lcd.setCursor (3,1);
    lcd.print("C Major!");
  } else if (sensor_value_01 == 1023) {
    has_start = true;
    lcd.clear();
    lcd.setCursor (2,0);
    lcd.print ("Current Chord");
    lcd.setCursor (3,1);
    if (sensor_value_02 == 1023) {
      lcd.print("F Major!");
    } else {
      lcd.print("A minor!");
    }
  } else {     // Nothing
    if (has_start == true) {
      lcd.clear();
      lcd.setCursor (1,0);
      lcd.print ("Current Chord:");
      lcd.setCursor (3,1);
      lcd.print ("No touch"); 
    }
    
  }
  delay(500);
}

