
  #include <ESP8266WiFi.h>
  #include <PubSubClient.h>
  // Connection info.
  #include <WiFiUdp.h>
  const char* ssid = "superman";
  const char* password =  "aaggiiff";
  const char* mqttServer = "broker.mqttdashboard.com";
  const int mqttPort = 1883;
  const char* clientID = "siagabanjirwwr";
  const char* channelName = "sbbbwwr";
  #include <NTPClient.h>
  WiFiClient MQTTclient;
    const long utcOffsetInSeconds = 25200;
  PubSubClient client(MQTTclient);
  WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

  
  String getValue(String data, char separator, int index){
        int found = 0;
        int strIndex[] = {0, -1};
        int maxIndex = data.length()-1;
      
        for(int i=0; i<=maxIndex && found<=index; i++){
          if(data.charAt(i)==separator || i==maxIndex){
              found++;
              strIndex[0] = strIndex[1]+1;
              strIndex[1] = (i == maxIndex) ? i+1 : i;
          }
        }
      
        return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
   }
   void tampilLcd(String x){
        char charBuf[x.length()];
        x.toCharArray(charBuf, x.length()); 
        Serial.println(charBuf);       
      }

  void callback(char* topic, byte* payload, unsigned int length) {
    String payload_buff;
    String pos;
    String tma;
    String statusSiaga;
    String kondisi;
    String dt;
    for (int i=0;i<length;i++) {
      payload_buff = payload_buff+String((char)payload[i]);
    }
    Serial.println("data dari server : ");
    Serial.println(payload_buff);  
    
    pos = getValue(payload_buff,',',0);
    dt = getValue(payload_buff,',',1);
    tma = getValue(payload_buff,',',2);
    statusSiaga = getValue(payload_buff,',',3);
    kondisi= getValue(payload_buff,',',4);

    Serial.print("pos :"); 
    Serial.println(pos);
    Serial.print("datetime :"); 
    Serial.println(dt);  
    Serial.print("TMA :"); 
    Serial.println(tma);
    Serial.print("status :"); 
    Serial.println(statusSiaga);
    Serial.print("Kondisi :"); 
    Serial.println(kondisi);    
    String x = pos+" "+tma+" "+statusSiaga+" "+kondisi;
    tampilLcd(x);

      
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
    Serial.println("subscribe to sbwwr");
    timeClient.begin();
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
      

  for (int i = 0; i < 4; i++){
      timeClient.update();
      Serial.println("Kirim data ke Server:");

      String h;
      String m;
      String s;

      if(timeClient.getHours()<10){
        h = '0'+String(timeClient.getHours());
        }else{
        h = String(timeClient.getHours());
          }
      if(timeClient.getMinutes()<10){
        m = '0'+String(timeClient.getMinutes());
        }else{
        m = String(timeClient.getMinutes());
        }
      if(timeClient.getSeconds()<10){
        s = '0'+String(timeClient.getSeconds());
        }else{
        s = String(timeClient.getSeconds());
        }

      int tma = 170;
      int flow = 1;
      int ch = 5;
      String x = "331,20200803"+h+m+s+","+tma+","+flow+","+ch+",";
      String a = "331,20200803"+h+m+s+","+"65249.28,37.24,98,121,6.559,0.98,13.851,13.851,12.941,0.474,0.542,0.430"+",";
      Serial.println(x);      
      Serial.println(a);      
      char y[x.length()];
      char b[a.length()];
      x.toCharArray(y,x.length());
      a.toCharArray(b,a.length());
      client.publish("dpswwrtes",y);
      client.publish("dpcdwwrtes",b);
      delay(5000);
    }
  
    }
  }
