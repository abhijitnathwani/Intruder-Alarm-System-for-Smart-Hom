#include "Enrf24.h"
#include "nRF24L01.h"
#include <string.h>
#include <SPI.h>
Enrf24 radio (P1_0, P2_0, P2_5); // P1.0=CE, P2.0=CSN, P2.5=IRQ
const uint8_t txaddr[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0x01 };

const char *str_on = "ON";
const char *str_off = "OFF";

void dump_radio_status_to_serialport(uint8_t);

void setup() {
  pinMode(P2_4, OUTPUT);
  digitalWrite(P2_4,LOW);
  //attachInterrupt(P2_4,reed_on_ISR,RISING);
  //attachInterrupt(P2_4,reed_off_ISR,FALLING);
  Serial.begin(9600);
  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  radio.begin();  // Defaults 1Mbps, channel 0, max TX power
  dump_radio_status_to_serialport(radio.radioState());

  radio.setTXaddress((void*)txaddr);
}

void loop() {
    digitalWrite(P2_4,HIGH);
    sleep(1);
    //Serial.println(str_off);
    radio.print("2 ");
    radio.print(1023-analogRead(A4));//radio.print(str_off);
    digitalWrite(P2_4,LOW);
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
