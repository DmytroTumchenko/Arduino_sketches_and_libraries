/*
  Dmytro's reciever,  catch command from another controller, blink led to confirm recieving a right command, then controll stepper motor and distance
  based on RF24/pingpair_ack
*/



#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"


// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins
RF24 radio(A0, 10);

// rgb - led pin configure
int led_1_Pin = 3;
int led_2_Pin = 5;
int led_3_Pin = 6;

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.

typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;                                              // The role of the current running sketch

// A single byte to keep track of the data being sent back and forth
byte counter = 1;

void setup() {
  pinMode(led_1_Pin, OUTPUT);
  pinMode(led_2_Pin, OUTPUT);
  pinMode(led_3_Pin, OUTPUT);
  analogWrite(led_1_Pin, 0);
  analogWrite(led_2_Pin, 0);
  analogWrite(led_3_Pin, 0);

  Serial.begin(115200);
  printf_begin();
  Serial.print(F("\n\rRF24/examples/pingpair_ack/\n\rROLE: "));
  Serial.println(role_friendly_name[role]);
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));

  // Setup and configure rf radio

  radio.begin();
  radio.setAutoAck(1);                    // Ensure autoACK is enabled
  radio.enableAckPayload();               // Allow optional ack payloads
  radio.setRetries(0, 15);                // Smallest time between retries, max no. of retries
  radio.setPayloadSize(1);                // Here we are sending 1-byte payloads to test the call-response speed
  radio.openWritingPipe(pipes[1]);        // Both radios listen on the same pipes by default, and switch when writing
  radio.openReadingPipe(1, pipes[0]);
  radio.startListening();                 // Start listening
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging

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

void loop(void) {
  // Pong back role.  Receive each packet, dump it out, and send it back

  byte pipeNo;
  byte gotByte;                                       // Dump the payloads until we've gotten everything
  while ( radio.available(&pipeNo)) {
    radio.read( &gotByte, 1 );
    radio.writeAckPayload(pipeNo, &gotByte, 1 );

    Serial.print("Got and pinged out: ");
    Serial.println( gotByte);

    switch (gotByte) {
      case 1:
        setColor_blink(255, 0, 0, 1);
        break;
      case 3:
        setColor_blink(255, 0, 0, 3);
        break;
      case 4:
        setColor_blink(0, 255, 0, 1);
        break;
      case 5:
        setColor_blink(0, 255, 0, 2);
        break;
      case 6:
        setColor_blink(0, 255, 0, 3);
        break;
      case 7:
        setColor_blink(0, 0, 255, 1);
        break;
      case 8:
        setColor_blink(0, 0, 255, 2);
        break;
      case 9:
        setColor_blink(0, 0, 255, 3);
        break;
      default:

        break;
    }
  }
}
