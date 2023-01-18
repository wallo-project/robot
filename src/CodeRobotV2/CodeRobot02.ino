#include <Servo.h>

Servo servoMoteurR;
Servo servoMoteurL;

int trigPin = 9;
int echoPin = 8;

float duration_us, distance_cm;

float centerPoint, leftPoint, rightPoint;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  // Parametrage du capteur ultrason
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // On attache les servos
  servoMoteurR.attach(13);
  servoMoteurL.attach(12);
}

void loop() {
  // put your main code here, to run repeatedly:
  servoMoteurR.detach();
  servoMoteurL.detach();

  // Si aucun obstacle n'est detecter par WALL-O
  if(!checkObstacle()){
    if(!servoMoteurL.attached()){
      servoMoteurR.attach(13);
      servoMoteurL.attach(12);     
    }

    servoMoteurR.writeMicroseconds(1440);
    servoMoteurL.writeMicroseconds(1550);
  }
  else{
    //Lancement du check de l'obstacle
    if(!servoMoteurL.attached()){
      servoMoteurR.attach(13);
      servoMoteurL.attach(12);     
    }
    Serial.println("Scan de l'obstacle ...");
    scanObstacle();
    Serial.println(rightPoint);
    Serial.println(centerPoint);
    Serial.println(leftPoint);
  }
  delay(1000);
}

float useSonic(){
  // Utilisation du capteur
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  // Mesure de la durée entre l'envoie et la reception du son
  duration_us = pulseIn(echoPin,HIGH);
  distance_cm = 0.017 * duration_us;
  //Serial.println(distance_cm);
  return distance_cm;
}

bool checkObstacle() {

  //Serial.println("Lancement du check");
  // Variable contenant le résultat du check
  bool obstacleDetected = false;
  float distance;
  distance = useSonic();
  //Serial.println(distance);

  // On vérifie en fonction des la distance calculé, la présence de l'obstacle
  if(distance < 30 && distance > 1){
    obstacleDetected = true;
  }
  //Serial.println(obstacleDetected);
  return obstacleDetected;
}

void scanObstacle(){
  // On commence par check le point gauche de l'obstacle
  bool obstacleChecked = true;
  float distanceBase = useSonic();
  float distanceAvant = distanceBase;
  float distanceApres;

  Serial.println("Debut scan");
  while(obstacleChecked){
    servoMoteurR.attach(13);
    servoMoteurL.attach(12);
    servoMoteurL.writeMicroseconds(1440);
    servoMoteurR.writeMicroseconds(1440);
    delay(300);
    servoMoteurL.detach();
    servoMoteurR.detach();
    delay(1000);
    distanceApres = useSonic();
    (distanceAvant,distanceApres);
    if(distanceApres-distanceAvant > 50){
      obstacleChecked = false;
    }
    else{
      distanceAvant = distanceApres;
    }
  }
  leftPoint = distanceAvant;  
  delay(500);

  obstacleChecked = true;
  float bornInf = distanceBase - 5;
  float bornSup = distanceBase + 5;
  float currentDistance;

  
  Serial.println("retour position inital ...");
  while(obstacleChecked){
    servoMoteurR.attach(13);
    servoMoteurL.attach(12);
    servoMoteurL.writeMicroseconds(1540);
    servoMoteurR.writeMicroseconds(1540);
    delay(300);
    servoMoteurR.detach();
    servoMoteurL.detach();
    delay(1000);
    currentDistance = useSonic();
    Serial.println(currentDistance);
    if(currentDistance > bornInf && currentDistance < bornSup){
      obstacleChecked = false;
    }
  }
  delay(2000);

  distanceAvant = distanceBase;
  obstacleChecked = true;
  while(obstacleChecked){
    servoMoteurR.attach(13);
    servoMoteurL.attach(12);
    servoMoteurL.writeMicroseconds(1520);
    servoMoteurR.writeMicroseconds(1520);
    delay(300);
    servoMoteurL.detach();
    servoMoteurR.detach();
    delay(1000);
    distanceApres = useSonic();
    Serial.print(distanceAvant,distanceApres);
    if(distanceApres-distanceAvant > 50){
      obstacleChecked = false;
    }
    else{
      distanceAvant = distanceApres;
    }
    Serial.println(distanceApres);
  }
  rightPoint = distanceAvant;
  centerPoint = distanceBase;
}
