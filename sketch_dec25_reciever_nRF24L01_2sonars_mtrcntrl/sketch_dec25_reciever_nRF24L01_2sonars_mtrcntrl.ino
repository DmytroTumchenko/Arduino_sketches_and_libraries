#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//#include <AccelStepper.h>
//#include <NewPing.h>



// rgb - led pin configure
int led_1_Pin = 3;
int led_2_Pin = 5;
int led_3_Pin = 6;
//NewPing sonar_left(2, 7, 20);
//NewPing sonar_right(8, 9, 20);
//unsigned short int left_sonar_distance;
//unsigned short int right_sonar_distance;

#define CE_PIN A0
#define CSN_PIN 10

const uint64_t pipe00 = 0xE8E8F0F0E1LL;
const uint64_t pipe01 = 0xE8E8F0F0E2LL;
const uint64_t pipe02 = 0xE8E8F0F0A2LL;
const uint64_t pipe03 = 0xE8E8F0F0D1LL;
const uint64_t pipe04 = 0xE8E8F0F0C3LL;
const uint64_t pipe05 = 0xE8E8F0F0E7LL;
/*
  Pipe - это труба с адресом из пяти байтов. В "трубы" мы передаем, трубы мы слушаем. Причем труба - это не номер канала передачи. Это идентификатор принимающего/передающего устройства на рабочем канале.
  Их можно организовать еще пять. Но есть ограничение: может различаться лишь последний значащий байт. Например, вот что еще можно определить:

  Меняем только последний байт!
*/

RF24 radio(CE_PIN, CSN_PIN); // Создаем объект Radio

//AccelStepper stepper(1, A7, A6); //использует пин 10 и 11 для dir и step, 1 - режим "external driver" (A4988, drv8825)

int dir = 1; // direction of rotation
int command = 0;

void setup()
{
  pinMode(A6, OUTPUT);
  pinMode(A7, OUTPUT);

  delay(1000);


  pinMode(led_1_Pin, OUTPUT);
  pinMode(led_2_Pin, OUTPUT);
  pinMode(led_3_Pin, OUTPUT);

  analogWrite(led_1_Pin, 0);
  analogWrite(led_2_Pin, 0);
  analogWrite(led_3_Pin, 0);

  Serial.begin(57600);
  radio.begin();
  radio.setChannel(0x60);
  radio.setDataRate(RF24_250KBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.openReadingPipe(0, pipe00);
  radio.setAutoAck(false);
  radio.setCRCLength(RF24_CRC_8);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(32);
  //radio.printDetails();

  //stepper.setMaxSpeed(300); //устанавливаем максимальную скорость вращения ротора двигателя (шагов/секунду)
  //stepper.setAcceleration(300); //устанавливаем ускорение (шагов/секунду^2)


}


void setColor_blink(int red, int green, int blue, int count)
{
  int iterator = 0;
  while (iterator < count)
  {
    //Serial.println(count);
    analogWrite(led_1_Pin, red);
    analogWrite(led_2_Pin, green);
    analogWrite(led_3_Pin, blue);
    delay(200);
    analogWrite(led_1_Pin, 0);
    analogWrite(led_2_Pin, 0);
    analogWrite(led_3_Pin, 0);
    delay(100);
    iterator++;
  }
}

void loop()
{

  //left_sonar_distance = sonar_left.ping_cm(200);
  //right_sonar_distance = sonar_right.ping_cm(200);
/*
  if ((left_sonar_distance < 20) &&  (left_sonar_distance > 0))
  {
    setColor_blink(10, 100, 70, 1);
  }
  // if (sonar_right.ping_cm(200) < 20)  setColor_blink(100, 200, 20, 1);
  if ((right_sonar_distance < 20) &&  (right_sonar_distance > 0))
  {
    setColor_blink(204, 255, 0, 1);
  }

*/
  if ( radio.available() )
  {
    radio.read( &command, sizeof(command) );  // по адресу переменной in функция записывает принятые данные;

  }
Serial.println(command);
  if (command == 1)
  {
    setColor_blink(255, 0, 0, 1);

  }
  if (command == 3)
  {
    setColor_blink(255, 0, 0, 3);


  }
  if (command == 4)
  {
    setColor_blink(0, 255, 0, 1);

  }
  if (command == 5)
  {
    setColor_blink(0, 255, 0, 2);
  }
  if (command == 6)
  {
    setColor_blink(0, 255, 0, 3);
  }
  if (command == 7)
  {
    setColor_blink(0, 0, 255, 1);
  }
  if (command == 8)
  {
    setColor_blink(0, 0, 255, 2);
  }
  if (command == 9)
  {
    setColor_blink(0, 0, 255, 3);
  }


  delay(500);
}

