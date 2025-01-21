#include "IRremote.h"
#define ENABLE 5
#define DIRA 3
#define DIRB 4
#define ECHO 14
#define TRIG 6
#define BUZZ 9

#define STOP 100
#define V1 101
#define V2 102
#define V3 103

int receiver = 13;
int state = STOP;


IRrecv irrecv(receiver);
decode_results results; 

void startStop() {
  if (state == STOP)
    state = V1;
  else
    state = STOP;
}

void goDown() {
  if(state == V3)
    state = V2;
  else if(state == V2)
    state = V1;
}

void goUp() {
  if(state == V1)
    state = V2;
  else if(state == V2)
    state = V3;
}

void translateIR()
{
  switch(results.value)
  {
    case 0xFFC23D: Serial.println("PAUSE");
    case 553536955: Serial.println("PAUSE"); startStop();  break;
    case 0xFFE01F: Serial.println("-");    
    case 4034314555: Serial.println("-"); goDown();    break;
    case 0xFFA857: Serial.println("+");
    case 2747854299: Serial.println("+"); goUp();   break;
  }// End Case

  //delay(200); // Do not get immediate repeat


} //END translateIR
void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  pinMode(ENABLE,OUTPUT);
  pinMode(DIRA,OUTPUT);
  pinMode(DIRB,OUTPUT);
  pinMode(ECHO,INPUT);
  pinMode(TRIG,OUTPUT);
  pinMode(BUZZ, OUTPUT);
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver

}/*--(end setup )---*/

float duration, distance;

void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume(); // receive the next value
    if (state == STOP) {
      digitalWrite(ENABLE,LOW);
    } else if(state == V1) {
      analogWrite(ENABLE,128);
      digitalWrite(DIRA,HIGH);
      digitalWrite(DIRB,LOW);
    } else if(state == V2) {
      analogWrite(ENABLE,180);
      digitalWrite(DIRA,HIGH);
      digitalWrite(DIRB,LOW);
    } else {
      analogWrite(ENABLE,255);
      digitalWrite(DIRA,HIGH);
      digitalWrite(DIRB,LOW);
    }
  }  
  digitalWrite(TRIG, LOW);  
	delayMicroseconds(2);  
	digitalWrite(TRIG, HIGH);  
	delayMicroseconds(10);  
	digitalWrite(TRIG, LOW); 
  duration = pulseIn(ECHO, HIGH);
  distance = (duration*.0343)/2;  
  Serial.print("Distance: ");  
	Serial.println(distance); 
  if(state != STOP && distance <= 10) {
    digitalWrite(DIRA,LOW);
    analogWrite(BUZZ,200);
  } else {
    digitalWrite(DIRA,HIGH);
    digitalWrite(BUZZ,LOW);
  }
	delay(100);  
}


