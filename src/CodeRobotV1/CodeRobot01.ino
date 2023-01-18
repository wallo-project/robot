#include <Servo.h>

Servo servoMoteurR;
Servo servoMoteurL;

int trigPin = 9;
int echoPin = 8;

int chrono = 0;
int chronoDeplace = 0;
int chronoDeplaceFin = 0;
int temp = 0;
int temp2 = 0;

float duration_us, distance_cm;

void setup() {
  Serial.begin (9600);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  servoMoteurR.attach(13);
  servoMoteurL.attach(12);
}

void loop() {
  Serial.println(chrono);
  Serial.println(chronoDeplace);
  Serial.println(chronoDeplaceFin);
  Serial.println(temp);

  servoMoteurR.detach();
  servoMoteurL.detach();
  
  if(checkObstacle()==false){
    if(!servoMoteurL.attached()){
      servoMoteurR.attach(13);
      servoMoteurL.attach(12);
    }
    if(chronoDeplace != 0 && chronoDeplaceFin == 0){
      chronoDeplaceFin = chrono;
      temp = chronoDeplaceFin - chronoDeplace;
      temp2 = temp + 4000;
    }
    if(temp != 0 ){
      Serial.println("Je tourne dans le sens après !");
      if((temp2 - temp) != 0){
        servoMoteurR.writeMicroseconds(1440);
        servoMoteurL.writeMicroseconds(1530);
        temp2 -= 400;
      }
      else{
        servoMoteurR.writeMicroseconds(1400);
        servoMoteurL.writeMicroseconds(1460);
        temp -= 400;
      }
      if(temp <= 0){
        temp = 0;
        temp2 = 0;
        chronoDeplace = 0;
        chronoDeplaceFin = 0;
      }
    }
    else{
      servoMoteurR.writeMicroseconds(1440);
      servoMoteurL.writeMicroseconds(1530);
    }
  }
  else{
    if(chronoDeplace == 0){
      chronoDeplace = chrono;
    }
    if(!servoMoteurL.attached()){
      servoMoteurR.attach(13);
      servoMoteurL.attach(12);
    }
    Serial.println("Je tourne !");
    servoMoteurR.writeMicroseconds(1520);
    servoMoteurL.writeMicroseconds(1540);
  }
  delay(400);
  chrono += 400;

}

bool checkObstacle() {
  bool obstacle = false;
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
   // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // print the value to Serial Monitor
  if(distance_cm < 25 && distance_cm > 1){
    obstacle = true;
  }

  Serial.print("distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");
  Serial.println(obstacle);

  return obstacle;
}