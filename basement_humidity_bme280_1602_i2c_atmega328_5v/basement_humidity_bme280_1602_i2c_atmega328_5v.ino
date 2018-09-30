#include <stdint.h>
#include "SparkFunBME280.h"
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <MQ2.h>
#include <Wire.h>
#include <LiquidCrystal_PCF8574.h>

BME280 mySensor;
LiquidCrystal_PCF8574 lcd(0x3F);  // set the LCD address to 0x27 for a 16 chars and 2 line display


#define GAZPIN A0
#define RELAY_PIN 5

MQ2 mq2(GAZPIN);
int lpg, co, smoke;
float humidity = 0;
float temperature = 0;
float pressure = 0;

long interval = 1000; // половина периода мигания (в миллисекундах)
long previousMillis = 0; // время, когда состояние светодиода обновлялось

#define SHOW_TEMPERATURE 1
#define SHOW_HUMIDITY 2
#define SHOW_PREASURE_MM 3
#define SHOW_PREASURE_PA 4
#define SHOW_LPG 5
#define SHOW_CO 6
#define SHOW_SMOKE 7

#define SHOW_FIRST 1
#define SHOW_LAST 7

#define HUMIDITY_LEVEL 70

short int show_state = 0;  // начнем с показа температуры

void setup() {

  Serial.begin(9600);
  /* Инициализируем дисплей: 2 строки по 16 символов */
  lcd.begin(16, 2);
  lcd.display();
  lcd.setBacklight(255);
  pinMode(RELAY_PIN, OUTPUT);
  mq2.begin();
  digitalWrite(RELAY_PIN, LOW);

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


  temperature = mySensor.readTempC();
  humidity = mySensor.readFloatHumidity();

  pressure = mySensor.readFloatPressure();
  lpg = mq2.readLPG();
  co = mq2.readCO();
  smoke = mq2.readSmoke();


  if (humidity >= HUMIDITY_LEVEL)
  {
    digitalWrite(RELAY_PIN, HIGH);
    delay(20);
    digitalWrite(LED_BUILTIN, HIGH);
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW);
    delay(20);
    digitalWrite(LED_BUILTIN, LOW);
  }

  if (show_state == 0) {
    show_state = SHOW_FIRST;
    show_measurements();
  }
  unsigned long currentMillis = millis(); // текущее время в миллисекундах

  if (currentMillis - previousMillis > interval) {
    // сохраняем последний момент, когда менялось состояние светодиода
    previousMillis = currentMillis;
    show_measurements();
  }
}


void show_measurements() {
  switch (show_state) {
    case SHOW_TEMPERATURE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Temperature: ");
      lcd.setCursor(0, 1);
      lcd.print(temperature);
      lcd.print(" C");
      break;
    case SHOW_HUMIDITY:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Humidity: ");
      lcd.setCursor(0, 1);
      lcd.print(humidity);
      lcd.print(" %");
      break;
    case SHOW_PREASURE_MM:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pressure: ");
      lcd.setCursor(0, 1);
      lcd.print(pressure / 133);
      lcd.print(" mmH");
      break;
    case SHOW_PREASURE_PA:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pressure: ");
      lcd.setCursor(0, 1);
      lcd.print(pressure);
      lcd.print(" Pa");
      break;
    case SHOW_LPG:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("LPG: ");
      lcd.setCursor(0, 1);
      lcd.print(lpg);
      lcd.print(" ppm");
      break;
    case SHOW_CO:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("CO: ");
      lcd.setCursor(0, 1);
      lcd.print(co);
      lcd.print(" ppm");
      break;
    case SHOW_SMOKE:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("smoke: ");
      lcd.setCursor(0, 1);
      lcd.print(smoke);
      lcd.print(" ppm");
      break;
    default:
      Serial.println("set correct showscreen");
      break;
  }
  if (show_state == SHOW_LAST) {
    show_state = SHOW_FIRST;
  }
  else show_state++;
}

