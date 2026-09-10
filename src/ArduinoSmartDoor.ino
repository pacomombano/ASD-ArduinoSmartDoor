// C++ code
// Project by Paco Mombano
// C++ code
// 0° =500
// 45° = 1000
// 90° = 1500
// 180° = 2500

#define servo1 6
#define servo2 9
#define servo3 3
#define fotoPin A0
#define fotoPin2 A1
#define pinRele 2

const int tempoTot = 20000; //durata totale del duty cycle
const float tGrado=11.11;
int intercetta = 500;
bool stop = 0;
void setup()
{
  pinMode(servo1, OUTPUT);
  pinMode(servo2, OUTPUT);
  pinMode(servo3, OUTPUT);
  pinMode(pinRele, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(fotoPin, INPUT_PULLUP);
  pinMode(fotoPin2, INPUT_PULLUP);
  Serial.begin(9600);
}

//Funzione di apertura della porta
void serraturaOpen(){
  for(int cont=0;cont<=90;cont++){
    digitalWrite(servo3, HIGH);
    delayMicroseconds(tGrado*cont+intercetta);
    digitalWrite(servo3, LOW);
    delayMicroseconds(tempoTot-(tGrado*cont+intercetta));
    delay(30);//Rallentatore
  }
}

//Funzione di chiusura della serratura
void serraturaClose(){
  digitalWrite(pinRele, LOW);
  for(int cont=0;cont<=90;cont++){
    digitalWrite(servo3, HIGH);
    delayMicroseconds(1000-tGrado*cont+intercetta);
    digitalWrite(servo3, LOW);
    delayMicroseconds(tempoTot-(1000-tGrado*cont+intercetta));
    delay(30);//Rallentatore
  }
  digitalWrite(pinRele, HIGH);
}


//Funzione di apertura contemporanea dei due servo
void apertura(){
  serraturaOpen();
  digitalWrite(pinRele, HIGH);
  for(int cont=0;cont<=90;cont++){
    digitalWrite(servo1, HIGH);
    digitalWrite(servo2, HIGH);
    delayMicroseconds(tGrado*cont+intercetta);
    digitalWrite(servo1, LOW);
    digitalWrite(servo2, LOW);
    delayMicroseconds(tempoTot-(tGrado*cont+intercetta));
    delay(30);//Rallentatore
  }
  stop=1;
  digitalWrite(pinRele, LOW);
}

//Funzione di chiusura contemporanea dei due servo
void chiusura(){
  digitalWrite(pinRele, HIGH);
  for(int cont=0;cont<=90;cont++){
    digitalWrite(servo1, HIGH);
    digitalWrite(servo2, HIGH);
    delayMicroseconds(1000-tGrado*cont+intercetta);
    digitalWrite(servo1, LOW);
    digitalWrite(servo2, LOW);
    delayMicroseconds(tempoTot-(1000-tGrado*cont+intercetta));
    delay(30);//Rallentatore
  }
  stop=0;
  digitalWrite(pinRele, LOW);
  serraturaClose();
}

//Esecuzione ciclica del codice
void loop(){
  float fotoAnalog1 = analogRead(fotoPin);
  float fotoAnalog2 = analogRead(fotoPin2);
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
  Serial.println(stop);
  delay(1000);
  
  if(sole>=3.5){
    if(stop==0){
      apertura();
  	}if(stop==1){
      digitalWrite(pinRele, LOW);
      digitalWrite(servo1, LOW);
      digitalWrite(servo2, LOW);
      digitalWrite(servo3, LOW);
  	}
  }if(sole<3.5){
    if(stop==1){
      chiusura();
    }if(stop==0){
      digitalWrite(pinRele, LOW);
      digitalWrite(servo1, LOW);
      digitalWrite(servo2, LOW);
      digitalWrite(servo3, LOW);
    }
  }
}
