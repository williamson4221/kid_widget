/*===========================================================================
 |   Assignment:  "Final Project"
 |
 |       Author:  Michael Williamson
 |
 |     Platform:  Arduino UNO
 |          IDE:  Arduino 1.8.15, Windows version
 |
 |        Class:  C Programming (2021FA.CSC.133.4101)
 |               
 |   Instructor:  Professor Fred Assi
 |     Due Date:  2021-12-13
 |
 +---------------------------------------------------------------------------
 |
 |  Description: Create a sketch that uses at least 1 digital input, 1 analog
 |  input, 2 different outputs, 1 interrupt function, 1 callable function,
 |  1 loop, and 1 control statement.
 |
 |  Input: User buttons 1 and 2, ambient light via photoresistor.  
 |
 |  Output: LCD display, 8 LEDs, fan.      
 |
*==========================================================================*/

#include <LiquidCrystal.h>                                        //library code last modified by Arturo Guadalupi and in the public domain

const int rs = 0, en = 1, d4 = 4, d5 = 5, d6 = 6, d7 = 7;         //initialize the library and associate it with the physical pins of the lcd
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int photoPin = 1;                                           //photoresistor sensor pin
const int button1 = 2, button2 = 8;                               //momentary switch pins                 
const int datapin = 10, clockpin = 11, latchpin = 12;             //shift register pins
const int brightCont = 3;                                         //digital pwm output pin for controlling screen brightness
const int fanPin = 9;                                             //digital output pin for fan control
volatile bool butt1flag = false;                                  //flag for ISR
byte data = 0;                                                    //for shift register

void setup() 
{
  pinMode(datapin, OUTPUT);
  pinMode(clockpin, OUTPUT);
  pinMode(latchpin, OUTPUT);
  pinMode(button1, INPUT);                                        //active HIGH
  pinMode(button2, INPUT);                                        //active HIGH
  pinMode(brightCont, OUTPUT);
  pinMode(fanPin, OUTPUT);
  attachInterrupt(0, isr, HIGH);                                  //interrupt is triggered with button one is pressed by the user
  lcd.begin(16, 2);                                               //set up the LCD's number of columns and rows:
}

void loop()                                                       //main loop 
{
  screenBrightness();                                             //update the screen brightness based on photoresistor 
  welcome();                                                      //main screen
  if (butt1flag == true)                                          //checks ISR flag
  {
    scanning();
  }
}

void isr()
{
  butt1flag = true;
}

void welcome()
{
  randomLED();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ghost Hunter");
  lcd.setCursor(0, 1);
  lcd.print("version 1.0");
  delay(250);                                                     //timing
}

void scanning()
{
  lcd.clear();
  int extent = random(3, 10);
  for (int i = 0; i < extent; i++)                                //loop runs a random interval between 3 and 10 cycles
  {
    lcd.setCursor(0, 0);
    lcd.print("Scanning...");
    knightRider();  
  }
  butt1flag = false;
  found();   
}

void found()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GHOST DETECTED");
  lcd.setCursor(0, 1);
  lcd.print("Press black!");
  allOn();
  for (int i = 0; i < 20; i++)                                     //should give the user about 5 seconds to push button 2                           
  {    
    if (digitalRead(button2) == HIGH)
    {    
      catching();                                                 //if the user presses button 2 at any time, the catch sequence will begin
      break;
    }
    delay(250);
  }
  allOff();                                                       //if the user doesn't push button 2, return to the main loop
}

void catching()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Vortex");
  lcd.setCursor(0, 1);
  lcd.print("ACTIVATED!");
  digitalWrite(fanPin, HIGH);                                     //turn on the fan
  int extent = random(8, 12);
  for (int i = 0; i < extent; i++)
  {
    vortex();     
  } 
  digitalWrite(fanPin, LOW);                                      //turn off the fan
  caught(); 
}

void caught()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Congratulations!");
  lcd.setCursor(0, 1);
  lcd.print("Ghost captured!");
  delay(4000);                                                    //displays message for 4 seconds
}


void screenBrightness()
{
  int ambientLight = analogRead(photoPin);                    
  int brightness = map(ambientLight, 0, 1023, 10, 255);
  analogWrite(brightCont, brightness);
}

void knightRider()
{
  int litPin;
  int delayTime = 100;                                            // time (milliseconds)
                                        
  for(litPin = 0; litPin <= 7; litPin++)                          // step through the LEDs, from 0 to 7
  {
    shiftWrite(litPin, HIGH);                                     // turn LED on
    delay(delayTime);                                             // pause to slow down the sequence
    shiftWrite(litPin, LOW);                                      // turn LED off
  }
  
  for(litPin = 7; litPin >= 0; litPin--)                          // step through the LEDs, from 7 to 0
  {
    shiftWrite(litPin, HIGH);                                     // turn LED on
    delay(delayTime);                                             // pause to slow down the sequence
    shiftWrite(litPin, LOW);                                      // turn LED off
  }
}

void randomLED()
{
  int index;
  int delayTime = 100;                                            // time (milliseconds) to pause between LEDs

  index = random(8);                                              // pick a random number between 0 and 7
  
  shiftWrite(index, HIGH);                                        // turn LED on
  delay(delayTime);                                               // pause to slow down the sequence
  shiftWrite(index, LOW);                                         // turn LED off
}

void allOn()
{
  int index;
  for(index = 0; index <= 7; index++)
  {
    shiftWrite(index, HIGH); 
  }
}

void allOff()
{
  int index;
  for(index = 8; index >= 0; index--)
  {
    shiftWrite(index, LOW); 
  }
}

void vortex()
{
  int delayTime = 200;                                         
  shiftWrite(0, HIGH);                                    
  shiftWrite(7, HIGH);                                 
  delay(delayTime);  
  shiftWrite(0, LOW);                                    
  shiftWrite(7, LOW); 
  shiftWrite(1, HIGH);                                    
  shiftWrite(6, HIGH);                                 
  delay(delayTime);
  shiftWrite(1, LOW);                                    
  shiftWrite(6, LOW);
  shiftWrite(2, HIGH);                                    
  shiftWrite(5, HIGH);                                 
  delay(delayTime);
  shiftWrite(2, LOW);                                    
  shiftWrite(5, LOW);
  shiftWrite(3, HIGH);                                    
  shiftWrite(4, HIGH);                                 
  delay(delayTime);
  shiftWrite(3, LOW);                                    
  shiftWrite(4, LOW);                                         
}

void shiftWrite(int desiredPin, boolean desiredState)
{
  bitWrite(data,desiredPin,desiredState);
  shiftOut(datapin, clockpin, MSBFIRST, data);
  digitalWrite(latchpin, HIGH);                                   //toggle the latchpin to send the data
  digitalWrite(latchpin, LOW);
}
