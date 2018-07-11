#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "config.h"

#define BLUE_LED 5 //灯1
#define GREEN_LED 14 //灯2

String BLUE_RES = "0";
String GREEN_RES = "0";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

void setup_wifi() {
  delay(10);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  String res;
  for (int i = 0; i < length; i++) {
     res+=(char)payload[i];
  }
  if (res=="update"){
    client.publish(BLUE_UUID, BLUE_RES.c_str());
    client.publish(GREEN_UUID, GREEN_RES.c_str());
    return;
  }
  if ((String)topic == (String)BLUE_UUID){
    BLUE_RES=res;
    if (res=="0"){
       digitalWrite(BLUE_LED, LOW);
    }else{
      digitalWrite(BLUE_LED, HIGH);
    }
  }
  if ( (String)topic == (String)GREEN_UUID ){
    GREEN_RES=res;
    int pin=res.toInt()*8;
    analogWrite(GREEN_LED, pin);
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(GUUID,USER,"123456")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(BLUE_UUID, "0");
      client.publish(GREEN_UUID, "0");
      // ... and resubscribe
      client.subscribe(BLUE_UUID);
      client.subscribe(GREEN_UUID);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  pinMode(BLUE_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  analogWrite(GREEN_LED, 0);//模拟信号输出
  Serial.begin(115200);
  waitKey();
  setup_wifi();
  client.setServer(mqtt_server,mqtt_port );
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    digitalWrite(2, LOW);
    reconnect();
  }
  client.loop();
   long now = millis();
   if (now - lastMsg > 1000) {
     lastMsg = now;
     digitalWrite(2, HIGH);
   }
   if (now - lastMsg > 500) {
     digitalWrite(2, LOW);
   }
}
