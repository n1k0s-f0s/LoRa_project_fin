#include <Arduino.h>
#include <base64.hpp>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <base64.hpp>
#include <SPI.h>
#include <LoRa.h>

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
//////////////////////////////////////////////////////////////////////////////////////
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
  
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);
  
  //Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" degrees C");
  //Serial.print("Humidity: "); Serial.print(humidity.relative_humidity); Serial.println("% rH");

  float temperature = temp.temperature;
  float humidity_value = humidity.relative_humidity;
  
  unsigned char string1[] = "Temperature in C degrees:";
  unsigned char base64_1[21];
  unsigned int base64_1_length = encode_base64(string1, strlen((char *) string1), base64_1);
  unsigned char string2[] = "Humidity (% rH):";
  unsigned char base64_2[21];
  unsigned int base642_length = encode_base64(string2, strlen((char *) string2), base64_2);

///////////////////////////////////////////////////////////////////////////////////////////
    Serial.println("Sending packet: ");
    Serial.println((char *) base64_1);
    Serial.println(temperature);
    Serial.println((char *) base64_2);
    Serial.println(humidity_value);
    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.println((char *) base64_1);
    LoRa.println(temperature);
    LoRa.println((char *) base64_2);
    LoRa.println(humidity_value);
    LoRa.endPacket();
  ////////////////////////////////////////////////////////////////////////////////////////
  delay(10000);
}
