char foo;
#include <SPI.h>
#include <WiFly.h>
#include <stdio.h>
#include "Credentials.h"
#include <EEPROM.h>
WiFlyServer server(80);

unsigned char AC_LOAD = 7;    // Output to Opto Triac pin
unsigned char dimming = 65;  // Dimming level (0-100)
unsigned char old = 3;
int dimmingAddress = 0;
boolean cinematicAddress = 10;
unsigned char i;
boolean cinematic = true;
char msg[128];
int letterCount = 0; 
void setup() {
  // put your setup code here, to run once:
  pinMode(AC_LOAD, OUTPUT);// Set AC Load pin as output
  attachInterrupt(1, zero_crosss_int, RISING);
  WiFly.begin();

  if (!WiFly.join(ssid, passphrase)) {
    while (1) {
      // Hang on failure.
    }
  }
  Serial.begin(9600);  //Serial communication at 9600 buad for debugging 
  Serial.print("IP: ");
  Serial.println(WiFly.ip());  //Prints IP address once associated with the network
  dimming=60;     //If DHCP is on, this IP can change. If static IP is set, it should not.
  delay(50); 
  dimming=EEPROM.read(dimmingAddress);
  cinematic=EEPROM.read(cinematicAddress);
  Serial.println(dimming);
  Serial.println(cinematic);
  server.begin(); //initialize the server

}

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms 
  // Every zerocrossing : (50Hz)-> 10ms (1/2 Cycle) For 60Hz (1/2 Cycle) => 8.33ms 
  // 10ms=10000us
  
  int dimtime = (100*dimming);    // For 60Hz =>65    
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(10);         // triac On propogation delay (for 60Hz use 8.33)
  digitalWrite(AC_LOAD, LOW);    // triac Off
}



void loop() {
  WiFlyClient client = server.available();
      
  int msgIsolator = 0;

  //int inChar; //variable for reading in data from the phone/browser 
  //boolean isParsing = false;// flag for reading in and parsing data 
           
  if (client) {
    char c;

    while (client.connected()) 
    {
      if (client.available()) 
      {
        c = client.read();
        delay(10);
        //Uncomment this line to see the HTTP respone
        //Serial.print(c);
        //Printing out responses will increase the amount of time per action 

        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        
        //this is where the message that the user entered is going so this is where
        //we record it into a char array
        if (msgIsolator == 1) 
        {
          recordMessage(c);
          //delay(100);
        }
         if (c == '\n' && msgIsolator == 1) 
       {
         
         checkAction();
         
       }
    
        if (!client.available())
        {
           client.println();

          //This basically creates what the user will see on the site. 
          client.print("Dimmer: ");
        }

        //if you want to see all data comming in and going out uncomment the line below
        //Serial.print(c);
        switch (c) {
          
          case '=':
              //trim the fat of the soon to be recorded char so we only get the user entered message
              msgIsolator = 1;
              break;
          case'f':                    // off fast
            turnOff(20);
            break;
          case'n':                    // on fast
            turnOn(20);
            break;
          case'v':                    // off slow 
            if(cinematic) {
             turnOff(50);
            }
             break;
          case'b':                    // on slow
            if(cinematic) {
              turnOn(50);
            }
            break;
          case'c':                    // Cinematic Toggle  (disables keypresses v and b)
            cinematic=!cinematic;
            EEPROM.write(cinematicAddress, cinematic);
            dimming=dimming-15;
            delay(50);
            old=dimming=dimming+15;
            break;
         case'r':                     // reset saved values
            resetValues();
            break;
         case '+':
           increaseBrightness(5);
           break;
        case'-':
           reduceBrightness(5);
           break;
        }
      }
    }

    client.flush();
    //client.stop();
  }
                 

}

void turnOn (int delayTime) {
  
        int maxi=80;
  maxi=old>80?80:old;
  for (i=dimming;i>maxi;i--)
  {
    old=i;
    dimming=i;
    delay(delayTime);
  }
}
void turnOff (int delayTime) {
    for (i=dimming;i<95;i++)
    {
      dimming=i;
      delay(delayTime);
    }
   
}
void resetValues() {
  dimming = 50;
  cinematic = true;
  EEPROM.write(dimmingAddress, dimming);
  EEPROM.write(cinematicAddress, cinematic);
}

void reduceBrightness(int amount) {
  if(dimming<90)
  {
    dimming+=amount;
    old=dimming;
    EEPROM.write(dimmingAddress, dimming);
  }
}
void increaseBrightness(int amount) {
  if (dimming>10)
  {
    dimming-=amount;
    old=dimming;
    EEPROM.write(dimmingAddress, dimming);
  }
  else                      // blink, if reached the full brightness
  {
    dimming=45;
    delay(50);
    old=dimming=10;
  }
}




///////////////////////////////////////////////////////////////////////

char recordMessage (char incomingMsg)
{
  //letterCount = 0;
  msg[letterCount] = incomingMsg;
  letterCount++;
  //delay(100);
}
///////////////////////////////////////////////////////////////////////
void checkAction() 
{ 
  //Serial.print("CA ");
  //Serial.print(msg);
  /*for(int i=0;i<letterCount;i++)
  {
  Serial.print(i);Serial.print(": ");
  Serial.println(msg[i]);}*/
  dimming=getstr();
  old=dimming;
  //-------------------------------------------------------
   //Serial.print("dimming ");
   //Serial.println(dimming );
   int msgIsolator = 0;
   
   letterCount = 0;
   
   
}//end checkAction()

int getstr()
{
  String inString = "";
  if (msg[0]!='=')
  {
       inString +=msg[0];
       inString +=msg[1];
  }
  else 
  {
        inString +=msg[1];
        inString +=msg[2];
  }
  //Serial.println(inString );
  return inString.toInt();
}


