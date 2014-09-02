#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

//AccelGyro//
MPU6050 accelgyro;
int16_t ax, ay, az;
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN 40
#define CSN_PIN 53
#define buttonPin 42

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int data[10], index=0;
int wrist, finger_1, finger_2, finger_3, finger_4, finger_5;
int button_state,lastButtonState,buttonPushCounter=1, joystick_x, joystick_y; 


void setup()
{
  Serial.begin(9600);
  Wire.begin();
  radio.begin();
  radio.openWritingPipe(pipe);
  pinMode(buttonPin, INPUT);
  accelgyro.initialize();

}

void loop()   
{
//Button//
button_state = digitalRead(buttonPin);
if (button_state != lastButtonState) 
    if (button_state == HIGH) {
      delay(20);
      buttonPushCounter++;
      } 
lastButtonState=button_state;
store(buttonPushCounter%2);//0
//Accel$&Gyro//
accelgyro.getAcceleration(&ax, &ay, &az);
store(ax);//1
store(ay);//2
//Wrist//  
/*wrist = analogRead(0);
store(wrist);
//Fingers//
finger_1 = analogRead(1);
finger_2 = analogRead(2);
finger_3 = analogRead(3);
finger_4 = analogRead(4);
finger_5 = analogRead(5);
store(finger_1);
store(finger_2);
store(finger_3);
store(finger_4);
store(finger_5);*/
//Joystick//
joystick_x = analogRead(6);
joystick_y = analogRead(7);
store(joystick_x);
store(joystick_y);

//SEND DATA TO RECEIVER//
radio.write( data, 11 );
index=0;
diag();
}

void store(int val)
{
data[index] = val;
index=index+1; 
}

void diag()
{
  delay(5);
Serial.println(data[0]);
Serial.println(data[1]);
Serial.println(data[2]);
Serial.println(data[3]);
Serial.println(data[4]);
Serial.println(data[5]);
Serial.println(data[6]);
Serial.println(data[7]);
Serial.println(data[8]);
Serial.println(data[9]);
Serial.println(data[10]);
Serial.println(" ");
}

