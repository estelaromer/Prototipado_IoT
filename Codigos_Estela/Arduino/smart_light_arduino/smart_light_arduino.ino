
/**************************************************************************************************************
 *     Programa: smart_light_arduino                                                                          *
 *     Autor: Estela Romero                                                                                   *
 *     Version: 1.0                                                                                           *
 *     Fecha: 07 Julio 2017                                                                                   *
 *     Descripcion: Haciendo uso de la comunicacion serie, el arduino envia tres                              *
 *     posibles estados a la Raspberry Pi en funcion de los niveles leidos de los                             *
 *     sensores de luz y presencia:                                                                           *
 *      - "Dia": hay luz suficiente para que se considere que es de dia                                       *
 *      - "Noche": no hay luz suficiente para que se considere que es de dia y no se detecta presencia        *
 *      - "Presencia": no hay luz suficiente para que se considere que es de dia y se detecta presencia       *
 *     A la vez, se lee por comunicacion serie la orden proveniente de la RPI y que actua sobre dos leds:     *  
 *      - "0" (ningun led encendido) cuando es de dia                                                         *
 *      - "1" (un led encendido) cuando es de noche y no se detecta presencia                                 *
 *      - "2" (dos leds encendidos) cuando es de noche y se detecta presencia                                 *
 **************************************************************************************************************/
 
int sensorLum = 0;
int sensorProx = 4;
int luz;
int presencia;
int ledPIN1 = 2;
int ledPIN2 = 3;
char dato;

void setup() {
  Serial.begin(9600);
  Serial.println("Iniciado");
  pinMode(ledPIN1, OUTPUT);
  digitalWrite(ledPIN1, LOW);
  pinMode(ledPIN2, OUTPUT);
  digitalWrite(ledPIN2, LOW);
  pinMode(sensorProx, INPUT);
  pinMode(sensorLum, INPUT);
}

void loop() {
  luz = analogRead(sensorLum);
  presencia = digitalRead(sensorProx);
  if (luz > 30){
    Serial.println("Dia");
  }
  else{ 
    if (presencia == 1){
      Serial.println("Presencia");
    }
    else{
      Serial.println("Noche"); 
    }    
  }
  delay(10000);

  if(Serial.available()>0){
  
    dato = Serial.read();
    if (dato == '2'){
      digitalWrite(ledPIN1,  HIGH);
      digitalWrite(ledPIN2, HIGH);
    }
    if (dato == '1'){
      digitalWrite(ledPIN1,  HIGH);
      digitalWrite(ledPIN2, LOW);
    }
    if(dato == '0'){
      digitalWrite(ledPIN1, LOW);
      digitalWrite(ledPIN2, LOW);
    }

  }

}
