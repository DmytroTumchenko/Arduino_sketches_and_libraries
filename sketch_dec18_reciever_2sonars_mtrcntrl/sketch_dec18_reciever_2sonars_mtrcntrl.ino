#include <AccelStepper.h>
#include <NewPing.h>
#include <VirtualWire.h>
#define ledpin 13

const int receive_pin = 4;

// rgb - led pin configure
int led_1_Pin = 3;
int led_2_Pin = 5;
int led_3_Pin = 6;
NewPing sonar_left(2, 7, 20);
NewPing sonar_right(8, 9, 20);
unsigned short int left_sonar_distance;
unsigned short int right_sonar_distance;

AccelStepper stepper(1, 10, 11); //использует пин 10 и 11 для dir и step, 1 - режим "external driver" (A4988, drv8825)

int dir = 1; // direction of rotation


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

  stepper.setMaxSpeed(300); //устанавливаем максимальную скорость вращения ротора двигателя (шагов/секунду)
  stepper.setAcceleration(300); //устанавливаем ускорение (шагов/секунду^2)

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
  left_sonar_distance = sonar_left.ping_cm(200);
  right_sonar_distance = sonar_right.ping_cm(200);

  if ((left_sonar_distance < 20) &&  (left_sonar_distance > 0))
  {
    setColor_blink(0, 255, 0, 1);
    // setColor_blink(10, 100, 70, 1);
  }
  // if (sonar_right.ping_cm(200) < 20)  setColor_blink(100, 200, 20, 1);
  if ((right_sonar_distance < 20) &&  (right_sonar_distance > 0))
  {
    setColor_blink(0, 0, 255, 1);
  }

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    // Message with a good checksum received, dump HEX
    if (buf[0] == 49)
    {
      setColor_blink(255, 0, 0, 1);
      stepper.setMaxSpeed(800);
      stepper.setAcceleration(200);
      stepper.runToNewPosition(0);

      stepper.moveTo(800);
      while (stepper.currentPosition() != 300)
        stepper.run();
      // cause an overshoot as we whiz past 300
      stepper.setCurrentPosition(600);
      /*if (Stepper1.distanceToGo() == 0) { //проверка, отработал ли двигатель предыдущее движение
        Stepper1.move(1600 * dir); //устанавливает следующее перемещение на 1600 шагов (если dir равен -1 будет перемещаться -1600 -> противоположное направление)
        }
        Stepper1.run();*/ //запуск шагового двигателя. Эта строка повторяется вновь и вновь для непрерывного вращения двигателя
      //delay(1000);
    }
    if (buf[0] == 51)
    {
      setColor_blink(255, 0, 0, 3);

      stepper.setMaxSpeed(200);
      stepper.setAcceleration(50);
      stepper.runToNewPosition(0);

      stepper.moveTo(1500);
      while (stepper.currentPosition() != 400)
        stepper.run();
      // cause an overshoot as we whiz past 400
      stepper.setCurrentPosition(600);
      /*dir = dir * (-1); //отрицательное значение dir, благодаря чему реализуется вращение в противоположном направлении
        if (Stepper1.distanceToGo() == 0) { //проверка, отработал ли двигатель предыдущее движение
        Stepper1.move(1600 * dir); //устанавливает следующее перемещение на 1600 шагов (если dir равен -1 будет перемещаться -1600 -> противоположное направление)
        }

        Stepper1.run();*/ //запуск шагового двигателя. Эта строка повторяется вновь и вновь для непрерывного вращения двигателя
      //delay(1000);
    }
    if (buf[0] == 52)
    {
      setColor_blink(0, 255, 0, 1);

    }
    if (buf[0] == 53) setColor_blink(0, 255, 0, 2);
    if (buf[0] == 54) setColor_blink(0, 255, 0, 3);
    if (buf[0] == 55) setColor_blink(0, 0, 255, 1);
    if (buf[0] == 56) setColor_blink(0, 0, 255, 2);
    if (buf[0] == 57) setColor_blink(0, 0, 255, 3);
  }
  delay(500);
}

