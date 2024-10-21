#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2
void setup(){
  Serial.begin(115200);
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(433E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
}
String both_data="";
void view_news(){
  Serial.println("REQUESTING NEWS DATA");
  String news_data = Serial.readStringUntil('\n');  // Read weather data until newline
}
void view_weather(){
  Serial.println("REQUESTING WEATHER DATA");
  String weather_data = Serial.readStringUntil('\n');

}
