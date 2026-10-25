// C++ code
// Project by Paco Mombano & Carpento
// C++ code
// 0° =500
// 45° = 1000
// 90° = 1500
// 180° = 2500
#define TRANSISTOR1_PIN 4
#define DOOR_SERVO1_PIN 6
#define DOOR_SERVO2_PIN 9
#define LOCK_SERVO1_PIN 3
#define PHOTOTRANSISTOR1_PIN A0
#define PHOTOTRANSISTOR2_PIN A1
#define RELAY1_PIN 2
#define TEMPERATURE_SENSOR1_PIN A2
#define BUTTON1_PIN 5
#define BUZZER1_PIN 7


const int totalTime=20000; //durata totale del duty cycle
const float timePerAngle=11.11;
const int intercept = 500;
bool SERVO_STATE = 0;//stop
bool BUZZER_STATE = 0;
bool BUTTON_STATE = 0;
bool FAN_STATE = 0;
void setup()
{
  pinMode(DOOR_SERVO1_PIN, OUTPUT);
  pinMode(DOOR_SERVO2_PIN, OUTPUT);
  pinMode(LOCK_SERVO1_PIN, OUTPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(PHOTOTRANSISTOR1_PIN, INPUT_PULLUP);
  pinMode(PHOTOTRANSISTOR2_PIN, INPUT_PULLUP);
  pinMode(TRANSISTOR1_PIN, OUTPUT);
  pinMode(BUTTON1_PIN, INPUT);
  pinMode(TEMPERATURE_SENSOR1_PIN, INPUT);
  Serial.begin(9600);
}

//Funzione di apertura serratura della porta
void serraturaOpen(){
  for(int cont=0;cont<=90;cont++){
    digitalWrite(LOCK_SERVO1_PIN, HIGH);
    delayMicroseconds(timePerAngle*cont+intercept);
    digitalWrite(LOCK_SERVO1_PIN, LOW);
    delayMicroseconds(totalTime-(timePerAngle*cont+intercept));
    delay(30);//Rallentatore
  }
}

//Funzione di chiusura serratura della serratura
void serraturaClose(){
  digitalWrite(RELAY1_PIN, LOW);
  for(int cont=0;cont<=90;cont++){
    digitalWrite(LOCK_SERVO1_PIN, HIGH);
    delayMicroseconds(1000-timePerAngle*cont+intercept);
    digitalWrite(LOCK_SERVO1_PIN, LOW);
    delayMicroseconds(totalTime-(1000-timePerAngle*cont+intercept));
    delay(30);//Rallentatore
  }
  digitalWrite(RELAY1_PIN, HIGH);
}


//Funzione di apertura contemporanea dei due servo
void apertura(){
  serraturaOpen();
  digitalWrite(RELAY1_PIN, HIGH);
  for(int cont=0;cont<=90;cont++){
    digitalWrite(DOOR_SERVO1_PIN, HIGH);
    digitalWrite(DOOR_SERVO2_PIN, HIGH);
    delayMicroseconds(timePerAngle*cont+intercept);
    digitalWrite(DOOR_SERVO1_PIN, LOW);
    digitalWrite(DOOR_SERVO2_PIN, LOW);
    delayMicroseconds(totalTime-(timePerAngle*cont+intercept));
    delay(30);//Rallentatore
  }
  SERVO_STATE=1;
  digitalWrite(RELAY1_PIN, LOW);
}

//Funzione di chiusura contemporanea dei due servo
void chiusura(){
  digitalWrite(RELAY1_PIN, HIGH);
  for(int cont=0;cont<=90;cont++){
    digitalWrite(DOOR_SERVO1_PIN, HIGH);
    digitalWrite(DOOR_SERVO2_PIN, HIGH);
    delayMicroseconds(1000-timePerAngle*cont+intercept);
    digitalWrite(DOOR_SERVO1_PIN, LOW);
    digitalWrite(DOOR_SERVO2_PIN, LOW);
    delayMicroseconds(totalTime-(1000-timePerAngle*cont+intercept));
    delay(30);//Rallentatore
  }
  SERVO_STATE=0;
  digitalWrite(RELAY1_PIN, LOW);
  serraturaClose();
}
//FUNCTIONS FOR TEMPERATURE SENSOR
double readTemperature() {
 double voltage = analogRead(TEMPERATURE_SENSOR1_PIN) * (5.0 / 1024.0);
 return (voltage - 0.5) * 100;
}

// FUNCTION FOR ALARM
bool activateBuzzer() {
	digitalWrite(BUZZER1_PIN, HIGH);
    BUZZER_STATE=1;
}
bool deactivateBuzzer() {
	digitalWrite(BUZZER1_PIN, LOW);
    BUZZER_STATE=0;
}

//Esecuzione ciclica del codice
void loop(){
  double tempIn = readTemperature();
  while (tempIn<-5.0) {
    if (tempIn<-5.0) {
      tempIn = readTemperature();
    }
  }

  if (tempIn>35.0) {
  	digitalWrite(TRANSISTOR1_PIN,HIGH);
    FAN_STATE = 1;
  } else {
    if (FAN_STATE) {
      digitalWrite(TRANSISTOR1_PIN,LOW);
      FAN_STATE = 0;
    }
  }
  
  if (BUZZER_STATE&&digitalRead(BUTTON1_PIN)==1) {
  	deactivateBuzzer();
  }
  float fotoAnalog1 = analogRead(PHOTOTRANSISTOR1_PIN);
  float fotoAnalog2 = analogRead(PHOTOTRANSISTOR2_PIN);
  float fotoAnalog = (fotoAnalog1+fotoAnalog2)/2;//Media dei valori misurati dai fototransistor
  float foto = map(fotoAnalog, 101, 1006, 0, 500);//Proporzione per cambio del range
  float incertezza = map((abs((fotoAnalog1-fotoAnalog2)/2)), 0, 1023, 0,5);//Calcolo dell'incertezza assoluta
  Serial.println("Incertezza:");
  Serial.println(incertezza);
  if(incertezza>1.5){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
  float sole = foto/100; //Valore in Volt
  Serial.println("Valore ambientale (0-5V):");
  Serial.println(sole);
  Serial.println("stop:");
  Serial.println(SERVO_STATE);
  delay(1000);
  
  if(sole<=4.80){
    if(SERVO_STATE==0){
      apertura();
  	}if(SERVO_STATE==1){
      digitalWrite(RELAY1_PIN, LOW);
      digitalWrite(DOOR_SERVO1_PIN, LOW);
      digitalWrite(DOOR_SERVO2_PIN, LOW);
      digitalWrite(LOCK_SERVO1_PIN, LOW);
  	}
  }if(sole>4.80){
    if(SERVO_STATE==1){
      chiusura();
    }if(SERVO_STATE==0){
      digitalWrite(RELAY1_PIN, LOW);
      digitalWrite(DOOR_SERVO1_PIN, LOW);
      digitalWrite(DOOR_SERVO2_PIN, LOW);
      digitalWrite(LOCK_SERVO1_PIN, LOW);
    }
  }
}
