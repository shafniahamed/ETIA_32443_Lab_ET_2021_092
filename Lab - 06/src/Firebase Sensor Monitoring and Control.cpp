#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

//Wi-Fi connectivity
#define WIFI_SSID "redmi 11s"
#define WIFI_PASSWORD "12345678"

//  Firebase 
#define FIREBASE_HOST "your-project-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH " your_firebase_database_secret"

// Firebase object
FirebaseData fbdo;

// Pin 
const int sensorPin = A0;      // Analog input pin
const int ledPin = D2;         // Red LED connected to D2

void setup() {
  Serial.begin(115200);

  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println("Wi-Fi connected successfully");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}


void loop() {
  // Read analog sensor value
  int sensorValue = analogRead(sensorPin);

  // Since ESP8266 has one analog pin, the same analog line can be
  // used according to the practical setup for LDR / Gas sensor reading.
  int ldrValue = sensorValue;
  int gasValue = sensorValue;

  // Display values on Serial Monitor
  Serial.print("LDR Value: ");
  Serial.print(ldrValue);
  Serial.print(" | Gas Value: ");
  Serial.println(gasValue);

  Firebase.setInt(fbdo, "/Lab6/Sensors/LDR", ldrValue);
  Firebase.setInt(fbdo, "/Lab6/Sensors/Gas", gasValue);

  if (Firebase.getString(fbdo, "/Lab6/LED_Control")) {
    String ledStatus = fbdo.stringData();

    if (ledStatus == "ON") {
      digitalWrite(ledPin, HIGH);
      Serial.println("LED is ON");
    }
    else if (ledStatus == "OFF") {
      digitalWrite(ledPin, LOW);
      Serial.println("LED is OFF");
    }
  }

  delay(2000);   
}

