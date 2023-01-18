#include <Arduino.h>

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
float distanceCm;
float distanceInch;

//Definitions 
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  if (!mpu.begin()) {
  Serial.println("Failed to find MPU6050 chip");
  while (1) {
    delay(10);
    }
  }
    Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}
  }
void vibrateMotor1(){
  analogWrite(12,600);
  delay(1000);
  analogWrite(12,0);
}

void vibrateMotor2(){
  analogWrite(12,600);
  delay(300);
  analogWrite(12,0);
  analogWrite(12,600);
  delay(300);
  analogWrite(12,0);
}
void vibrateMotor3(){
  analogWrite(12,600);
  delay(300);
  analogWrite(12,0);
  analogWrite(12,600);
  delay(300);
  analogWrite(12,0);
  analogWrite(12,600);
  delay(300);
  analogWrite(12,0);
}

void loop() {
// Monitor distance from sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_SPEED/2;
  distanceInch = distanceCm * CM_TO_INCH;
//Send distance measured to GUI via serial port
  Serial.println(distanceCm);
//Monitor accelerometer values
  if (a.acceleration.x < 10) {
    Serial.println(a.acceleration.x);
  }
  else{
    digitalWrite(buzzerPin,HIGH);
    delay(10000);
  }
    
//Compare data collected to prepare for alert
  if(distanceCm <= 60){
    vibrateMotor3();
  }
  else if(distanceCm <= 80){
    vibrateMotor2();
  }
  else if(distanceCm <= 100) {
    vibrateMotor1();
  }

//Delay between sensor readings
  delay(1000);
}
