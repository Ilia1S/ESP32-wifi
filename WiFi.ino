#include "WiFi.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"


const char* ssid = "#Telia-6CA436";
const char* password = "CJ9RmR6nZsNRYkcR";

unsigned long lastTime = 0;
unsigned long timerDelay = 10000;

String jsonBuffer;

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    if (WiFi.status()== WL_CONNECTED)
    {
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=Vyborg,RU&APPID=000c09b80d47f05f9135b1469fc0f8ca";
       jsonBuffer = httpGETRequest(serverPath.c_str());
       Serial.println(jsonBuffer);
       JSONVar myObject = JSON.parse(jsonBuffer);
       if (JSON.typeof(myObject) == "undefined")
       {
        Serial.println("Parsing input failed!");
        return;
       }
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperature: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressure: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Humidity: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Wind Speed: ");
      Serial.println(myObject["wind"]["speed"]);
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
String httpGETRequest(const char* serverName)
{
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName);
  int httpResponseCode = http.GET();
  String payload = "{}"; 
  if (httpResponseCode>0) 
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else 
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  return payload;
}
