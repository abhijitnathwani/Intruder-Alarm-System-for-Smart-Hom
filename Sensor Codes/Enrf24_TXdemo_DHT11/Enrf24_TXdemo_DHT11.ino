#include "Enrf24.h"
#include "dht.h"
#include "nRF24L01.h"
#include <string.h>
#include <SPI.h>
Enrf24 radio (P1_0, P2_0, P2_5); // P1.0=CE, P2.0=CSN, P2.5=IRQ
dht mydht;
const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

const char *str_on = "ON";
const char *str_off = "OFF";

void dump_radio_status_to_serialport(uint8_t);

void setup() {
  pinMode(P2_3, OUTPUT);
  digitalWrite(P2_3,LOW);
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());
  radio.setTXaddress((void*)txaddr);
}

void loop() {
  
  int16_t dhttemp[1];
  int16_t dhthumd[1];
  digitalWrite(P2_3,HIGH);
  sleep(200);
  mydht.readRawData(P2_4, dhttemp,dhthumd, 0);
  digitalWrite(P2_3,LOW);
  //Serial.print(dhttemp[0]);
  //Serial.print("\t");
  //Serial.println(dhthumd[0]);
  radio.print("3 ");
  radio.print(dhttemp[0]);
  radio.print(" ");
  radio.print(dhthumd[0]);
  radio.flush();  // Force transmit (don't wait for any more data)
  radio.deepsleep();
  //dump_radio_status_to_serialport(radio.radioState());  // Should report IDLE
  radio.flush();
  sleep(2000);
  
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
