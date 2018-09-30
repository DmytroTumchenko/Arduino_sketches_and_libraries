/*
  Dmytro's reciever,  catch command from another controller, blink led to confirm recieving a right command, then controll stepper motor and distance
  based on RF24/pingpair_ack
*/



#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
#include <AccelStepper.h>

// Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins
RF24 radio(A0, 10);

AccelStepper stepper(1, A5, A4); //использует пин A5 и A4 для dir и step, 1 - режим "external driver" (A4988, drv8825)

// rgb - led pin configure
int led_1_Pin = 3;
int led_2_Pin = 5;
int led_3_Pin = 6;
int left_barrier = A2;
int right_barrier = A3;
bool left_barrier_reached = false;  //  need to protect target holder from falling from rails after reentering command continue move left after reaching left barrier possition
bool right_barrier_reached = false;//  need to protect target holder from falling from rails after reentering command continue move right after reaching left barrier possition

// Topology
const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };              // Radio pipe addresses for the 2 nodes to communicate.

// Role management: Set up role.  This sketch uses the same software for all the nodes
// in this system.  Doing so greatly simplifies testing.

typedef enum { role_ping_out = 1, role_pong_back } role_e;                 // The various roles supported by this sketch
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};  // The debug-friendly names of those roles
role_e role = role_pong_back;                                              // The role of the current running sketch

typedef enum { right = 1, left } target_dir;                 // Direction of target movement in mode "left-right manual"
typedef enum { easy = 1, moderate, hard } target_difficult;       // Target difficult control in mode "random"

// A single byte to keep track of the data being sent back and forth
byte counter = 1;

void setup() {
  pinMode(A2, INPUT);
  pinMode(A4, INPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  pinMode(led_1_Pin, OUTPUT);
  pinMode(led_2_Pin, OUTPUT);
  pinMode(led_3_Pin, OUTPUT);
  analogWrite(led_1_Pin, 0);
  analogWrite(led_2_Pin, 0);
  analogWrite(led_3_Pin, 0);

  Serial.begin(115200);
  printf_begin();
  Serial.println(role_friendly_name[role]);

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

  stepper.setSpeed(50);
  stepper.setMaxSpeed(3000); //устанавливаем максимальную скорость вращения ротора двигателя (шагов/секунду)
  stepper.setAcceleration(300); //устанавливаем ускорение (шагов/секунду^2)
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

void target_move_left_right(target_dir a, int steps = 200, int move_speed = 200, int move_acceleration = 300) {
  stepper.setSpeed(move_speed);
  stepper.setAcceleration(move_acceleration); //устанавливаем ускорение (шагов/секунду^2)
  switch (a) {
    case right:
      left_barrier_reached = false;

      stepper.runToNewPosition(steps);
      stepper.setCurrentPosition(0);
      while ((digitalRead(right_barrier) == LOW) && (stepper.distanceToGo() <= steps) ) {
        stepper.runSpeed();
      }
      if (digitalRead(right_barrier) == HIGH)  right_barrier_reached = true;

      break;
    case left:
      right_barrier_reached = false;
      stepper.runToNewPosition(-1 * steps);
      stepper.setCurrentPosition(0);
      while ((digitalRead(left_barrier) == LOW) && (stepper.distanceToGo() <= steps) ) {
        stepper.runSpeed();
      }
      if (digitalRead(left_barrier) == HIGH)  left_barrier_reached = true;

      break;
    default:
      Serial.println("Set correct argument to target_move_left_right(): right or left");
      break;
  }
}

void target_move_strafe( int move_speed = 200, int move_acceleration = 300) {
  stepper.setSpeed(move_speed);
  stepper.setAcceleration(move_acceleration);

  while (radio.isAckPayloadAvailable() == false) {
    // move right to barrier
    if ( digitalRead(right_barrier) == LOW) {
      stepper.runToNewPosition(-10000);
      stepper.setCurrentPosition(0);
    }
    // move left to barrier
    if ( digitalRead(left_barrier) == LOW) {
      stepper.runToNewPosition( 10000 );
      stepper.setCurrentPosition(0);
    }
    stepper.runSpeed();
  }
}

void target_move_randomly( target_difficult tar_diff = easy) {
  int random_speed_min = 50;  // speed ranges to set random values acording to difficult
  int random_speed_max = 50;

  int random_acceleration_min = 50;// acceleration ranges to set random values acording to difficult
  int random_acceleration_max = 50;

  int random_pause_min = 1000;// pause in movement ranges to set random values acording to difficult
  int random_pause_max = 2000;

  switch (tar_diff) {
    case easy:
      random_speed_min = 50;
      random_speed_max = 200;
      random_acceleration_min = 50;
      random_acceleration_max = 100;
      break;
    case moderate:
      random_speed_min = 50;
      random_speed_max = 400;
      random_acceleration_min = 50;
      random_acceleration_max = 200;
      break;
    case hard:
      random_speed_min = 50;
      random_speed_max = 600;
      random_acceleration_min = 50;
      random_acceleration_max = 300;
      break;
    default:
      Serial.write("Choose correct difficult for target_move_randomly()  ");
      break;
  }

  stepper.setSpeed(random(random_speed_min, random_speed_max));
  stepper.setAcceleration(random(random_acceleration_min, random_acceleration_max));

  while (radio.isAckPayloadAvailable() == false) {
    
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
        target_move_left_right(left);
        break;
      case 3:
        setColor_blink(255, 0, 0, 3);
        target_move_left_right(right);
        break;
      case 4:
        setColor_blink(0, 255, 0, 1);
        target_move_strafe(300);
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

  if (digitalRead(left_barrier) == HIGH)     setColor_blink(10, 100, 70, 1);
  if (digitalRead(right_barrier) == HIGH)     setColor_blink(204, 255, 0, 1);

}
