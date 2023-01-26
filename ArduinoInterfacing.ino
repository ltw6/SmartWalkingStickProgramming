#include <Arduino.h>
#include <Wire.h>

//Variable Initialization
const int trigPin = 5;
const int echoPin = 18;
const int motorPin = 12;
const int buzzerPin = 11;


const int BUFFER_SIZE = 1;
char buffer[BUFFER_SIZE];

int velocity = 5;

int dataOutput[2];

long duration;
int distanceCm;
int distanceInch;

const int MPU_ADDR = 0x68; //I2C address for MPU6050 Accelerometer module
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

//Definitions 
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Wire.begin(21,22,100000); //sda, scl, clock speed
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B0);
  Wire.write(0);
  Wire.endTransmission(true);
}
 
void vibrateMotor1(){
  analogWrite(12,300);
  delay(1000);
  analogWrite(12,0);
}

void vibrateMotor2(){
  analogWrite(12,500);
  delay(300);
  analogWrite(12,0);
  analogWrite(12,500);
  delay(300);
  analogWrite(12,0);
}
void vibrateMotor3(){
  analogWrite(12,800);
  delay(300);
  analogWrite(12,0);
  analogWrite(12,800);
  delay(300);
  analogWrite(12,0);
  analogWrite(12,800);
  delay(300);
  analogWrite(12,0);
}

void loop() 
{
  //Array position 0=distance, 1=Velocity, 2=MotorStatus, 3=mode1, 4=mode2, 5=mode3 6=AcX,7=AcY, 8=AcZ, 9=buzzerOn
  int outputArray[10] = { 0,0,0,0,0,0,0,0,0,0 };
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(true);
  Wire.beginTransmission(MPU_ADDR);
  Wire.requestFrom(MPU_ADDR, 14, true);
  outputArray[6] = Wire.read() //−− 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  outputArray[7] = Wire.read() //−− 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  outputArray[8] = Wire.read() //−− 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  for (int z = 6; z<=8; i++) {
    if (outputArray[z] > 2) {
      outputArray[9] = 1;
    }
  }
// Monitor distance from sensor

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  distanceInch = distanceCm * CM_TO_INCH;
//Send distance measured to GUI via serial port
  outputArray[0] = distanceCm;

    
//Compare data collected to prepare for alert
  if(distanceCm <= 60){
    vibrateMotor3();
    outputArray[5] = 1;
    outputArray[2] = 1;
  }
  else if(distanceCm <= 80){
    vibrateMotor2();
    outputArray[4] = 1;
    outputArray[2] = 1;
  }
  else if(distanceCm <= 100) {
    vibrateMotor1();
    outputArray[3] = 1;
    outputArray[2] = 1;
  }
  //Serial.println(outputArray);
  for (int i=0; i<=5; i++)
  {
    Serial.println(outputArray[i]);
  }
//Delay between sensor readings
  delay(100);
}
