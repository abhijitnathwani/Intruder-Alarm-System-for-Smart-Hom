#include "Enrf24.h"
#include "nRF24L01.h"
#include <string.h>
#include <SPI.h>

#define led1 9
#define buzzer 5
#define button1 8
int arm=80;
long premillis=0;
Enrf24 radio (2, 4, 3);//(ce,csn,irq);
const uint8_t rxaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };
int change=0;
const char *str_on = "ON";
const char *str_off = "OFF";
char rxd[10];
void dump_radio_status_to_serialport(uint8_t);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
  radio.setRXaddress((void*)rxaddr);
  radio.enableRX();  // Start listening
  
  pinMode(led1,OUTPUT);
  pinMode(buzzer,OUTPUT);
  pinMode(button1,INPUT_PULLUP);
  
  digitalWrite(led1,LOW);
  digitalWrite(buzzer,LOW);
}

void loop() {
  char inbuf[100];
  //dump_radio_status_to_serialport(radio.radioState());  // Should show Receive Mode
  //while (!radio.available(true));
  if (radio.read(inbuf))
  { 
    if(arm == 81)
    {   
      Serial1.println(inbuf);
      Serial.println(inbuf);
    }
    else
    {   
      Serial.print("disarmed ");
      Serial.println(inbuf);
    }
  }
  
  if(Serial1.available()>=2)
  {
    rxd[0] = Serial1.read();
    rxd[1] = Serial1.read();
    if(rxd[0]=='5' && rxd[1] == '0')
    {
      digitalWrite(buzzer,LOW);
    }
    else if(rxd[0]=='5' && rxd[1] == '1')
    {
      digitalWrite(buzzer,HIGH);
    }
    else if(rxd[0]=='8' && rxd[1] == '0')
    {
      digitalWrite(led1,LOW);
      arm=80;
    }
    else if(rxd[0]=='8' && rxd[1] == '1')
    {
      digitalWrite(led1,HIGH);
      arm=81;
    }
  }

  if(Serial.available()>=2)
  {
    rxd[0] = Serial.read();
    rxd[1] = Serial.read();
    if(rxd[0]=='5' && rxd[1] == '0')
    {
      digitalWrite(buzzer,LOW);
    }
    else if(rxd[0]=='5' && rxd[1] == '1')
    {
      digitalWrite(buzzer,HIGH);
    }
    else if(rxd[0]=='8' && rxd[1] == '0')
    {
      digitalWrite(led1,LOW);
      arm=80;
    }
    else if(rxd[0]=='8' && rxd[1] == '1')
    {
      digitalWrite(led1,HIGH);
      arm=81;
    }
  }
  
  if(digitalRead(button1)==LOW && change==0 && (millis()-premillis > 500))
  {
    premillis = millis();
    if(arm==80)
    {
      arm=81;
      digitalWrite(led1,HIGH);
      change=1;
    }
     else if(arm==81)
      {
        arm=80;
        digitalWrite(led1,LOW);
        change=1;
      }
    }
  if(change==1 && arm==80)
  {
    Serial1.print("80");
    Serial.print("80");
    change=0;
  }
  else if(change==1 && arm==81)
  {
    Serial1.print("81");
    Serial.print("81");
    change=0;
  }
  delay(30);
}

void dump_radio_status_to_serialport(uint8_t status)
{
  Serial.print("Enrf24 radio transceiver status: ");
  switch (status) {
    case ENRF24_STATE_NOTPRESENT:
      Serial.println("NO TRANSCEIVER PRESENT");
      break;

    case ENRF24_STATE_DEEPSLEEP:
      Serial.println("DEEP SLEEP <1uA power consumption");
      break;

    case ENRF24_STATE_IDLE:
      Serial.println("IDLE module powered up w/ oscillators running");
      break;

    case ENRF24_STATE_PTX:
      Serial.println("Actively Transmitting");
      break;

    case ENRF24_STATE_PRX:
      Serial.println("Receive Mode");
      break;

    default:
      Serial.println("UNKNOWN STATUS CODE");
  }
}


