#include <Wire.h>
#include <esp8266_wifi_Serial.h>
const int MPU_addr = 0x68; // I2C адрес модуля MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
// WiFi network info.
const int counter_servo = 6 ;
String alphabet = "0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvb";
int angle_servo [counter_servo] = {90, 90, 0, 90, 90, 90};

unsigned long int time1 = 0 ;

void setup() {
  Serial.begin(9600);
  WifiSerial.begin();
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // устанавливаем в 0 (это "пробуждает" модуль MPU-6050)
  Wire.endTransmission(true);
  Serial.println("Wrote to IMU");
  Serial.println("Connecting to ");
 /* WifiSerial.println ( "XXXXX0");
  delay (3000);
   WifiSerial.println ( "XXXX0X");
  delay (3000);
   WifiSerial.println ( "XXX0XX");
  delay (3000);
   WifiSerial.println ( "XX0XXX");
  delay (3000);
   WifiSerial.println ( "X0XXXX");
  delay (3000);
   WifiSerial.println ( "0XXXXX");
  delay (3000);
  WifiSerial.println ( "XXXXXX");

  while (1);
*/
}
void loop() {
  mpu_read();


}
bool sgatie = 0;
void palec ()
{
  const int datchik = analogRead (A0);
  if (datchik < 125) sgatie = 1;
  else sgatie = 0;
}







String SSS = "";
void mpu_read() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  //Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  // Serial.print("Accelerometer Values: \n");
  // Serial.print("AcX: "); Serial.print(AcX); Serial.print("\nAcY: "); Serial.print(AcY); Serial.print("\nAcZ: "); Serial.print(AcZ);
  // Serial.print("\nTemperature: " );  Serial.print(Tmp);
  // Serial.print("\nGyroscope Values: \n");
  // Serial.print("GyX: "); Serial.print(GyX); Serial.print("\nGyY: "); Serial.print(GyY); Serial.print("\nGyZ: "); Serial.print(GyZ);
  // Serial.print("\n");
  //delay(1000);
  SSS = "";
  for (int i = 0; i < counter_servo; i++)
  {
    SSS += alphabet[angle_servo [i] / 3];
    Serial.print (String(alphabet[angle_servo [i] / 3 ]));

    Serial.print ("-");
    Serial.print (angle_servo [i]);
    Serial.print (" ");
  }
  //
   Serial.print (SSS);
  Serial.print ("   ");
  if (time1 + 1000 < millis() ) {
    WifiSerial.println(SSS);
    time1 = millis();
  }
  if (AcZ >= 15500) { // держим руку ровно
    int porog = 3600;
    if (abs(AcY) > abs(AcX)) {
      if (AcY > porog) {
        Serial.println("left");
        angle_servo [0] -= 3;
      }
      else if (AcY < -porog) {
        Serial.println("right");
        angle_servo [0] += 3;
      }
      else Serial.println("0");
    }
    else {
      if (AcX > porog) {
        Serial.println("forward");

        angle_servo [1] += 3;
        angle_servo [2] -= 3;
      }
      else if (AcX < -porog) {
        Serial.println("back");

        angle_servo [1] -= 3;
        angle_servo [2] += 3;
      }
      else Serial.println("0");
    }
  }
  else { // держим руку криво
    Serial.print("krivo - ");
    int porog = 10000;
    if (abs(AcY) > abs(AcX)) {
      if (AcY < -porog) {
        Serial.println("left");
        angle_servo [4]--;
      }
      else if (AcY > porog) {
        Serial.println("right");

        angle_servo [4]++;
      }
      else Serial.println("0");
    }
    else {
      if (AcX < -porog) {
        Serial.println("forward");

        angle_servo [3]++;
      }
      else if (AcX > porog) {
        Serial.println("back");

        angle_servo [3]--;
      } else Serial.println("0");
    }
  }

  for (int i = 0; i < counter_servo; i++)
  {
    angle_servo [i] = constrain ( angle_servo [i], 10, 170 );
  }

  //Serial.println(AcZ);
  // Serial.print(" ");
  // if (AcY>porog_Y) Serial.println("+");
  // else if (AcY<-porog_Y) Serial.println("-");
  // else Serial.println("0");
}
