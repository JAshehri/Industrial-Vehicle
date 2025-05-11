#include <DHT.h>

// Motor control pins
const int N1 = 49;
const int N2 = 47;
const int N3 = 45;
const int N4 = 43;

// Sensor pins
#define MQ2_PIN A0
#define DHTPIN 52
#define DHTTYPE DHT11
#define TRIG_PIN 53
#define ECHO_PIN 51

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600); // HC-12 Communication

  pinMode(N1, OUTPUT);
  pinMode(N2, OUTPUT);
  pinMode(N3, OUTPUT);
  pinMode(N4, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  dht.begin();
  stopMotors();
}

void loop() {
  // Receive movement commands
  if (Serial3.available()) {
    String command = Serial3.readStringUntil('\n');
    command.trim();
    Serial.print("Command Received: ");
    Serial.println(command);

    if (command == "FORWARD") {
      goForward();
    } else if (command == "BACKWARD") {
      goBackward();
    } else if (command == "LEFT") {
      turnLeft();
    } else if (command == "RIGHT") {
      turnRight();
    } else if (command == "STOP") {
      stopMotors();
    }
  }

  // Every 2 seconds: Read sensors and send data
  static unsigned long lastSend = 0;
  if (millis() - lastSend >= 2000) {
    lastSend = millis();

    int gasValue = analogRead(MQ2_PIN);
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    long duration;
    float distance;

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
    distance = duration * 0.0343 / 2; // Convert to cm

    Serial.println("==== Sensor Readings ====");
    Serial.print("Gas (MQ2): "); Serial.println(gasValue);

    if (isnan(temp) || isnan(hum)) {
      Serial.println("DHT11 Error: Failed to read");
    } else {
      Serial.print("Temperature: "); Serial.print(temp); Serial.println(" °C");
      Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %");
    }

    Serial.print("Distance: "); Serial.print(distance); Serial.println(" cm");
    Serial.println("=========================");

    // Send sensor data with clear formatting
    Serial3.println(
      "Temp " + String(temp, 1) + "C," +
      "Humidity: " + String(hum, 1) + "%," +
      "Gas: " + String(gasValue) + " AQI," +
      "Distance: " + String(distance, 1) + "cm"
    );

  }
}

// Motor control functions
void goForward() {
  digitalWrite(N1, LOW);
  digitalWrite(N4, LOW);
  digitalWrite(N2, HIGH);
  digitalWrite(N3, HIGH);
}

void goBackward() {
  digitalWrite(N4, HIGH);
  digitalWrite(N3, LOW);
  digitalWrite(N1, HIGH);
  digitalWrite(N2, LOW);
}

void turnLeft() {
  digitalWrite(N3, HIGH);
  digitalWrite(N2, LOW);
  digitalWrite(N1, LOW);
  digitalWrite(N4, LOW);
}

void turnRight() {
  digitalWrite(N2, HIGH);
  digitalWrite(N3, LOW);
  digitalWrite(N1, LOW);
  digitalWrite(N4, LOW);
}

void stopMotors() {
  digitalWrite(N1, LOW);
  digitalWrite(N2, LOW);
  digitalWrite(N3, LOW);
  digitalWrite(N4, LOW);
}
