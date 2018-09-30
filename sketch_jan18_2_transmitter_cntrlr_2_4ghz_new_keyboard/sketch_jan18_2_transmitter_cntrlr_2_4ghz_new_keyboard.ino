#include <Keypad.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"


const byte ROWS = 4; // Four rows
const byte COLS = 3; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 8, 7, 6, 5 };
// Connect keypad COL0, COL1 and COL2, COL3 to these Arduino pins.
byte colPins[COLS] = { 4, 3, 2 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


int command = 0;
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

#define CE_PIN A0
#define CSN_PIN 10
RF24 radio(CE_PIN, CSN_PIN); // Создаем объект Radio

void setup()
{
  Serial.println("setup");
  pinMode(A0, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);

  Serial.begin(57600);
  radio.begin();

  radio.setChannel(0x60);
  radio.setPALevel(RF24_PA_MAX);
  radio.setPayloadSize(32);
  radio.setAutoAck(false);

  radio.setCRCLength(RF24_CRC_8);
  radio.setDataRate(RF24_250KBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.openWritingPipe(pipe00); //- если хотим что -то передавать.
  /*
    Что еще надо знать - openWritingPipe пишет адрес трубы в НУЛЕВУЮ область труб.
    Если вы дадите команду openReadingPipe(0,pipe01), то адрес первой команды будет затерт.
    И так, с возможностью передачи, модуль может слушать только пять труб, с 1 по 5.
  */

  radio.stopListening();
  radio.printDetails();
  //radio.testCarrier();

  //Serial.println("setup ended \n");
}


void loop()
{
  char key = kpd.getKey();


  if (key == 49)
  {
    command  = 1;
  }
  if (key == 51)
  {
    command  = 3;
  }
  if (key == 52)
  {
    command  = 4;
  }
  if (key == 53)
  {
    command = 5;
  }
  if (key == 54)
  {
    command  = 6;
  }
  if (key == 55)
  {
    command = 7;
  }
  if (key == 56)
  {
    command  = 8;
  }
  if (key == 57)
  {
    command  = 9;
  }
  radio.write(&command,  sizeof(command));
  //radio.printDetails();
  //Serial.println(command);
  delay(500);
}

int serial_putc( char c, FILE * ) {
  Serial.write( c );
  return c;
}

void printf_begin(void) {
  fdevopen( &serial_putc, 0 );
}

