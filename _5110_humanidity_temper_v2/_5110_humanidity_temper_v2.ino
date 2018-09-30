//
// AUTHOR: Nitin Sharma
// PURPOSE: Example to showcase the use of Nokia5110 lcd library
//
#include <Nokia_5110.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <MQ2.h>

#define RST 2
#define CE 3
#define DC 4
#define DIN 5
#define CLK 6

#define GAZPIN A0
#define DHTPIN            7         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT11     // DHT 11 

// LCD object
Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);

// For showing time since program started
unsigned long time;

MQ2 mq2(GAZPIN);
int lpg, co, smoke;

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  //
  // Initialize LCD with your custom pin configurations
  // initLCD(CLK,DIN,DC,CE,RST)
  // CLK needs to be PWM enabled
  //
 lcd.setContrast(80); // 60 is the default value set by the driver
  // Alternatively use the default lcd.initLCD(). In that
  // case default arduino configuration would be used which
  // is as follows:
  //
  // RST -> pin 6 arduino
  // CE -> pin 5 arduino 
  // DC -> pin 4 arduino
  // DIN -> pin 3 arduino
  // CLK -> pin 2 arduino
  // 

  Serial.begin(9600); 
  
  dht.begin();
  mq2.begin();
  // Print temperature sensor details.
   sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  lcd.setCursor(10,2);
  lcd.println("Temperature");  delay(1000);  lcd.clear();
  lcd.println("Sensor: "); lcd.print(sensor.name);
  delay(50);
  lcd.clear();
  lcd.print("Driver Ver: "); lcd.println(sensor.version);
  lcd.print("Unique ID: "); lcd.print(sensor.sensor_id);
  delay(50);
  lcd.clear();
  lcd.println("Max Value: "); lcd.print(sensor.max_value); lcd.println(" *C");
  lcd.println("Min Value: "); lcd.print(sensor.min_value); lcd.println(" *C");
  lcd.println("Resolution: "); lcd.print(sensor.resolution); lcd.print(" *C");
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  delay(50);
  lcd.clear();
  
  lcd.setCursor(15,2);
  lcd.println("Humidity");
  delay(50);
  lcd.clear();
  lcd.println("Sensor: "); lcd.println(sensor.name);
  lcd.print("Driver Ver: "); lcd.println(sensor.version);
  lcd.print("Unique ID: "); lcd.print(sensor.sensor_id);
  delay(50);
  lcd.clear();
  lcd.println("Max Value: "); lcd.print(sensor.max_value); lcd.println("%");
  lcd.println("Min Value: "); lcd.print(sensor.min_value); lcd.println("%");
  lcd.println("Resolution: "); lcd.print(sensor.resolution); lcd.print("%"); 
  delay(50);
  lcd.clear();
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  
}

void loop() {
  

  // Delay between measurements.
  delay(delayMS);
  lcd.setCursor(0,0);
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    lcd.clear();
    lcd.println("Error reading temperature!");
    delay(1000);
  }
  else {
    lcd.print("Temp: ");
    lcd.print(event.temperature);
    lcd.println(" *C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    lcd.clear();
    lcd.println("Error reading humidity!");
    delay(1000);
  }
  else {
    lcd.print("Hum: ");
    lcd.print(event.relative_humidity);
    lcd.println("%");
  }
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LPG: ");
  lcd.print(mq2.readLPG());
  lcd.println("ppm");
  lcd.print("CO: ");
  lcd.print(mq2.readCO());
  lcd.println("ppm");
  lcd.print("smoke: ");
  lcd.print(mq2.readSmoke());
  lcd.println(" ppm");
  delay(1000);
  lcd.clear();
  lcd.setContrast(70);
}
