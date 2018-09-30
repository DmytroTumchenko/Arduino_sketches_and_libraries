#include <RF24.h>
#include <RF24_config.h>
#include <printf.h>
#include <nRF24L01.h>


#include <Keypad.h>

const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns
// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte rowPins[ROWS] = { 9, 8, 7, 6 };
// Connect keypad COL0, COL1 and COL2, COL3 to these Arduino pins.
byte colPins[COLS] = { 5, 4, 3, 2 };

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
#define CE_PIN A0
#define CSN_PIN 10

int command[0];

const uint64_t pipe = 0xE8E8F0F0E1LL; // адресс потока, по которому идет отправка, по идее єтот адресс сдедует поменять когда буду клепать много мишенных установок, чтобы они друг другу не меали
RF24 radio(CE_PIN, CSN_PIN); // Создаем объект Radio

void setup()
{
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(pipe);
}


void loop()
{
  char key = kpd.waitForKey();


  if (key == 49)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0]  = 1;

    radio.write(command, sizeof(command));


    digitalWrite(13, 0);
  }
  if (key == 51)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0]  = 3;

    radio.write(command, sizeof(command));
    digitalWrite(13, 0);
  }
  if (key == 52)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0]  = 4;
    radio.write(command, sizeof(command));

    digitalWrite(13, 0);
  }
  if (key == 53)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0]  = 5;
    radio.write(command, sizeof(command));

    digitalWrite(13, 0);
  }
  if (key == 54)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0]  = 6;
    radio.write(command, sizeof(command));

    digitalWrite(13, 0);
  }
  if (key == 55)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0] = 7;
    radio.write(command, sizeof(command));

    digitalWrite(13, 0);
  }
  if (key == 56)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0]  = 8;
    radio.write(command, sizeof(command));

    digitalWrite(13, 0);
  }
  if (key == 57)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    command[0]  = 9;
    radio.write(command, sizeof(command));

    digitalWrite(13, 0);
  }

}

