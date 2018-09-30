#include <stdint.h>
#include "SparkFunBME280.h"
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <MQ2.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

BME280 mySensor;
LiquidCrystal_PCF8574 lcd(0x3F);  // set the LCD address to 0x27 for a 16 chars and 2 line display


//#define GAZPIN A0
#define RELAY 5

//MQ2 mq2(GAZPIN);
int lpg, co, smoke;
float humidity = 0;
float temperature = 0;
int pressure = 0;

void setup() {

  Serial.begin(9600);
  /* Инициализируем дисплей: 2 строки по 16 символов */
  //  lcd.begin(16, 2);
  //  lcd.display();
  //  lcd.setBacklight(255);
  pinMode(RELAY, OUTPUT);
  //mq2.begin();

  //***Driver settings********************************//
  //commInterface can be I2C_MODE or SPI_MODE
  //specify chipSelectPin using arduino pin names
  //specify I2C address.  Can be 0x77(default) or 0x76
  //For I2C, enable the following and disable the SPI section

  mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76; //Адрес датчика, в моём случае не стандартный

  //For SPI enable the following and dissable the I2C section
  //mySensor.settings.commInterface = SPI_MODE;
  //mySensor.settings.chipSelectPin = 10;

  //***Operation settings*****************************//

  //renMode can be:
  //  0, Sleep mode
  //  1 or 2, Forced mode
  //  3, Normal mode
  mySensor.settings.runMode = 3; //В примере предлагают использовать Forced mode, но при обновлении раз в секунду достаточно Normal mode

  //tStandby can be:
  //  0, 0.5ms
  //  1, 62.5ms
  //  2, 125ms
  //  3, 250ms
  //  4, 500ms
  //  5, 1000ms
  //  6, 10ms
  //  7, 20ms
  mySensor.settings.tStandby = 5; //Очевидно чаще не нужно

  //filter can be off or number of FIR coefficients to use:
  //  0, filter off
  //  1, coefficients = 2
  //  2, coefficients = 4
  //  3, coefficients = 8
  //  4, coefficients = 16
  mySensor.settings.filter = 0;

  //tempOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.tempOverSample = 1;

  //pressOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.pressOverSample = 1;

  //humidOverSample can be:
  //  0, skipped
  //  1 through 5, oversampling *1, *2, *4, *8, *16 respectively
  mySensor.settings.humidOverSample = 1;

  //Calling .begin() causes the settings to be loaded
  mySensor.begin();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  /*
    temperature = mySensor.readTempC();
    humidity = mySensor.readFloatHumidity();
    pressure = mySensor.readFloatPressure();
    Serial.println("Temperature ");
    Serial.println(temperature);
    Serial.println(" C");

    Serial.println("humidity ");
    Serial.println(humidity);
    Serial.println(" %");

    Serial.println("pressure ");
    Serial.println(pressure);
    Serial.println(" Pa");
    delay(5000);*/
  /*
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperature: ");
    lcd.setCursor(0, 1);
    lcd.print(temperature);
    lcd.print(" C");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.setCursor(0, 1);
    lcd.print(humidity);
    lcd.print(" %");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pressure: ");
    lcd.setCursor(0, 1);
    lcd.print(pressure / 133);
    lcd.print(" mmH");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pressure: ");
    lcd.setCursor(0, 1);
    lcd.print(pressure);
    lcd.print(" Pa");
    delay(1000);

  */
  /*
      if (humidity > 45)
      {
        digitalWrite(RELAY, HIGH);
        digitalWrite(LED_BUILTIN, HIGH);
      }
      else
      {
        digitalWrite(RELAY, LOW);
        digitalWrite(LED_BUILTIN, LOW);
      }
  */
  /*
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("LPG: ");
    lcd.setCursor(0, 1);
    lcd.print(mq2.readLPG());
    lcd.print(" ppm");
    delay(1000);


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("CO: ");
    lcd.setCursor(0, 1);
    lcd.print(mq2.readCO());
    lcd.print(" ppm");
    delay(1000);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("smoke: ");
    lcd.setCursor(0, 1);
    lcd.print(mq2.readSmoke());
    lcd.print(" ppm");
    delay(1000);
  */

}

