void setup_wifi() {

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("WiFi connected");
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "Clock";

    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");

      client.publish("clock", "clock,1");

      client.subscribe("clockSub");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      delay(2000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  String output = "";
  for (int i = 0; i < length; i++) {
    output = output + (char)payload[i];
  }
  if (output.equals("TOGGLE")) {
    state = !state;
    String msg = "clock," + String(state);
    client.publish("clock", msg.c_str());
    digitalWrite(relay, state);
  } else {
    char timeArray[8];
    
    output.toCharArray(timeArray, 8);

    digitalWrite(latchPin1, LOW);
    digitalWrite(latchPin2, LOW);
    digitalWrite(latchPin3, LOW);
    digitalWrite(latchPin4, LOW);

    shiftOut(dataPin1, clockPin1, MSBFIRST, dec_digits[(int)timeArray[0] - 48]);
    shiftOut(dataPin2, clockPin2, MSBFIRST, dec_digits[(int)timeArray[1] - 48]);
    shiftOut(dataPin3, clockPin3, MSBFIRST, dec_digits[(int)timeArray[2] - 48]);
    shiftOut(dataPin4, clockPin4, MSBFIRST, dec_digits[(int)timeArray[3] - 48]);

    digitalWrite(latchPin1, HIGH);
    digitalWrite(latchPin2, HIGH);
    digitalWrite(latchPin3, HIGH);
    digitalWrite(latchPin4, HIGH);
  }
}
