// PubNub MQTT example using ESP8266.
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
// Connection info.
const char* ssid = "Instagram: @ksr_pro";
const char* password =  "kutjingmomjing";
const char* mqttServer = "broker.mqttdashboard.com";
const int mqttPort = 1883;
const char* clientID = "siagabanjirciliwung";
const char* channelName = "alfan123";
WiFiClient MQTTclient;
PubSubClient client(MQTTclient);

void callback(char* topic, byte* payload, unsigned int length) {
  String payload_buff;
  for (int i=0;i<length;i++) {
    payload_buff = payload_buff+String((char)payload[i]);
  }
  Serial.println(payload_buff); // Print out messages.
  if(payload_buff == "1001"){
//  Serial.println("success"); // Print out messages. 
  client.publish("alfan123","11/11/33/43/23/12/12/23/34/"); 
    }
}

long lastReconnectAttempt = 0;
boolean reconnect() {
  if (client.connect(clientID)) {
    client.subscribe(channelName); // Subscribe.
  }
  return client.connected();
}
void setup() {
  Serial.begin(9600);
  Serial.println("Attempting to connect...");
  WiFi.begin(ssid, password); // Connect to WiFi.
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Couldn't connect to WiFi.");
      while(1) delay(100);
  }
  client.setServer(mqttServer, mqttPort); // Connect to PubNub.
  client.setCallback(callback);
  lastReconnectAttempt = 0;
}
void loop() {
  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) { // Try to reconnect.
      lastReconnectAttempt = now;
      if (reconnect()) { // Attempt to reconnect.
        lastReconnectAttempt = 0;
      }
    }
  } else { // Connected.
    client.loop();
//    client.publish("alfan123","Hello world!"); // Publish message.
//    delay(1000);
  }
}