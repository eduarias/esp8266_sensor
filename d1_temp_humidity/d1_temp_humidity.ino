#include "DHTesp.h"

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!)
#error Select ESP8266 board.
#endif

#define DHTPIN D2

DHTesp dht;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println(ARDUINO_BOARD);
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");

  // Autodetect is not working reliable, don't use the following line
  // dht.setup(DHTPIN);
  // use this instead: 
  dht.setup(DHTPIN, DHTesp::DHT22); // Connect DHT sensor to GPIO 17
}

void loop() {
  //delay(dht.getMinimumSamplingPeriod());
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);
}
