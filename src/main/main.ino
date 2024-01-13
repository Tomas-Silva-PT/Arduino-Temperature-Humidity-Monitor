#include "DHT.h"
#include <LiquidCrystal_I2C.h>

#define lcdAddress 0x27
#define lcdColumns 16
#define lcdRows 2

#define DHTPIN 2
#define DHTTYPE DHT11

#define buttonPin 11

LiquidCrystal_I2C lcd(lcdAddress, lcdColumns, lcdRows); // Default address of most PCF8574 modules, change according
DHT dht(DHTPIN, DHTTYPE);

char tempModes[] = {'C', 'F'};
int currentTempModeIdx = 0;
int buttonPressed;
int previousButtonState = LOW;

int tempModesLength = sizeof(tempModes) / sizeof(tempModes[0]); // Get number of different Modes: https://thecircuitmaker.com/arduino-array-length/#Recommendation

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float tempC = dht.readTemperature(); // Celsius
  float tempF = dht.readTemperature(true); // Fahrenheit

  // Check if reading has failed
  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("DHT Sensor failed to make a reading!");
    lcd.setCursor(0, 0);
    lcd.print("DHT Sensor");
    lcd.setCursor(0, 1);
    lcd.print("failed to read");
  } else {
    // Check if user pressed the button
    buttonPressed = digitalRead(buttonPin);
    if ( buttonPressed == HIGH and previousButtonState != HIGH ) { // Prevent multiples changes when the button is being pressed for a long time (makes the button work like a switch)
      currentTempModeIdx = ( currentTempModeIdx + 1 ) % tempModesLength;
      previousButtonState = HIGH;
      Serial.println("Button Pressed!");
    }
    if ( buttonPressed == LOW ) {
      previousButtonState = LOW;
    }

    // Show Temperature based on the mode (Celsius of Fahrenheit)
    Serial.print("Temperature: ");
    switch(tempModes[currentTempModeIdx]) {
      case 'C':
        Serial.print(tempC);
        Serial.println(" ºC");

        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(tempC);
        lcd.write(32); // space
        lcd.write(223); // º
        lcd.print("C");

        break;
        case 'F':
        Serial.print(tempF);
        Serial.println(" ºF");

        lcd.setCursor(0, 0);
        lcd.print("Temp: ");
        lcd.print(tempF);
        lcd.write(32); // space
        lcd.write(223); // º
        lcd.print("F");

        break;
    }

    // Show humidity
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.println("%");

    lcd.setCursor(0, 1);
    lcd.print("Humidity: ");
    lcd.print(humidity);
    lcd.print("%");
  }

  delay(1000);
}
