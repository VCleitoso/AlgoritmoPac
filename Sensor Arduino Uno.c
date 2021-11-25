#define trigPin A0
#define echoPin A1
#include "SoftwareSerial.h"

int duration;

SoftwareSerial blackBoardSlave(A4,A5); 
/* Usei software serial, pois achei mais fácil de entender.*/


void setup(){
  blackBoardSlave.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   Serial.begin(9600);
 
 
}

void loop() {
 
duracao();

 blackBoardSlave.print(duration); //Envia variável duration pro esp

   
  
  
 
}

//Função para mandar a variável duration
void duracao(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);   
  long duration;
}
