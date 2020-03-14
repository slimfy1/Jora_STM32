//
// Ultrasonic HR-SC04 sensor test
//
// http://robocraft.ru
//

#include "Ultrasonic.h"

// sensor connected to:
// Trig(1) - 12, Echo(2) - 13
Ultrasonic ultrasonic1(10, 9);
Ultrasonic ultrasonic2(8, 7);
Ultrasonic ultrasonic3(6, 5);
float dist1_cm; 
float dist2_cm; 
float dist3_cm;
char buff[3]; 
char message[9];
char test_mes[9]={0,0,0,0,0,0,0,0,0};
int incomingByte;

void measure()
{
  dist1_cm = ultrasonic1.Ranging(CM);   // get distance
  delay(5);                 // arbitary wait time.
  dist2_cm = ultrasonic2.Ranging(CM);   // get distance
  delay(5);                 // arbitary wait time.
  dist3_cm = ultrasonic3.Ranging(CM);   // get distance
  delay(5);                 // arbitary wait time.
}

void composemessage()
{
  dtostrf(dist1_cm, 3, 0, buff); // Leave room for too large numbers!
  message[0]=buff[0];
  message[1]=buff[1];
  message[2]=buff[2];
  memset(buff, 0, sizeof(buff));
  dtostrf(dist2_cm, 3, 0, buff); // Leave room for too large numbers!
  message[3]=buff[0];
  message[4]=buff[1];
  message[5]=buff[2];
  memset(buff, 0, sizeof(buff));
  dtostrf(dist3_cm, 3, 0, buff); // Leave room for too large numbers!
  message[6]=buff[0];
  message[7]=buff[1];
  message[8]=buff[2];
  memset(buff, 0, sizeof(buff));
  Serial.write(message);
  delay(5);
}
void setup()
{
  Serial.begin(115200);             // start the serial port
}

void loop()
{
  if (Serial.available() > 0) 
  {
                // считываем принятый байт:
    incomingByte = Serial.read();
 
                // сообщаем, что именно мы получили:
    
    if(incomingByte = 1);
    {
      measure();
      composemessage(); 
    }

   }

   
}
