/*
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Modified Time: June 2015
 * Description: This demo can recognize 9 gestures and output the result, including move up, move down, move left, move right,
 * 				move forward, move backward, circle-clockwise, circle-counter clockwise, and wave.
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <Wire.h>       // Built-in
#include "paj7620.h"    // Go to Sketch, Include Library then Manage Library, then in the search box (top-right) enter PAJ7620, install the library

/* 
Notice: When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
        You also can adjust the reaction time according to the actual circumstance.
*/
#define GES_REACTION_TIME		40			// You can adjust the reaction time according to the actual circumstance.
#define GES_ENTRY_TIME			800			// When you want to recognize the Forward/Backward gestures, your gestures' reaction time must less than GES_ENTRY_TIME(0.8s). 
#define GES_QUIT_TIME		   	100

#define Anticlockwise 0
#define Clockwise     1

enum swipe_dir {L, R, U, D};
#define Up    5
#define Down  3  
#define Left  2
#define Right 4

void setup()
{
	uint8_t error = 0;

	Serial.begin(115200);
	Serial.println("\nPAJ7620U2 TEST DEMO: Recognises 9 gestures.");
  LED_Rotate(Clockwise,4);

	error = paj7620Init();			// initialize Paj7620 registers
	if (error) 
	{
		Serial.print("INIT ERROR,CODE:");
		Serial.println(error);
	}
	else
	{
		Serial.println("INIT OK");
	}
	Serial.println("Please input your gestures:\n");
}

void loop()
{
	uint8_t data = 0, data1 = 0, error;
	
	error = paj7620ReadReg(0x43, 1, &data);				// Read Bank_0_Reg_0x43/0x44 for gesture result.
	if (!error) 
	{
		switch (data) 									// When different gestures be detected, the variable 'data' will be set to different values by paj7620ReadReg(0x43, 1, &data).
		{
			case GES_RIGHT_FLAG:
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
	  			delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Left");
          LED_Flash(Left, 200);
        }          
				break;
			case GES_LEFT_FLAG: 
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Right");
          LED_Flash(Right, 200);
        }          
				break;
			case GES_UP_FLAG:
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Up");
          LED_Flash(Up, 200);
				}          
				break;
			case GES_DOWN_FLAG:
				delay(GES_ENTRY_TIME);
				paj7620ReadReg(0x43, 1, &data);
				if(data == GES_FORWARD_FLAG) 
				{
					Serial.println("Forward");
					delay(GES_QUIT_TIME);
				}
				else if(data == GES_BACKWARD_FLAG) 
				{
					Serial.println("Backward");
					delay(GES_QUIT_TIME);
				}
				else
				{
					Serial.println("Down");
          LED_Flash(Down, 200);
        }          
				break;
			case GES_FORWARD_FLAG:
				Serial.println("Forward");
        Forward(500);
				delay(GES_QUIT_TIME);
				break;
			case GES_BACKWARD_FLAG:		  
				Serial.println("Backward");
        Backward(500);
        delay(GES_QUIT_TIME);
				break;
			case GES_CLOCKWISE_FLAG:
				Serial.println("Clockwise");
        LED_Rotate(Clockwise,4);
        break;
			case GES_COUNT_CLOCKWISE_FLAG:
				Serial.println("anti-clockwise");
				LED_Rotate(Anticlockwise,4);
				break;  
			default:
				paj7620ReadReg(0x44, 1, &data1);
				if (data1 == GES_WAVE_FLAG) 
				{
					Serial.println("wave");
          LED_Wave(100);
      	}
				break;
		}
	}
	delay(100);
}

void LED_Flash(int pin, int duration) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  delay(duration);
  digitalWrite(pin, LOW);
}

void LED_Rotate(int rotate, int rotations) {
  for (int i = 0; i < rotations; i++) {
    LED_Flash((rotate == Clockwise) ? 2 : 2, 100);
    LED_Flash((rotate == Clockwise) ? 3 : 5, 100);
    LED_Flash((rotate == Clockwise) ? 4 : 4, 100);
    LED_Flash((rotate == Clockwise) ? 5 : 3, 100);
  }
}

void Forward(int duration){
  LED_On(2);
  LED_On(3);
  LED_On(4);
  delay(duration);
  LED_Off(2);
  LED_Off(3);
  LED_Off(4);
}

void Backward(int duration){
  LED_On(4);
  LED_On(5);
  LED_On(2);
  delay(duration);
  LED_Off(4);
  LED_Off(5);
  LED_Off(2);
}

void LED_On(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void LED_Off(int pin){
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void LED_Wave(int duration){
  LED_On(2);
  LED_On(3);
  LED_On(4);
  LED_On(5);
  delay(duration);
  LED_Off(2);
  LED_Off(3);
  LED_Off(4);
  LED_Off(5);
}

void LED_Swipe(swipe_dir swipe_direction, int swipes) {
  for (int i = 0; i < swipes; i++) {
    if (swipe_direction == R) {
      LED_Flash(2, 50);
      LED_Flash(4, 50);
    }
    if (swipe_direction == L) {
      LED_Flash(4, 50);
      LED_Flash(2, 50);
    }
    if (swipe_direction == U) {
      LED_Flash(5, 75);
      LED_Flash(3, 50);
    }
    if (swipe_direction == D) {
      LED_Flash(3, 50);
      LED_Flash(5, 75);
    }
    delay(300);
  }
}

