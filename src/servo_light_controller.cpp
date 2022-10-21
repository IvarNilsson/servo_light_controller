/*
  servo_light_controller
  servo_light_controller
  ©Ivar Nilsson
  card: ESP32 (ESP32 Dev Module)
*/


#include <HTTPClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// WiFi credentials
const char* ssid = "";
const char* password = "";

int BTNon = 35;
int BTNoff = 34;

// 0=off 1=on
int state = 0;
String recived;

Servo myservo;
int servo = 33;

void setup() {
  pinMode(servo, OUTPUT);
  myservo.attach(servo); // attach the servo to our servo object
  myservo.write(90);

  pinMode(BTNon, INPUT);
  pinMode(BTNoff, INPUT);

  delay(1000);
  Serial.begin(115200);
  delay(1000);
  // connect to WiFi
  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  Serial.flush();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}
void loop() {

  if (digitalRead(BTNon) == HIGH && digitalRead(BTNoff) == HIGH) {
    //rst?
  } else if (digitalRead(BTNon) == HIGH) {
    Serial.println("on - force");
    turnOn();
    state = 1;
  } else if (digitalRead(BTNoff) == HIGH) {
    Serial.println("off - force");
    turnOff();
    state = 0;
  }

  recived = readB2();

  if (recived.equalsIgnoreCase("on") && state != 1) {
    Serial.println("on");
    turnOn();
    state = 1;
  } else if (recived.equalsIgnoreCase("off") && state != 0) {
    Serial.println("off");
    turnOff();
    state = 0;
  }



  delay(1000);
}


void turnOn() {
  myservo.write(50);
  delay(1000);
  myservo.write(80);
  delay(1000);
}

void turnOff() {
  myservo.write(150);
  delay(1000);
  myservo.write(100);
  delay(1000);
}

void turnWait() {
  myservo.write(90);
  delay(1000);
}

String readB2() {
  String payload;
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "";
    Serial.println("Making a request");
    http.begin(url.c_str()); //Specify the URL and certificate
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    if (httpCode > 0) { //Check for the returning code
      payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }
    else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  } else {
    Serial.println("FUUUUUUK");
  }
  return payload;
}