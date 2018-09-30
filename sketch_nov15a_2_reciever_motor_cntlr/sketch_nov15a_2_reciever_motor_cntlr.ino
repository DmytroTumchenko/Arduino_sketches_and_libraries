#include <VirtualWire.h>
#define ledpin 13

const int receive_pin = 12;

// rgb - led pin configure
int led_1_Pin = 7;
int led_2_Pin = 8;
int led_3_Pin = 9;

void setup()
{
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, LOW);
  delay(1000);
  Serial.begin(9600);  // Debugging only
  Serial.println("setup");


  pinMode(led_1_Pin, OUTPUT);
  pinMode(led_2_Pin, OUTPUT);
  pinMode(led_3_Pin, OUTPUT);

  analogWrite(led_1_Pin, 0);
  analogWrite(led_2_Pin, 0);
  analogWrite(led_3_Pin, 0);

  // Initialise the IO and ISR
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_set_rx_pin(receive_pin);
  vw_setup(1000);  // Bits per sec
  vw_rx_start();       // Start the receiver PLL running
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
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;
  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    // Message with a good checksum received, dump HEX
    if (buf[0] == 49)
    {
      Serial.println("1 is pressed");
      setColor_blink(255, 0, 0, 1);
    }
    if (buf[0] == 51) setColor_blink(255, 0, 0, 3);
    if (buf[0] == 52) setColor_blink(0, 255, 0, 1);
    if (buf[0] == 53) setColor_blink(0, 255, 0, 2);
    if (buf[0] == 54) setColor_blink(0, 255, 0, 3);
    if (buf[0] == 55) setColor_blink(0, 0, 255, 1);
    if (buf[0] == 56) setColor_blink(0, 0, 255, 2);
    if (buf[0] == 57) setColor_blink(0, 0, 255, 3);
  }
}

