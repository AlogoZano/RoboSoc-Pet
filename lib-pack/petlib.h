#pragma once 
#include <Arduino.h> 
 
#define LEFT_PWM 11
#define LEFT_FWD 8
#define LEFT_REV 7

#define RIGHT_PWM 9
#define RIGHT_FWD 5
#define RIGHT_REV 4

void set_motor_pwm(int fwd_pin, int rev_pin, int pwm_pin, int value, char reversed);
void setLeft(int speed);
void setRight(int speed);

void motorInit(); 
#define SONAR_TRIG_LEFT 2
#define SONAR_ECHO_LEFT 3

#define SONAR_TRIG_RIGHT 13
#define SONAR_ECHO_RIGHT 12

#define SONAR_TRIG_FOR A0 //Forward connected to analogue pins as there is no space
#define SONAR_ECHO_FOR A1 //I think analogue pins can be treated like digital pins

#define SONAR_TIMEOUT 20000

unsigned long int sonarGetDistance(int trig, int echo);
void sonarInit();