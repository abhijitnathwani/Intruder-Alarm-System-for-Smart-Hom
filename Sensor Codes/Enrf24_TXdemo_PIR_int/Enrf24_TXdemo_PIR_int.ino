#include "Enrf24.h"
#include "nRF24L01.h"
#include <string.h>
#include <SPI.h>
Enrf24 radio (P1_0, P2_0, P2_5); // P1.0=CE, P2.0=CSN, P2.5=IRQ
const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };
volatile int flag1 = HIGH;
volatile int flag2 = HIGH;
void dump_radio_status_to_serialport(uint8_t);

void setup() {
  pinMode(P2_3, INPUT);
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
  radio.setTXaddress((void*)txaddr);
  attachInterrupt(P2_3,pir_ISR,CHANGE);
}

void loop() {
  if(flag1 == LOW)
  {
    Serial.println("Motion Detected");
    radio.print("31");
    radio.flush();  // Force transmit (don't wait for any more data)
    radio.deepsleep();
    //dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
    radio.flush();
    flag1 = HIGH;
  }
  
  if(flag2 == LOW)
  {
    Serial.println("Motion Detected");
    radio.print("31");
    radio.flush();  // Force transmit (don't wait for any more data)
    radio.deepsleep();
    //dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
    radio.flush();
    flag2 = HIGH;
  }
  
  //mydht.readRawData(P2_4, dhttemp,dhthumd, 0);
  //digitalWrite(P2_3,LOW);
  //Serial.print(dhttemp[0]);
  //Serial.print("\t");
  //Serial.println(dhthumd[0]);
  //    radio.print("3 ");
  //    radio.print(dhttemp[0]);
  //    radio.print(" ");
//    radio.print(dhthumd[0]);
//    radio.flush();  // Force transmit (don't wait for any more data)
//    radio.deepsleep();
//    //dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
//    radio.flush();
  sleep(200);
  
}
void pir_ISR(void)
{
  if(digitalRead(P2_3)==HIGH)flag1 = LOW;
  else flag2=LOW;
  
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
