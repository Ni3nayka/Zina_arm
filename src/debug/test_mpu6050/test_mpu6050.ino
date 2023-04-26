#include <Wire.h>
const int MPU_addr=0x68;  // I2C адрес модуля MPU-6050
int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;
// WiFi network info.

void setup(){
 Serial.begin(9600);
 Wire.begin();
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);  // PWR_MGMT_1 register
 Wire.write(0);     // устанавливаем в 0 (это "пробуждает" модуль MPU-6050)
 Wire.endTransmission(true);
 Serial.println("Wrote to IMU");
  Serial.println("Connecting to ");
  
}
void loop(){
mpu_read();

}
void mpu_read(){
 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
 Wire.endTransmission(false);
 Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
 AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
 AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
 AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
 //Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
 GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
 GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
 GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
// Serial.print("Accelerometer Values: \n");
// Serial.print("AcX: "); Serial.print(AcX); Serial.print("\nAcY: "); Serial.print(AcY); Serial.print("\nAcZ: "); Serial.print(AcZ);   
// Serial.print("\nTemperature: " );  Serial.print(Tmp);
// Serial.print("\nGyroscope Values: \n");
// Serial.print("GyX: "); Serial.print(GyX); Serial.print("\nGyY: "); Serial.print(GyY); Serial.print("\nGyZ: "); Serial.print(GyZ);
// Serial.print("\n");
 //delay(1000);

  if (AcZ>=15500) { // держим руку ровно
    int porog = 3600;
    if (abs(AcY)>abs(AcX)) {
      if (AcY>porog) Serial.println("left");
      else if (AcY<-porog) Serial.println("right");
      else Serial.println("0");
    }
    else {
      if (AcX>porog) Serial.println("forward");
      else if (AcX<-porog) Serial.println("back");
      else Serial.println("0");
    }
  }
  else { // держим руку криво
    Serial.print("krivo - ");
    int porog = 10000;
    if (abs(AcY)>abs(AcX)) {
      if (AcY<-porog) Serial.println("left");
      else if (AcY>porog) Serial.println("right");
      else Serial.println("0");
    }
    else {
      if (AcX<-porog) Serial.println("forward");
      else if (AcX>porog) Serial.println("back");
      else Serial.println("0");
    }
  }
  
 //Serial.println(AcZ);
// Serial.print(" ");
// if (AcY>porog_Y) Serial.println("+");
// else if (AcY<-porog_Y) Serial.println("-");
// else Serial.println("0");
 }
