#include <VirtualWire.h>
#include <Keypad.h>


//const int rx_led = 17;
//const int tx_led = 13;
const int transmit_pin = 10;

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


void setup()
{
  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(1000);   // Bits per sec

  //pinMode(tx_led, OUTPUT);
  //pinMode(rx_led, OUTPUT);
  //digitalWrite(tx_led, LOW);
  //digitalWrite(rx_led, LOW);
  Serial.begin(9600);
}


void loop()
{
  char key = kpd.waitForKey();


  if (key == 49)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "1";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }
  if (key == 51)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "3";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }
  if (key == 52)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "4";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }
  if (key == 53)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "5";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }
  if (key == 54)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "6";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }
  if (key == 55)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "7";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }
  if (key == 56)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "8";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }
  if (key == 57)
  {
    digitalWrite(13, 1); // Flash a light to show transmitting
    char *msg = "9";
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx(); // Wait until the whole message is gone
    digitalWrite(13, 0);
  }

}

