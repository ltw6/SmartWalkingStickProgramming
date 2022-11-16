//Variable Initialization
const int trigPin = 5;
const int echoPin = 18;

long duration;
float distanceSm;
float distanceInch;

//Definitions 
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

void setup() {
  //Initialize HCSR05 sensor input pins
  Serial.begin(9600); //Begin serial communication
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin,INPUT);
  
 
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
