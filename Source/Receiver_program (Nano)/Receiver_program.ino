//Title: Wireless Automatic Plant Watering System
//Program Made By: Muhammad Zubair SC
//
// receiver.pde
//
// Simple example of how to use VirtualWire to receive messages
// Implements a simplex (one-way) receiver with an Rx-B1 module
//
// See VirtualWire.h for detailed API docs
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2008 Mike McCauley
// $Id: receiver.pde,v 1.3 2009/03/30 00:07:24 mikem Exp $
//
//Declaration of library 'VritualWire.h'
#include <VirtualWire.h>
//
//
// Initialization of variables and pins of interfacing devices and modules
//
int count;
char a='0';
int indicator =1;
int motorpin=6;//sometimes pin 9 n 10 are dedicated to servo control therefore use other pins for more control of dc motor
int currentH=8;
int currentL=7;
int waterpump=2;
//Function Declaration 
void motorPump();
void Backward();
void Forward();
//
// Setup function of Arduino
//
void setup()
{   
    //Definig the operation of digital and PWM (Pulse Wdith Modulation) pins to be Output of Arduino
    pinMode(waterpump, OUTPUT);
    pinMode(motorpin, OUTPUT);
    pinMode(currentH, OUTPUT);
    pinMode(currentL, OUTPUT);
    //
    //Defining the receiver's pin, allowing the inverted copy of data to be retireve together with original data to cancel noise
    //and allowing the data to be received at 4000 bits per seconds
    //Start the receiver PLL running
    vw_set_rx_pin(12);      
    vw_set_ptt_inverted(true); 
    vw_setup(4000);	    
    vw_rx_start();
}
//
//Continuously executed program section
//
void loop()
{
    //Initializing the unsigned integer array 'buf' for receiving the data transmitted data
    //Initializing the unsigned integer variable 'buflen' to monitor the length of received data
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;   
    //If construct becomes true when the data inside 'buf' is  not empty and the communication between transmitter and receiver is established
    if (vw_get_message(buf, &buflen))
    {   
        //Initialize the counter variable 'i'
        //Turns on the LED to indicate that the comunication has been established
        //For loop executes till 'i' is less than the total length of the received data as it starts from 0, takes each content of array 'buf' and convert it into character and assign it to variable 'a'
	      int i;
        digitalWrite(13, true); 
	      for (i = 0; i < buflen; i++)
	         {
              char c = (buf[i]);
              a=c;            
	         }
    //If-elseif-else construct for watering the plants A,B and C. If varaible 'a' =='1' then the robot waters plant A, or if varaible 'a' =='2' 
    //then the robot waters plant B and if varaible 'a' =='3' then the robot waters plant c and does not do anything otherwise
   if(a=='1')
    { 
      //Switch case for tracking the location of the robot, if the robot location is 1 then only the water pump is executed
      //and if its location is 2 then the robot moves backward and the indicator is decrement by 1 and if its location is 3 then the 
      //robot wiil move two times backward together with decreasing indicator value from 3-2 and to 1 in the end and then water the plant A with a
      //5 second delay
      switch(indicator)
      {
         case 1:
           motorPump();
           delay(5000);
           break;
           
        case 2:
           Backward();
           indicator = indicator-1;
           motorPump();
           delay(5000);
           break;

        case 3:
          Backward();
          indicator = indicator-1;
          Backward();
          indicator = indicator-1;
          motorPump();
          delay(5000);

        default:
           break;
      }
    }

    else if(a=='2')
    {
      switch(indicator)
      { 
      //Switch case for tracking the location of the robot, if the robot location is 1 then the robot moves forward by adding 1 to 'indicator', if the 
      //location is 2 then the robot does not move at all and if the location 3 then the robot moves backward by subtracting 1 from 'indicator' and then waters the plant B in the end with
      //a 5 second dealy
        case 1:
          Forward();
          indicator=indicator+1;
          motorPump();
          delay(5000);
          break;

         case 2:
          motorPump();
          delay(5000);
          break;

        case 3:
          Backward();//try to eliminate delay in the end
          indicator=indicator-1;
          motorPump();
          delay(5000);
          break;

       default:
        break;
      }

    }
    else if(a=='3')
    {
      switch(indicator)
      {
      //Switch case for locating the position of the robot, if the robot location is 1 then the robot moves forward two times by adding 1 to 'indicator' each time, if the 
      //location is 2 then the robot moves forward one time by adding 1 to the variable 'indicator' and if the location 3 does not moves and then waters the plant B in the end with
      //a dealy of 5 seconds
        case 1:
          Forward();
           indicator=indicator+1;
           Forward();
           indicator=indicator+1;
           motorPump();
           delay(5000);
           break;

        case 2:
          Forward();
          indicator=indicator+1;
          motorPump();
          delay(5000);
          break;

        case 3:
          motorPump();
          delay(5000);
          break;

      default:
        break;
      }
  
    }
    else{
        }
   //Turns off the LED at pin 13 of Arduino
   digitalWrite(13, false);
    }
}
//
// Implementation of 'Forward()' function
//
void Forward()
{
 // Sets the varaible 'currentH' to high and 'currentL' to low and rotate the motor at 60% of its maximum speed
 //Adding a delay of 325 milliseconds and applying brakes to the motor and delay 1 seconds
 digitalWrite(currentH, HIGH);
 digitalWrite(currentL, LOW);
 analogWrite(motorpin, 150);
 delay(325);
 analogWrite(motorpin, 0);
 delay(1000);
}
//
// Implementation of 'Backward()' function
//
void Backward()
{
 // Sets the varaible 'currentH' to low and 'currentL' to high and rotate the motor at 60% of its maximum speed
 //Adding a delay of 325 milliseconds and applying brakes to the motor and delay 1 seconds
 digitalWrite(currentH, LOW);
 digitalWrite(currentL, HIGH);
 analogWrite(motorpin, 150);//angle is 420 degree
 delay(325);
 analogWrite(motorpin, 0);
 delay(1000);
}
//
// Implementation of 'motorPump()' function
//
void motorPump()
{
 //Sets the variable 'waterpump' to high, waits for 5 seconds and then sets it back to low and wait 500 milliseconds
 digitalWrite(waterpump, HIGH);
 delay(5000);
 digitalWrite(waterpump, LOW);
 delay(500);
}
