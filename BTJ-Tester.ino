#include <LiquidCrystal.h>
#include "GFButton.h"
LiquidCrystal lcd(12,11,5,4,3,2);

GFButton boton(9);
GFButton boton2(13);

//funciones

// Pines de Arduino para las pruebas
#define Pin_Colector 6
#define Pin_Base 7
#define Pin_Emisor 8

#define LEC_BASE A1
#define LEC_EMISOR A2
#define LEC_COLECTOR A0

int pines = 0;
String tipo;
String Pin6;
String Pin7;
String Pin8;
int inB;
int inC;
int inE;
float V_Pin6[] = {0,0,0}; // BCE / 678
float V_Pin7[] = {0,0,0};
float V_Pin8[] = {0,0,0};
int BCE[] = {0,0,0};
float Vbe,Vbc;
float IC = 0;
float IB = 0;
float HFE = 0;
float V1=0;
float V2=0;
float VC=0;
float VB=0;
float VE=0;
float VCE, IC1,VAF;

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido a ");
  lcd.setCursor(0, 1);
  lcd.print("BJT-Tester ");

  pinMode(Pin_Base, OUTPUT); // COLECTOR
  pinMode(Pin_Colector, OUTPUT); // BASE
  pinMode(Pin_Emisor, OUTPUT); // EMISOR

  pinMode(LEC_BASE, INPUT);

  delay(1000);

}

void loop() {
  
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INSERTE EL BJT");
  lcd.setCursor(0,1);
  lcd.print("PULSE EL BOTON");
  delay(50);
  
  if(boton.isPressed()){
    lcd.clear();
    int cincos = 0;
    InPin6();
    InPin7();
    InPin8();
    digitalWrite(8, LOW);

    if(V_Pin6[0] > 3 || V_Pin6[1] > 3 || V_Pin6[2] > 3){
      cincos++;
    }
    if(V_Pin7[0] > 3 || V_Pin7[1] > 3 || V_Pin7[2] > 3){
      cincos++;
    }
    if(V_Pin8[0] > 3 || V_Pin8[1] > 3 || V_Pin8[2] > 3){
      cincos++;
    }
    Serial.println(cincos);
    if (cincos > 1){
      tipo = "NPN";
      NPN();
    }
    if(cincos == 1){
      tipo = "PNP";
      PNP();
    }

    delay(500);
    BETA();
    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(tipo);
    lcd.print(" hfe: ");
    lcd.print(HFE);
    Serial.println(tipo);
    lcd.setCursor(0,1);
    lcd.print("1: " + Pin6);
    lcd.print(" 2: " + Pin7);
    lcd.print(" 3: " + Pin8);
    lcd.setCursor(0,1);

    Serial.print("VBE (V): ");
    Serial.println(Vbe); // Imprime el voltaje en milivoltios en el monitor serial
    delay(10000); // Espera un segundo antes de realizar otra lectura
    
    delay(50);
    lcd.setCursor(0,0);
    lcd.print("PULSE EL BOTON2");
    lcd.setCursor(0,1);
    lcd.print("PARA MEDIR VAF");
    delay(1500);
  }

    
  
  if(boton2.isPressed()){
    
    digitalWrite(BCE[0], HIGH);
    digitalWrite(BCE[1], HIGH);
    delay(200);

    if(BCE[0] == 6){
      V1=analogRead(LEC_BASE);
    }else if(BCE[0] == 7){
      V1=analogRead(LEC_COLECTOR);
    }else{
      V1=analogRead(LEC_EMISOR);
    }
    if(BCE[2] == 6){
      V2=analogRead(LEC_BASE);
    }else if(BCE[2] == 7){
      V2=analogRead(LEC_COLECTOR);
    }else{
      V2=analogRead(LEC_EMISOR);
    }
    VC=(V1*5)/1023;
    VE=(V2*5)/1023;
    VCE=VC-VE;
    IC1=((5-VC)/2200)*1000;
    VAF = (VCE)/(IC1);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("VAF: ");
    lcd.print(VAF);
    delay(10000);
  }
}

void InPin6(){
  digitalWrite(6, HIGH); // Activar PB0, pin Colector

    inB = analogRead(LEC_BASE); //PIN 6
    V_Pin6[0] = inB * 5.0 / 1024.0;
    Serial.println(V_Pin6[0]);
    inC = analogRead(LEC_COLECTOR); //PIN 7
    V_Pin6[1] = inC * 5.0 / 1024.0;
    Serial.println(V_Pin6[1]);
    inE = analogRead(LEC_EMISOR); //PIN 8
    V_Pin6[2] = inE * 5.0 / 1024.0;
    Serial.println(V_Pin6[2]);
    delay(500);
}
void InPin7(){
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH); // Activar PB0, pin base

    inB = analogRead(LEC_BASE); //PIN 6
    V_Pin7[0] = inB * 5.0 / 1024.0;
    Serial.println(V_Pin7[0]);
    inC = analogRead(LEC_COLECTOR); //PIN 7
    V_Pin7[1] = inC * 5.0 / 1024.0;
    Serial.println(V_Pin7[1]);
    inE = analogRead(LEC_EMISOR); //PIN 8
    V_Pin7[2] = inE * 5.0 / 1024.0;
    Serial.println(V_Pin7[2]);
    delay(500);
}
void InPin8(){
  digitalWrite(7, LOW);
  delay(100);
  digitalWrite(8, HIGH); // Activar PB0, pin Emisor

    inB = analogRead(LEC_BASE); //PIN 6
    V_Pin8[0] = inB * 5.0 / 1024.0;
    Serial.println(V_Pin8[0]);
    inC = analogRead(LEC_COLECTOR); //PIN 7
    V_Pin8[1] = inC * 5.0 / 1024.0;
    Serial.println(V_Pin8[1]);
    inE = analogRead(LEC_EMISOR); //PIN 8
    V_Pin8[2] = inE * 5.0 / 1024.0;
    Serial.println(V_Pin8[2]);
    delay(500);
}
void NPN(){
  float P[] = {V_Pin6[1],V_Pin7[1],V_Pin8[1]};
  int Pin;

  Serial.println(P[0]);
  if (P[0] > 1 && P[0] < 3){
    Pin = 0;
  }else if(P[1] > 1 && P[1] < 3){
    Pin = 1;
  }else{
    Pin = 2;
  }
  float V1,V2;
  Serial.println(Pin);
  switch(Pin){
    case 0:
      Pin6 = "B";
      V1 = V_Pin6[0] - V_Pin6[1];
      V2 = V_Pin6[0] - V_Pin6[2];
      if(V1>V2){
        Pin7 = "E";
        Pin8 = "C";
        Vbe = V1;
        Vbc = V2;
        BCE[0] = 6;
        BCE[1] = 8;
        BCE[2] = 7;
      }else{
        Pin8 = "E";
        Pin7 = "C";
        Vbe = V2;
        Vbc = V1;
        BCE[0] = 6;
        BCE[1] = 7;
        BCE[2] = 8;
      }
      
    break;

    case 1:
      Pin7 = "B";
      V1 = V_Pin7[1] - V_Pin7[0];
      V2 = V_Pin7[1] - V_Pin7[2];
      if(V1>V2){
        Pin6 = "E";
        Pin8 = "C";
        Vbe = V1;
        Vbc = V2;
        BCE[0] = 7;
        BCE[1] = 8;
        BCE[2] = 6;
      }else{
        Pin8 = "E";
        Pin6 = "C";
        Vbe = V2;
        Vbc = V1;
        BCE[0] = 7;
        BCE[1] = 6;
        BCE[2] = 8;
      }
      
    break;

    case 2:
      Pin8 = "B";
      V1 = V_Pin8[2] - V_Pin8[0];
      V2 = V_Pin8[2] - V_Pin8[1];
      if(V1>V2){
        Pin6 = "E";
        Pin7 = "C";
        Vbe = V1;
        Vbc = V2;
        BCE[0] = 8;
        BCE[1] = 7;
        BCE[2] = 6;
      }else{
        Pin7 = "E";
        Pin6 = "C";
        Vbe = V2;
        Vbc = V1;
        BCE[0] = 8;
        BCE[1] = 6;
        BCE[2] = 7;
      }
    break;
  }
  
}
void PNP(){
float P[] = {V_Pin6[1],V_Pin7[1],V_Pin8[1]};
  int Pin;
  Serial.println(P[0]);
  if (P[0] < 1 || P[0] > 4){
    Pin = 0;
  }else if(P[1] < 1 || P[1] > 4){
    Pin = 1;
  }else{
    Pin = 2;
  }
  bool Ma;
  Serial.println(Pin);
  switch(Pin){
    case 0:
      Pin6 = "B";
      Ma = V_Pin7[0] > V_Pin8[0];
      if (Ma == true){
        Pin7 = "C";
        Pin8 = "E";
        Vbe = V_Pin8[2]-V_Pin8[0];
        Vbc = V_Pin8[1]-V_Pin8[0];
        BCE[0] = 6;
        BCE[1] = 7;
        BCE[2] = 8;
      }else{
        Pin7 = "E";
        Pin8 = "C";
        Vbe = V_Pin7[1]-V_Pin7[0];
        Vbc = V_Pin7[2]-V_Pin7[0];
        BCE[0] = 6;
        BCE[1] = 8;
        BCE[2] = 7;
      }
      
    break;

    case 1:
      Pin7 = "B";
      Ma = V_Pin6[1] > V_Pin8[1];
      if (Ma == true){
        Pin6 = "C";
        Pin8 = "E";
        Vbe = V_Pin8[2]-V_Pin8[1];
        Vbc = V_Pin8[0]-V_Pin8[1];
        BCE[0] = 7;
        BCE[1] = 6;
        BCE[2] = 8;
      }else{
        Pin6 = "E";
        Pin8 = "C";
        Vbe = V_Pin6[0]-V_Pin7[1];
        Vbc = V_Pin6[2]-V_Pin7[1];
        BCE[0] = 7;
        BCE[1] = 8;
        BCE[2] = 6;
      }
    break;

    case 2:
      Pin8 = "B";
      Ma = V_Pin6[2] > V_Pin7[2];
      if (Ma == true){
        Pin6 = "C";
        Pin7 = "E";
        Vbe = V_Pin7[1]-V_Pin7[2];
        Vbc = V_Pin7[0]-V_Pin7[2];
        BCE[0] = 8;
        BCE[1] = 6;
        BCE[2] = 7;
      }else{
        Pin7 = "C";
        Pin6 = "E";
        Vbe = V_Pin6[0]-V_Pin6[2];
        Vbc = V_Pin6[1]-V_Pin6[2];
        BCE[0] = 8;
        BCE[1] = 7;
        BCE[2] = 6;
      }
    break;
  }
}

void BETA(){
  digitalWrite(BCE[0], HIGH);
  digitalWrite(BCE[1], HIGH);
  delay(200);
  if(BCE[0] == 6){
    V1=analogRead(LEC_BASE);
  }else if(BCE[0] == 7){
    V1=analogRead(LEC_COLECTOR);
  }else{
    V1=analogRead(LEC_EMISOR);
  }
  if(BCE[1] == 6){
    V2=analogRead(LEC_BASE);
  }else if(BCE[1] == 7){
    V2=analogRead(LEC_COLECTOR);
  }else{
    V2=analogRead(LEC_EMISOR);
  }
  VC=(V1*5)/1023;
  VB=(V2*5)/1023;
  IC=((5-VC)/2200)*1000;
  IB=((5-VB)/100000)*1000;
  HFE=3*IC/IB;
}
