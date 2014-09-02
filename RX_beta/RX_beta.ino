#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
/*-----( Declare Constants and Pin Numbers )-----*/
#define CE_PIN 40
#define CSN_PIN 53
#define OUT1PIN1 30     // H-bridge OUT1.1
#define OUT1PIN2 31   // H-bridge OUT1.2
#define ENABLE1 2    // H-bridge Ena 1
#define OUT2PIN1 32   // H-bridge OUT2.1
#define OUT2PIN2 33   // H-bridge OUT2.2
#define ENABLE2 3    // H-bridge Ena 2

const uint64_t pipe = 0xE8E8F0F0E1LL; // Define the transmit pipe
// NOTE: the "LL" at the end of the constant is "LongLong" type
/*-----( Declare objects )-----*/
RF24 radio(CE_PIN, CSN_PIN); // Create a Radio
/*-----( Declare Variables )-----*/
int data[10];  

void setup()   
{
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  pinMode(OUT1PIN1, OUTPUT);// setup Motor 1 pins
  pinMode(OUT1PIN2, OUTPUT);
  pinMode(OUT2PIN1, OUTPUT);// setup Motor 2 pins
  pinMode(OUT2PIN2, OUTPUT);
  pinMode(ENABLE1, OUTPUT);//pwm speed pins
  pinMode(ENABLE2, OUTPUT);
}

void loop() 
{
  if ( radio.available() )// Read the data payload until we've received everything
  {
    bool done = false;
    while (!done)// Fetch the data payload
    {
     done = radio.read( data, sizeof(data) );
 //  diag();
     control();
    }
  }
    else
    {
    delay(10);
    }
 
}
 
void control()
{
  int ok=0;
 
 if(data[1] < -6500)
     {ok=0;
     forward(map(abs(data[1])/1000, 0, 16, 0, 255));}
      else if(data[1] > 5500)  
     {ok=0;
     reverse(map(abs(data[1])/1000, 0, 16, 0, 255));}
      else
      {ok=1;
       halt();}

   if(ok==1) 
    if(data[2] < -6500) 
     left(map(abs(data[2])/1000, 0, 16, 0, 255));
    else if(data[2] > 6500)     
       right(map(abs(data[2])/1000, 0, 16, 0, 255));
        else
       halt(); 
}

 
void forward(int accel)
{
 digitalWrite(OUT1PIN1, HIGH);   
 digitalWrite(OUT1PIN2, LOW);
 digitalWrite(OUT2PIN1, LOW);   
 digitalWrite(OUT2PIN2, HIGH);
 analogWrite(ENABLE1, accel );
 analogWrite(ENABLE2, accel );
}

void reverse(int accel)
{
 digitalWrite(OUT1PIN1, LOW);   
 digitalWrite(OUT1PIN2, HIGH);
 digitalWrite(OUT2PIN1, HIGH);   
 digitalWrite(OUT2PIN2, LOW);
 analogWrite(ENABLE1, accel );
 analogWrite(ENABLE2, accel );
}

void left(int accel)
{
 digitalWrite(OUT1PIN1, LOW);   
 digitalWrite(OUT1PIN2, HIGH);
 digitalWrite(OUT2PIN1, LOW);   
 digitalWrite(OUT2PIN2, HIGH);
 analogWrite(ENABLE1, accel );
 analogWrite(ENABLE2, accel );
}

void right(int accel)
{
 digitalWrite(OUT1PIN1, HIGH);   
 digitalWrite(OUT1PIN2, LOW);
 digitalWrite(OUT2PIN1, HIGH);   
 digitalWrite(OUT2PIN2, LOW);
 analogWrite(ENABLE1, accel );
 analogWrite(ENABLE2, accel );
}

void halt()
{
 digitalWrite(OUT1PIN1, LOW);   
 digitalWrite(OUT1PIN2, LOW);
 digitalWrite(OUT2PIN1, LOW);   
 digitalWrite(OUT2PIN2, LOW);
 analogWrite(ENABLE1,0 );
 analogWrite(ENABLE2,0 ); 
 }
 
void diag()
{
  delay(500);
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
//map(val, 0, 1023, 0, 179);

