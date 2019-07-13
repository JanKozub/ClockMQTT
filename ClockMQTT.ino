#include <WiFi.h>
#include <PubSubClient.h>

#define relay 19
#define dots 27

#define latchPin1 4
#define dataPin1 16
#define clockPin1 2

#define latchPin2 5
#define dataPin2 18
#define clockPin2 17

#define latchPin3 12
#define dataPin3 14
#define clockPin3 13

#define latchPin4 21
#define dataPin4 23
#define clockPin4 22

const char* ssid;
const char* password;
const char* mqtt_server;

byte dec_digits[] = {0b11000000, 0b11111001, 0b10100100, 0b10110000, 0b10011001, 0b10010010, 0b10000011, 0b11111000, 0b10000000, 0b10011000 };

WiFiClient espClient;
PubSubClient client(espClient);

String formattedDate;
String timeStamp;

int lastTime1 = 0;
int lastTime2 = 0;
int lastTime3 = 0;

boolean dotsState = true;
boolean state = true;

void setup() {
  importPrivateData();
  
  pinMode(dots, OUTPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, state);
  digitalWrite(dots, HIGH);

  pinMode(latchPin1, OUTPUT);
  pinMode(clockPin1, OUTPUT);
  pinMode(dataPin1, OUTPUT);
  pinMode(latchPin2, OUTPUT);
  pinMode(clockPin2, OUTPUT);
  pinMode(dataPin2, OUTPUT);
  pinMode(latchPin3, OUTPUT);
  pinMode(clockPin3, OUTPUT);
  pinMode(dataPin3, OUTPUT);
  pinMode(latchPin4, OUTPUT);
  pinMode(clockPin4, OUTPUT);
  pinMode(dataPin4, OUTPUT);

  Serial.begin(115200);

  setup_wifi();

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}
void loop() {
  long now = millis();

  if (!client.connected()) {
    reconnect();
  } else {
    client.loop();

    if (now - lastTime3 > 5000) {
      lastTime3 = now;
      String msg = "clock" + String(state);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("clock", msg.c_str());
     
    }
  }
}
