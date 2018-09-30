/*
DHT11Tester_ATtiny.ino

                    ATtiny 85
                      _____
              RESET -|     |- 5V
 Software Serial Rx -|     |- DHT11 Pin (pullup resistor 4.7k)
 Software Serial Tx -|     |-
                GND -|_____|-



This sketch demonstrates the usage of the DHT11 library on an ATtiny85. For details on wiring and information on possible compiler errors, see the link below.
http://projectsfromtech.blogspot.com/2014/02/dht11-on-attiny85.html





Last edited: 2/8/2014
Matthew
http://projectsfromtech.blogspot.com/


*/

//Celsius to Fahrenheit conversion
double Fahrenheit(double celsius)
{
  return 1.8 * celsius + 32;
}

// fast integer version with rounding
//int Celcius2Fahrenheit(int celcius)
//{
//  return (celsius * 18 + 5)/10 + 32;
//}


//Celsius to Kelvin conversion
double Kelvin(double celsius)
{
  return celsius + 273.15;
}

// dewPoint function NOAA
// reference (1) : http://wahiduddin.net/calc/density_algorithms.htm
// reference (2) : http://www.colorado.edu/geography/weather_station/Geog_site/about.htm
//
double dewPoint(double celsius, double humidity)
{
  // (1) Saturation Vapor Pressure = ESGG(T)
  double RATIO = 373.15 / (273.15 + celsius);
  double RHS = -7.90298 * (RATIO - 1);
  RHS += 5.02808 * log10(RATIO);
  RHS += -1.3816e-7 * (pow(10, (11.344 * (1 - 1 / RATIO ))) - 1) ;
  RHS += 8.1328e-3 * (pow(10, (-3.49149 * (RATIO - 1))) - 1) ;
  RHS += log10(1013.246);

  // factor -3 is to adjust units - Vapor Pressure SVP * humidity
  double VP = pow(10, RHS - 3) * humidity;

  // (2) DEWPOINT = F(Vapor Pressure)
  double T = log(VP / 0.61078); // temp var
  return (241.88 * T) / (17.558 - T);
}

// delta max = 0.6544 wrt dewPoint()
// 6.9 x faster than dewPoint()
// reference: http://en.wikipedia.org/wiki/Dew_point
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}


#include <dht11.h>
#include <SoftwareSerial.h>

dht11 DHT11;

#define DHT11PIN 2


SoftwareSerial mySerial(3, 4); // RX, TX


void setup()
{
  mySerial.begin(9600);
  mySerial.println("DHT11 TEST PROGRAM ");
  mySerial.print("LIBRARY VERSION: ");
  mySerial.println(DHT11LIB_VERSION);
  mySerial.println();
}

void loop()
{
  mySerial.println("\n");

  int chk = DHT11.read(DHT11PIN);

  //  mySerial.print("Read sensor: ");
  switch (chk)
  {
    case DHTLIB_OK:
      mySerial.println("OK");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      mySerial.println("Checksum error");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      mySerial.println("Time out error");
      break;
    default:
      mySerial.println("Unknown error");
      break;
  }

  mySerial.print("Humidity (%): ");
  mySerial.println((float)DHT11.humidity, 2);

  mySerial.print("Temperature (°C): ");
  mySerial.println((float)DHT11.temperature, 2);

  mySerial.print("Temperature (°F): ");
  mySerial.println(Fahrenheit(DHT11.temperature), 2);

  mySerial.print("Temperature (°K): ");
  mySerial.println(Kelvin(DHT11.temperature), 2);

  mySerial.print("Dew Point (°C): ");
  mySerial.println(dewPoint(DHT11.temperature, DHT11.humidity));

  mySerial.print("Dew PointFast (°C): ");
  mySerial.println(dewPointFast(DHT11.temperature, DHT11.humidity));

  delay(2000);
}
//
// END OF FILE
//
