#define trigPin 4
#define echoPin 5
#define trigPin2 6
#define echoPin2 7
#include <Servo.h>

int openedTimes = 0;
int secondsFull = 0;

Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  pinMode(trigPin2,OUTPUT);
  pinMode(echoPin2,INPUT);
  myservo.attach(3);
  //Serial.println("Start the prototype");
  myservo.write(0); //so that it wouldn't move cause for some reason it does after launching Qt program
}

void loop() {
  long distance, duration, distance2, duration2;

  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration / 29 / 2;
  
  digitalWrite(trigPin2,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2,LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 / 29 / 2;

  if ((distance <= 10) && (distance2 <= 10)){
    light_alarm();
    openlid();
    light_alarm(); //light will light up twice - before opening and after it
  }
  else if ((distance <= 10) && (distance2 > 10)){
    openlid();
  }
  else if ((distance > 10) && (distance2 <= 10)){
    light_alarm();
  }
  
  String command;
  if(Serial.available()){
    command = Serial.readString();    
  }

  if(command == "pressed"){
    openlid();
  } else if (command == "lights") {light_alarm();}
  else if (command == "update") {updatingLCDs();};
}

void openlid(){
  myservo.write(180);
  delay(500);  //the lid will close after 2 seconds letting trash to fall
  myservo.write(0);
  delay(500);  //a break so that the person could move away
  //Serial.print("Opened");
  openedTimes += 1;
}

void light_alarm() {
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  delay(100);
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(10, HIGH);
  digitalWrite(11, HIGH);
  delay(100);
  digitalWrite(8, HIGH);
  digitalWrite(9, HIGH);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  delay(100);
  digitalWrite(8,LOW);
  digitalWrite(9,LOW);
  //Serial.print("Full");
  secondsFull += 1;
}

void updatingLCDs(){
  Serial.print(openedTimes);
  Serial.print(',');
  Serial.print(secondsFull);
  Serial.print(',');    
  Serial.flush();
}
