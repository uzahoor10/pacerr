#include <Component.h>
#include <Arduino.h>

Component::Component() {}
//very high cost to have each state in a string, this var is for all objects
char readValue[15];

Component::Component(char* name1, int type1, int port1) {
  name=name1;
  type=type1;
  port=port1;
  if(type==DIGITAL)  {
    pinMode(port, OUTPUT);
  }

  emptyReadValue();
  state=0;
}
char* Component::getValue() {
  itoa(state,readValue, 10);
  return readValue;
}

char* Component::getTypeName() {
   static char* typeNames[] = {"DIGITAL", "ANALOG", "PWM", "RELAY", "LIGHT", "TEMPERATURE", "SERIAL", "LIB", "PING"};
  return typeNames[type];
}

char* Component::write(char* c1) {
 char* r="\0";
 if(type==DIGITAL || type==RELAY) {
   state = atoi(c1);
   digitalWrite(port, state);
   return r;
 }
 else if(type==PWM) {
   state = atoi(c1);
   analogWrite(port, atoi(c1));
   return r;
 }
 else if(type==SERIAL) {
   Serial.print(c1);
   return r;
 }   
}

void Component::emptyReadValue() {
  for(int x=0;x<15;x++) {
     readValue[x]='\0';
  }
}
char* Component::read() {
  emptyReadValue();
  if(type==ANALOG || type==LIGHT || type==TEMP) {
    state = analogRead(port);
    return getValue();
  }
  else if(type==DIGITAL || type==RELAY || type==PWM) {
    pinMode(port, INPUT);
    state = digitalRead(port);
    pinMode(port,OUTPUT);
    return getValue();
  } else if(type==SERIAL) {

    int counter=0;
    while(Serial.available()>0 && counter<15) 
    {
      char c = Serial.read(); 
      delay(5);
      readValue[counter++]=c;
    }
    return readValue;
  } else if(type==LIB) {
    //should call function pointer here...

  } else if(type==PING) {
    //Parallax ping based on digital pulseIn
    pinMode(port, OUTPUT);
    digitalWrite(port, LOW);
    delayMicroseconds(2);
    digitalWrite(port, HIGH);
    delayMicroseconds(5);
    digitalWrite(port, LOW);

    pinMode(port, INPUT);
    //long duration1 = pulseIn(port, HIGH);
    //long cm1 =  duration1 / 29 /2;
    //Serial.print("Duration: ");
    //Serial.println(duration1);
    //Serial.print("CM : ");
    //Serial.println(cm1);
    state = pulseIn(port, HIGH);
    return getValue();
  } 
  else return "\0";
}
