//Title: Wireless Automatic Plant Watering System
//Author: Muhammad Zubair SC
// 
// transmitter.pde
//
// Simple example of how to use VirtualWire to transmit messages
// Implements a simplex (one-way) transmitter with an TX-C1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: transmitter.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
//
//Library declaration before using its function in programming
#include <VirtualWire.h>
#include <LiquidCrystal.h>
//
//Decalring the function for interfacing the LCD pins to arduino by following the format
//LiquidCrystal(rs, enable, d4, d5, d6, d7) 
LiquidCrystal lcd(4, 7, 8, 9, 10, 11);
//
//Initialization of variables and Interfacing pins
char msg[]="";
int sensorPin1=A5;
int sensorPin2=A6;
int sensorPin3=A7;
int sensorValue1 =0;
int sensorValue2 =0;
int sensorValue3 =0;
int percent1 =0;
int percent2 =0;
int percent3 =0;
//Function Declaration 
void convertToPercent(int );
// Setup function of Arduino
void setup()
{
    //Starts the 20x2 LCD
    //Sets the cursor position to column 6 and first row
    //prints the 'WELCOME' in the LCD
    //Delay 1 second
    lcd.begin(20,2);
    lcd.setCursor(6,0);
    lcd.print("WELCOME");
    delay(1000);
    //Clears the LCD
    //Sets the cursor position to column 6 and first row
    //prints the 'To' in the LCD
    //Delay 1 second and clears the LCD
    lcd.clear();
    lcd.setCursor(8,0);
    lcd.print("TO");
    delay(1000);
    lcd.clear();
    //Loop for Text effect
    for(int k=0; k<3;k++)
    {
     //Set the curcor position to column 2 and prints the output on the first row
     //Set the curcor position to column 2 and prints the output on the second row and delay by 1 second
     //Clears the LCD and delay 500 milliseconds
      lcd.setCursor(2,0);
      lcd.print("AUTOMATIC PLANT");
      lcd.setCursor(2,1);
      lcd.print("WATERING SYSTEM");
      delay(1000);
      lcd.clear();
      delay(500);
    }
    //Display the statement in the quotation mark on the LCD, set the curcsor positon on LCD to column 2 and second row and print the another statement on LCD
    //and delay 3 Seconds
    lcd.print("DISPLAYING POT SOIL");
    lcd.setCursor(2,1);
    lcd.print(" MOISTURE LEVEL");
    delay(3000);
    lcd.clear();
    //
    //Defining the Operation of Analogue pins of sensors as Input
    pinMode(sensorPin1,INPUT);
    pinMode(sensorPin2,INPUT);
    pinMode(sensorPin3,INPUT);
   //Defining the transmitter pin, allowing the inverted copy of data to be sent together with original data to cancel noise
   //and allowing the data to be sent at 4000 bits per seconds
    vw_set_tx_pin(12);         
    vw_set_ptt_inverted(true); 
    vw_setup(4000);	        
}

void loop()
{
  //Acquiring the data from the analogue pins of Humidity Sensor and convert the data into percentage by using user defined function 'convertToPercent()'
  sensorValue1 = analogRead(sensorPin1);
  percent1 = round(convertToPercent(sensorValue1));
  sensorValue2 = analogRead(sensorPin2);
  percent2 = round(convertToPercent(sensorValue2));
  sensorValue3 = analogRead(sensorPin3);
  percent3 = round(convertToPercent(sensorValue3));
  //Held the pointer to LCD and clears any previous statement written on LCD and print the statement wirtten in quotation mark and the value of 'percent1','percent12'&'percent3' respectively
  //Delay by 1 second
  lcd.begin(20,2);
  lcd.clear();
  lcd.print("POT A=");
  lcd.print(percent1);
  lcd.print("%");
  lcd.setCursor(10,0);
  lcd.print("POT B=");
  lcd.print(percent2);
  lcd.print("%");
  lcd.setCursor(5,1);
  lcd.print("POT C=");
  lcd.print(percent3);
  lcd.print("%");
  delay(1000);
 //
 //If-elseif-else conditonal strcuture to implement the Priority Scheduling Technique according to which the program wirtes 1 to the 'msg' till the 'percent1' is greater than 50% and writes 2 to 'msg' till the mositure level of 'percent2' is greater than 50%
 // and writes 3 to 'msg' 3 till the mositure level of varibale 'percent3' is greater than 50% and writes 0 to 'msg' if all plants have satidfying mositure level diaplys the result on the LCD

  if((percent1<=50)||((percent1<=50)&&(percent2<=50))||((percent1<=50)&&(percent2<=50)&&(percent3<=50)))
    strcpy(msg,"1");//so when the plant 1 is watered completely then it will proceed to plant 2

  else if((percent2<=50)||((percent2<=50)&&(percent3<=50)))
    strcpy(msg,"2");

  else if(percent3<=50)
    strcpy(msg,"3");

  else
    {
      strcpy(msg,"0");
      lcd.print("PLANTS ARE WATERED!");
    }
  //
  //Turns on the LED at Pin 13 of Arduino
  //Sends the data written in the variable 'msg'
  //Wait for the transmission to be done
  //Delay 50 milliseconds
  digitalWrite(13, true);      // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg)); //Sending the message
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);   // Turn the LED off.
  delay(50);                 // A short gap.
} 
//
//
//Functions 'convertToPercent()' Implementation
//Fucntion takes the input and bound it to 660-1015 depending upon its value and map 660-1015 to 100-0 accrodingly to get percentage value of input data
//Handles any values computed by the method which is greater than 100 and limits it to 100
int convertToPercent(int value)
{
  int percentValue =0;
  value=constrain(value,660,1015);
  percentValue =map(value, 1015,660,0,100);
  if(percentValue>=100)
  percentValue=100;
  return percentValue;
}


