#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int vrxPin = A1;
const int vryPin = A0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  Serial3.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Waiting...");
}

void loop() {
  if (Serial3.available()) {
    String data = Serial3.readStringUntil('\n');
    data.trim();

    Serial.print("Received from Car: ");
    Serial.println(data);

    lcd.clear();

    // Split the data by commas
    int idx1 = data.indexOf(',');
    int idx2 = data.indexOf(',', idx1 + 1);
    int idx3 = data.indexOf(',', idx2 + 1);

    String temp = data.substring(0, idx1);
    String hum = data.substring(idx1 + 1, idx2);
    String gas = data.substring(idx2 + 1, idx3);
    String distance = data.substring(idx3 + 1);

    // Print on LCD
    lcd.setCursor(0, 0);
    lcd.print(temp);
    lcd.setCursor(16, 0);
    lcd.print(hum);

    lcd.setCursor(0, 1);
    lcd.print(gas);
    lcd.setCursor(16, 1);
    lcd.print(distance);
  }

  int x = analogRead(vrxPin);
  int y = analogRead(vryPin);

  String command = "";

  int threshold = 100;
  if (y > 512 + threshold) {
    command = "FORWARD";
  } else if (y < 512 - threshold) {
    command = "BACKWARD";
  } else if (x > 512 + threshold) {
    command = "RIGHT";
  } else if (x < 512 - threshold) {
    command = "LEFT";
  } else {
    command = "STOP";
  }

  static String lastCommand = "";
  if (command != lastCommand) {
    Serial3.println(command);
    Serial.println("Sent: " + command);
    lastCommand = command;
  }

  delay(100);
}
