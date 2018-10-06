#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#include "DHT.h"
#include "config.h"

#define DHTPIN D4
#define DHTTYPE DHT22

const String deviceName = "Dormitorio Alberto";
const String apiUri = "http://192.168.1.44:8080/sensor/data";

DHT dht(DHTPIN, DHTTYPE);

struct Data
{
  float temperature;
  float humidity;
};

struct Data data;

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");
  dht.begin();
  configureWiFi(SSID, NETWORKPASSWORD);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(10000);

  data.humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  data.temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(data.humidity) || isnan(data.temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(data.humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(data.temperature);
  Serial.println(" *C ");
  
  sendData(&data);

}

void configureWiFi(char *ssid, char *password) {
  /*
   * Configure WiFi using SSID and password
   */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Connect to the WiFi
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

int sendData(struct Data *sensorData) {
  /*
   * Sends values for temperature an humidity
   */
  // JSON
  const size_t bufferSize = 2*JSON_OBJECT_SIZE(3);
  StaticJsonBuffer<bufferSize> jsonBuffer;
  // DynamicJsonBuffer jsonBuffer(bufferSize);
  
  JsonObject &root = jsonBuffer.createObject();
  
  JsonObject &data = root.createNestedObject("data");
  data["temperature"] = sensorData->temperature;
  data["humidity"] = sensorData->humidity;

  root["device_name"] = deviceName;
 
  char JSONmessageBuffer[300];
  root.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  Serial.println("JSON data:");
  Serial.println(JSONmessageBuffer);
  
  // Serial.printf("Connection status: %d\n", WiFi.status());
  // WiFi.printDiag(Serial);
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    Serial.println("Sending data ...");
 
    HTTPClient http;    //Declare object of class HTTPClient
 
    http.begin(apiUri);      //Specify request destination
    http.addHeader("Content-Type", "application/json");  //Specify content-type header
 
    int httpCode = http.PUT(JSONmessageBuffer);   //Send the request
    String payload = http.getString();                  //Get the response payload

    Serial.print("Response code: ");
    Serial.println(httpCode);   //Print HTTP return code
    Serial.print("Response payload: ");
    Serial.println(payload);    //Print request response payload
 
    http.end();  //Close connection
 
  }else{
     Serial.println("Error in WiFi connection");   
  }
}
