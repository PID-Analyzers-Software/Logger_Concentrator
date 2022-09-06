#include <Wire.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int timelap = 0;

// set pin numbers
const int buttonPin = 22;  // the number of the pushbutton pin
const int inputPin = 34;

const int numReadings  = 100;
int readings [numReadings];
int readIndex  = 0;
long total  = 0;


// variable for storing the pushbutton status
int buttonState = 0;
int time_remain = millis();


// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  pinMode(buttonPin, INPUT);
  delay(1000);
}

void loop() {
  delay(50);
  int time_now =  millis();
  float Voltage = smooth();
  Serial.println(Voltage);

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    time_remain = time_now + 5 * 60 * 1000;
  }

  if ((time_remain - time_now) > 0) {
    if (timelap < time_now) {
      SerialBT.print((String((5 * 60 * 1000 - (time_remain - time_now)) / 1000) + ", " + String(Voltage, 0) + ",ppb\n").c_str());
      Serial.println("Sending data...");
      timelap = time_now + 990;
    }
  }

}

long smooth() { /* function smooth */
  ////Perform average on sensor readings
  long average;
  // subtract the last reading:
  total = total - readings[readIndex];
  // read the sensor:
  readings[readIndex] = analogRead(inputPin) * 3300 / 4095;
  // add value to total:
  total = total + readings[readIndex];
  delay(50);
  // handle index
  readIndex = readIndex + 1;
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  // calculate the average:
  average = total / numReadings;
  return average;
}
