#define trigPin A0
#define echoPin A1
#include AUnit.h
#include "SoftwareSerial.h"

SoftwareSerial blackBoardSlave(A4,A5); // (RX, TX), Faz comunicação master com o esp, usar portas certas
/* Usei software seial, pois achei mais fácil de entender.*/

//Testes

teste(duration) {
 asserNotNull(1,1);
}

long duration;

void setup(){
  blackBoardSlave.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   Serial.begin(9600);
 
 
}

void loop() {
  
 void duration(trigPin,echoPin);

 blackBoardSlave.print(duration); //Envia variável duration pro esp

   
  
  
 
}

//Função para mandar a variável duration
void duration(trigPin,echoPin){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);   
  return duration;
}
