#include <ESP8266WiFi.h>

const char* ssid = "TP-Link";             
const char* password = "69990101";     

const char* apiKey = "ddxDVOqNvFLcwLLJSUnrkYUdu35AsFPYGoHff7-wJUM";
const char* eventName = "CAR_CALL";
const char* host = "maker.ifttt.com";
const int port = 80;

int pirPin = D2;

void setup() 
{  
  pinMode(BUILTIN_LED ,OUTPUT);
  Serial.begin(9600);
  Serial.println("\n\nIFTTT Test\n");
  WiFi.begin(ssid, password);

  Serial.print("Connecting to network");
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(200);    
    if (++counter > 100) 
      ESP.restart();
    Serial.print( "." );
  }
  Serial.println("\nWiFi connected");
  pinMode(pirPin, INPUT);
  Serial.println("All configured!");
}

void loop() 
{
  if(digitalRead(pirPin) == HIGH)
  {    
    digitalWrite(BUILTIN_LED, LOW); 
    Serial.print("Connecting to ");
    Serial.print(host);
    Serial.print(':');
    Serial.println(port);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    if (!client.connect(host, port)) 
    {
      Serial.println("connection failed");
      delay(5000);
      return;
    }

    // This will send the trigger to the server
    if (client.connected()) 
    {
      String url = "/trigger/";
      url += eventName;
      url += "/with/key/";
      url += apiKey;
      
      Serial.println("Sending trigger to server");
      Serial.println(url);
      
      client.print(String("POST ") + url + " HTTP/1.1\r\n" +
             "Host: " + host + "\r\n" + 
             "Content-Type: application/x-www-form-urlencoded\r\n" + 
             "Content-Length: 0\r\n\r\n");
    }

    Serial.println("Trigger sent. Waiting for a bit...");
    delay(1*60*5
    00); // execute once every minute, don't flood remote service
    Serial.println("Ready for new trigger");
  }
  else if(digitalRead(pirPin) == LOW)
  {
    digitalWrite(BUILTIN_LED, HIGH);
  }
}
