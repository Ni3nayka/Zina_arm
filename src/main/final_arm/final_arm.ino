int led = D3 ;
const int counter_servo = 6 ;
const int pins_servo[counter_servo] = {D5, D2, D1, D0, D6, D7};
int angle_servo [counter_servo] = {90, 90, 90, 90, 90, 90};
#define WIFI_NAME "DIR-803B"
#define WIFI_PASSWORD "mamamia123"
#define SERVER_IP "192.168.16.70"
#define SERVER_PORT 1234
#define ENABLE_AVOCADO_ESP_WIFI_GAMEPAD
#include <AVOCADO_esp.h>
AVOCADO_esp esp;

void setup() {
  Serial.begin(115200);
  esp.setup();
  for (int i = 0; i < counter_servo; i++)
  {
    pinMode(pins_servo[i], OUTPUT);
  } pinMode(led, OUTPUT);
}

void loop () {
  loop1();
  Read_data();
  upgrade_servoes();
}

void loop1() {
  esp.update();
  for (int i = 0; i < GAMEPAD_BUTTON_QUANTITY; i++) {
    Serial.print(esp.gamepad_button[i]); // bool
  }
  Serial.print(" <> ");
  // allow
  for (int i = 0; i < GAMEPAD_ARROW_QUANTITY; i++) {
    Serial.print(esp.gamepad_arrow[i]); // int
    Serial.print(" ");
  }
  Serial.print(" <> ");
  // joystick
  for (int i = 0; i < GAMEPAD_JOYSTICK_QUANTITY; i++) {
    Serial.print(esp.gamepad_joystick[i]); // int
    Serial.print(" ");
  }
  Serial.println();
}

void Read_data ()
{
  if ( esp.gamepad_button[5] == 1) angle_servo[5] = 0;
  else angle_servo [5] = 180;
  if ( esp.gamepad_button[4] == 1)angle_servo[2] ++;
  if ( esp.gamepad_button[6] == 1)angle_servo[2] --;
  if (esp.gamepad_joystick[0] == -100)angle_servo [0] ++;
  if (esp.gamepad_joystick[0] == 100)angle_servo [0] --;
  if (esp.gamepad_joystick[1] == -100)angle_servo [1] ++;
  if (esp.gamepad_joystick[1] == 100)angle_servo [1] --;
  if (esp.gamepad_joystick[2] == 100)angle_servo [3] ++;
  if (esp.gamepad_joystick[2] == -100)angle_servo [3] --;
  if (esp.gamepad_joystick[3] == -100)angle_servo [4] ++;
  if (esp.gamepad_joystick[3] == 100)angle_servo [4] --;
  if ( esp.gamepad_button[7] == 1) digitalWrite ( led, 1);
  if ( esp.gamepad_button[7] != 1) digitalWrite ( led, 0);
  for ( int i = 0; i < counter_servo ; i++)
  {
    angle_servo [i] = constrain (angle_servo [i], 0, 180);
  }
}
void upgrade_servoes() {
  for (int i = 0; i < counter_servo; i++) {
    // Serial.print (angle_servo [i]);
    //Serial.print( " ");
    write_servo(pins_servo[i], angle_servo[i]);
  } //Serial.println( " ");
}
void write_servo(int pin, int angle) {
  angle = constrain(angle, 0, 180);
  int pos = map(angle, 0, 180, 544, 2400);
  digitalWrite(pin, HIGH);
  delayMicroseconds(pos);
  digitalWrite(pin, LOW);
  delayMicroseconds(20000 - pos);
  delay(1);
}
