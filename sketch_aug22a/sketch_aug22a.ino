#include <WiFi.h>
#include <movingAvg.h>                  // https://github.com/JChristensen/movingAvg
#include <Wire.h>
#include <Adafruit_ADS1015.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int timelap = 0;

// set pin numbers
const int buttonPin = 22;  // the number of the pushbutton pin
const int buzzPin =  4;    // the number of the LED pin

// variable for storing the pushbutton status
int buttonState = 0;
int time_remain = millis();

#define wifi_ssid             "22CD"
#define wifi_password       "00525508"


// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

movingAvg avgInput(10);                  // define the moving average object

Adafruit_ADS1115 ads(0x48);


void setup() {
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);

  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");


  // initialize the pushbutton pin as an input
  pinMode(buttonPin, INPUT);
  // initialize the LED pin as an output
  pinMode(buzzPin, OUTPUT);
  avgInput.begin();
  ads.begin();
  delay(1000);
}

void loop() {
  delay(100);
  int time_now =  millis();

  float Voltage = analogRead(4) * 3300 / 4095;

  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    time_remain = time_now + 5 * 60 * 1000;
  }

  if ((time_remain - time_now) > 0) {
    if (timelap < time_now) {
      SerialBT.print(("" + String(Voltage, 0) + ",mV, \n").c_str());
      Serial.println(Voltage);
      Serial.println("Sending data...");
      timelap = time_now + 1000;
    }
  }

}
