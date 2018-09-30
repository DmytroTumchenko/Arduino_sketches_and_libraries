#include <Ultrasonic.h>

#define SONAR_1_TRIGGER_PIN  12
#define SONAR_2_TRIGGER_PIN  12

#define SONAR_1_ECHO_PIN     10
#define SONAR_2_ECHO_PIN     9

Ultrasonic ultrasonic_SONAR_1(SONAR_1_TRIGGER_PIN, SONAR_1_ECHO_PIN);
Ultrasonic ultrasonic_SONAR_2(SONAR_2_TRIGGER_PIN, SONAR_2_ECHO_PIN);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  float cmMsec, inMsec;
  long microsec = ultrasonic.timing();

  cmMsec = ultrasonic.convert(microsec, Ultrasonic::CM);
  
  Serial.print("MS: ");
  Serial.print(microsec);
  Serial.print(", CM: ");
  Serial.print(cmMsec);
  delay(1000);
}
