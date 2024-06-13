#include <Arduino.h>
#include <base64.hpp>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <base64.h>
#include <SPI.h>
#include <LoRa.h>
#include <stdio.h>

Adafruit_AHTX0 aht;
#define AHT10_ADDR 0x38
//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

void setup() {
  Serial.begin(9600);
  //sensor
  Serial.println("Adafruit AHT10/AHT20 demo!");

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");
  while (!Serial);
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }

  LoRa.setTxPower(5); //Sets transmission power (max is 20dBm)
  Serial.println("LoRa Initializing OK!");
  

}

void loop() {
  //delay(20000);
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  float temperature = temp.temperature;
  float humidity_value = humidity.relative_humidity;
  String temp_str = String(temperature);
  String humidity_str = String(humidity_value);
  
  Serial.println("TN"+temp_str+" "+"HN"+humidity_str);
  String mess_encoded = base64::encode("TN"+temp_str+" "+"HN"+humidity_str);
  
  Serial.println("Sending packet: ");
  Serial.println(mess_encoded);

  LoRa.beginPacket();
  LoRa.print(mess_encoded);
  LoRa.endPacket();
  delay(45000);
}
