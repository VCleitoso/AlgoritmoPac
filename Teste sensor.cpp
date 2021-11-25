#define trigPin 25
#define echoPin 33

//const int trigPin = 25;
//const int echoPin = 33;
long duration;
int Cm;

float A; 
float B; 
float L;

void setup(){
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
  
   
  /*Cm= duration*0.017175;
  
  A= (334 - Cm);
  B= A * 10000;
  L = B/1000;*/
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
 

   
  
  
 
}
