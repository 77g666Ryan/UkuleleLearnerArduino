/*
  Innovation Project: Ukulele UniChord
  Author by Best Bees Team.
  User Scenario:
    For Ukulele beginers, one of the most challenging part is the chords. 
    On Piano, you can clearly see notes and chords through keys. However on Ukulele, all you can see are strings. 
    If a system is able to tell what chords the user is playing, it will be very helpful.

  Tasks:
   - Research (Jay, Ryan, Aaron)
     We thought there were two ways to detect the notes and chords. 
     One way is to use the sound sensor. However, if there are background noise, than the sound sensor will pick up every sound and not be accurate.
     The other way is to use the pressure sensor. The good part is that it will be more reliable than the sound sensor because it is not affected by noise.
   - Design (Ryan and Anthony)
     1. Define the scope
     2. Draw the workflow graphs
     3. Wrote psudo code
   - Implementation (All the members)
     - Get chord of song - Authony Ouyang
     - Detect chords - Ryan Liu
     - Check chords - Austin Li
     - Wait a measure - Max Sun
  - Testing (Jay)
  - 
  - Demo to Experts (Max and Ryan)
*/

#include <Wire.h> // include header file of wire
#include <LiquidCrystal_I2C.h> // include header file of LiquidCrystal Arduino library for I2C LCD displays

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27(hexadecimal) for a 16 characters and 2 line display
int s_pin_00 = A0; // Input pot for touch sensor 1
int s_pin_01 = A1; // Input pot for touch sensor 2
int s_pin_02 = A2; // Input pot for touch sensor 3

int sensor_value_00 = 0;  // value read from the pot A0
int sensor_value_01 = 0;  // value read from the pot A1
int sensor_value_02 = 0; // value read from the pot A2
bool has_start = false; // value whether a song starts

int led = 5; // value of pin for led
int button = 4; // value of pin for button

int button_value; // value of button

void setup() {
  // initialize the lcd 
  lcd.init();  
  lcd.backlight();
  lcd.setCursor (1,0); // set the cursor at the first row, second letter
  lcd.print ("Hello Musician!"); // LCD print Hello Musician!
  lcd.setCursor (0,1); // set the cursor at the second row, first letter
  lcd.print ("Shall we start?"); // LCD print shall we start

  // initialize the serial
  Serial.begin(9600); // initialize serial communications at 9600 bps. It helps debugging

  // initialize the pressure sensor
  pinMode(s_pin_00,INPUT); // set the digital pin A0 as input
  pinMode(s_pin_01,INPUT); // set the digital pin A1 as input
  pinMode(s_pin_02,INPUT); // set the digital pin A2 as input

  // initialize the led
  pinMode(led, OUTPUT); // set pin for led as output

  // initialize the button
  pinMode (button, INPUT); // define the button interface as input
  button_value = digitalRead (button); // read the digital 4 level value and assign it to button_value
}

void loop() {
  // main logic of UniChord, to run repeatedly
  // sensor values range [0, 1023].   1023 = 2^10 - 1
  sensor_value_00 = analogRead(s_pin_00);  // read the value from A0
  sensor_value_01 = analogRead(s_pin_01);  // read the value from A1
  sensor_value_02 = analogRead(s_pin_02);  // read the value from A2

  Serial.println(sensor_value_01); // Serial catch the sensor value and draw the graph
  
  button_value = digitalRead (button); // read the digital 4 level value and assign it to button_value
  if(button_value == LOW) { // if press the button
    has_start = true; // start to detect chords
    digitalWrite(led, HIGH); // make led light 
  }
  
  if(has_start == true) { // if start to detect chords
    String detect_chord = detectChord(); // call detectChord() function to detect current chord
  }
  delay(500); // delay 0.5 second
  //practiceChords();

}

// function of practice chords
void practiceChords() {
  String chords[] = {"C Major", "A minor"};
  String chord = chords[rand() % 2];
  lcd.clear();
  lcd.setCursor (2,0);
  lcd.print ("Please play");
  lcd.setCursor (3,1);
  lcd.print(chord);
  delay(3000);

  bool is_chord_right = false;
  //for(int i = 0; i < 10; i++) {
    // check whether the chord is correct and light the led
    // call detect chord to get current pressed chord
    Serial.println(sensor_value_01);
    String detect_chord = detectChord();
    is_chord_right = checkChord(detect_chord, chord);
    if (is_chord_right == true) {
      lcd.clear();
      lcd.setCursor (0,0);
      lcd.print ("Succeed");
      delay(3000);
      return;
    }
    
    //delay(1000);
  //}
  if(is_chord_right == false) {
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print ("Failed! ");
    delay(3000);
  }
  
}

// function of detecting chords.
String detectChord() {
  Serial.println(sensor_value_01); // Serial catch the sensor value and draw the graph
  String chord = ""; // define the viable of chord

  // if the sensor value is 1023 (the highest value, because players need to press hard to make a sound on Ukulele)
  if(sensor_value_00 == 1023) { 
    has_start = true; // song already started
    lcd.clear(); // clear all of the words on LCD
    lcd.setCursor (2,0); // set the cursor at the first row, the third letter
    lcd.print ("Current Chord"); // print on LCD Current Chord
    lcd.setCursor (3,1); // set the cursor at the second row, the fouth letter
    lcd.print("C Major!"); // print on LCD C Major
    chord = "C Major"; // set the viable chord as C Major and return at the end of the function
  } else if (sensor_value_01 == 1023) {
    has_start = true; // song already started
    lcd.clear(); // clear all of the words on LCD
    lcd.setCursor (2,0); // set the cursor at the first row, the third letter
    lcd.print ("Current Chord"); // print on LCD Current Chord
    lcd.setCursor (3,1); // set the cursor at the second row, the fouth letter
    lcd.print("A minor!"); // print on LCD A minor
    chord = "A minor"; // set the viable chord as A minor and return at the end of the function
    if (sensor_value_02 == 1023) {
      //lcd.print("F Major!");
      chord = "F Major"; // set the viable chord as F Major and return at the end of the function
    } 
  } else {     // is not playing any chords
    if (has_start == true) {
      lcd.clear(); // clear all of the words on LCD
      lcd.setCursor (1,0); // set the cursor at the first row, the second letter
      lcd.print ("Current Chord:"); // print on LCD Current Chord
      lcd.setCursor (3,1); // set the cursor at the second row, the fouth letter
      lcd.print ("No Chord"); // print on LCD No Chord
    }
  }

  return chord; // return chord value
}

// function of checking whether the chord is correctly pressed
bool checkChord(String detect_chord, String given_chord) {
  bool is_chord_same = false;
  digitalWrite(led, LOW); // turn off the led

  // Compare the detected chord and the given chord
  if(detect_chord.equals(given_chord)) {
    digitalWrite(led, HIGH); // turn on the led
    is_chord_same = true; // chord is same
    
  }
  delay(500);
  digitalWrite(led, LOW); // turn off the led
  return is_chord_same; // return whether chord is the same
}
