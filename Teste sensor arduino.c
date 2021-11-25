#define trigPin A0
#define echoPin A1
//Arduino Uno

//Temos que pegar isso daqui, criar funções e escrever testes.
#include "SoftwareSerial.h"
SoftwareSerial blackBoardSlave(A4,A5); // (RX, TX), Faz comunicação master com o esp, usar portas certas
/* Usei software seial, pois achei mais fácil de entender.*/

long duration;
int Cm;

float A; 
float B; 
float L;

void setup(){
  blackBoardSlave.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   Serial.begin(9600);
 
 
}

void loop() {
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);  
  
  int kek = analogRead(trigPin);
  int cringe = analogRead(echoPin);
  
   
  Cm= duration*0.017175;
 
  
  A= (334 - Cm);
  B= A * 10000;
  L = B/1000;
  Serial.println("-------------");
  Serial.print(Cm);
  Serial.println("cm");
  delay(200);
  Serial.print( kek );
  Serial.println(" trigger");
   Serial.print( cringe );
   Serial.println(" echo");
   Serial.print( duration );
  Serial.println(" duração");
    Serial.println("-------------");
 blackBoardSlave.print(duration); //Envia variável duration pro esp

   
  
  
 
}