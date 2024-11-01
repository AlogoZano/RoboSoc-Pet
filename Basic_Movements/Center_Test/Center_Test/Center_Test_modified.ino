#define LEFT_PWM 11
#define LEFT_FWD 7
#define LEFT_REV 8

#define RIGHT_PWM 9
#define RIGHT_FWD 4
#define RIGHT_REV 5

#define SONAR_TRIG_LEFT 2
#define SONAR_ECHO_LEFT 3

#define SONAR_TRIG_RIGHT 13
#define SONAR_ECHO_RIGHT 12

#define SOUND_SPEED_MPS 340.0
#define MAX_DIST_CM 20
#define DISTANCE_COEFFICIENT 80 // Unused for now, can be used for alternate motor speed calculation.

unsigned long int echo_time;
unsigned long int echo_start;

void setLeft(int speed) {
  if (!speed) {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 0);
  } else if (speed > 0) {
    digitalWrite(LEFT_FWD, 1);
    digitalWrite(LEFT_REV, 0);
  } else {
    digitalWrite(LEFT_FWD, 0);
    digitalWrite(LEFT_REV, 1);
  }
  analogWrite(LEFT_PWM, abs(speed));
}

void setRight(int speed) {
  if (!speed) {
    digitalWrite(RIGHT_FWD, 0);
    digitalWrite(RIGHT_REV, 0);
  } else if (speed > 0) {
    digitalWrite(RIGHT_FWD, 1);
    digitalWrite(RIGHT_REV, 0);
  } else {
    digitalWrite(RIGHT_FWD, 0);
    digitalWrite(RIGHT_REV, 1);
  }
  analogWrite(RIGHT_PWM, abs(speed));
}

void echo_isr() {
  echo_time = micros() - echo_start;
  Serial.println(echo_time);
}

void setup() {
  // put your setup code here, to run once:
  //MOTOR A DIR
  pinMode(LEFT_FWD,OUTPUT);
  pinMode(LEFT_REV,OUTPUT);

  //MOTOR B DIR 
  pinMode(RIGHT_REV,OUTPUT);
  pinMode(RIGHT_FWD,OUTPUT);

  //SPEED CTRL
  pinMode(RIGHT_PWM,OUTPUT);
  pinMode(LEFT_PWM,OUTPUT);

  //Front ultrasound
  pinMode(SONAR_TRIG_LEFT, OUTPUT);
  pinMode(SONAR_ECHO_LEFT, INPUT);
  digitalWrite(SONAR_TRIG_LEFT, 1);

  pinMode(SONAR_TRIG_RIGHT, OUTPUT);
  pinMode(SONAR_ECHO_RIGHT, INPUT);
  digitalWrite(SONAR_TRIG_RIGHT, 1);

  Serial.begin(115200);
  
}

unsigned long int sonarGetDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  return pulseIn(echo, HIGH, 10000);
}

float getDistanceCentimeters(int trig, int echo)
{
	digitalWrite(trig, LOW);
	delayMicroseconds(2);
	digitalWrite(trig, HIGH);
	delayMicroseconds(10);
	digitalWrite(trig, LOW);
	unsigned long int echoTime = pulseIn(echo, HIGH, 10000);
	return ((float)echoTime * SOUND_SPEED_MPS / 10000.0) / 2;
}

float boundedDistanceRatio(float value, float minValue)
{
	float out;
	out = max(minValue - value, 0.0) / minValue;
	return out;
}

void loop() {
  delay(8);
  float distanceLeft = sonarGetDistanceCentimeters(SONAR_TRIG_LEFT, SONAR_ECHO_LEFT);
  delay(8);
  float distanceRight = sonarGetDistanceCentimeters(SONAR_TRIG_RIGHT, SONAR_ECHO_RIGHT);
  
  float distanceFactorLeft = 1.0 - boundedDistanceRatio(distanceLeft, MIN_DIST_CM);
  float distanceFactorRight = 1.0 - boundedDistanceRatio(distanceRight, MIN_DIST_CM);

  //float leftRightRatio = ((distanceRight == 0) || (distanceLeft == 0))?1.0:distanceLeft/distanceRight;
  //float distanceFactor = 80.0*((distanceLeft - distanceRight)/((float)(distanceLeft + distanceRight)));

  float speedLeft = 110.0 * distanceFactorLeft;
  float speedRight = 110.0 * distanceFactorRight;

  setLeft((int)speedLeft);
  setRight((int)speedRight);
}
