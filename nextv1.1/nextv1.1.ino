
#include <SPI.h>
#include <LoRa.h>

#include <TinyGPS++.h>

//  GPS
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);

//LORA
#define ss 5
#define rst 14
#define dio0 2

int counter = 0;

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //868E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 started at 9600 baud rate");
}
void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }
  //Send LoRa packet to receiver
  if (gps.location.isUpdated()) {
    Serial.print("LAT: ");
    Serial.println(gps.location.lat(), 6);
    Serial.print("LONG: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("SPEED (km/h) = ");
    Serial.println(gps.speed.kmph());
    Serial.print("ALT (min)= ");
    Serial.println(gps.altitude.meters());
    Serial.print("HDOP = ");
    Serial.println(gps.hdop.value() / 100.0);
    Serial.print("Satellites = ");
    Serial.println(gps.satellites.value());
    Serial.print("Time in UTC: ");
    Serial.println(String(gps.date.year()) + "/" + String(gps.date.month()) + "/" + String(gps.date.day()) + "," + String(gps.time.hour()) + ":" + String(gps.time.minute()) + ":" + String(gps.time.second()));
    Serial.println("");
    LoRa.beginPacket();
    LoRa.print("hello ");
    LoRa.print(counter);
    LoRa.endPacket();

    counter++;

    delay(1000);
  }
