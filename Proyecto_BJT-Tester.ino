#include <LiquidCrystal.h>
#include "GFButton.h"
LiquidCrystal lcd(12,11,5,4,3,2);

float V1=0;
float V2=0;
float VC=0;
float VB=0;
float VE=0;
float IC=0;
float IB=0;
float HFE=0;
float VCE1=0;
float VCE2=0;
float IC2=0;
float VRC=0;
float ro=0;
float VAF=0;
int tipo=0;
int C=0;
int aux=0;
GFButton boton(9);

//funciones

int NPN_PNP();
int Terminales(int tipo);
void BETA();
void Voltaje_Early(int tipo);


void setup() {
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Bienvenido a ");
  lcd.setCursor(0, 1);
  lcd.print("BJT-Tester ");
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  //pinMode(A0,INPUT);
  //pinMode(A1,INPUT);
  Serial.begin(9600);
  digitalWrite(6,LOW);//COLECTOR
  digitalWrite(7,HIGH);//BASE
  digitalWrite(8,LOW);//EMISOR
  delay(3000);
  lcd.clear();
}

void loop() {
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("INSERTE EL BJT");
  lcd.setCursor(0,1);
  lcd.print("PULSE EL BOTON");
  //delay(10);
  //Serial.println(analogRead(A1));
  if(boton.isPressed()){
    lcd.clear();
    tipo=NPN_PNP();
    Serial.println(tipo);
    C=Terminales(tipo);
    BETA();
    Voltaje_Early(tipo);
  }

}

int NPN_PNP(){
  lcd.println("EL BJT ES");
  digitalWrite(6,LOW);//COLECTOR
  digitalWrite(7,HIGH);//BASE
  digitalWrite(8,LOW);//EMISOR
  delay(200);
  V1=analogRead(A0);
  VC=((V1*5)/1023)*1000;
  V2=analogRead(A2);
  VE=((V2*5)/1023)*1000;
  Serial.println(analogRead(A2));
  Serial.println(V2);
  if( VC > 0 || VE > 0 ){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("EL BJT ES NPN ");
    lcd.setCursor(0,1);
    delay(200);
    lcd.println("PULSE EL BOTON ");
    do{
      if(boton.isPressed()){
        aux=1;
      }else{
        aux= 0;
      }
    }while(aux==0); 
    return 1;
  }else if ((VC && VE)<0.5){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.println("EL BJT ES PNP");
    delay(200);
    lcd.setCursor(0,1);
    lcd.println("PULSE EL BOTON ");
    do{
      if(boton.isPressed()){
        aux=1;
      }else{
        aux= 0;
      }
    }while(aux==0);
    return 2;
  }else {
    lcd.clear();
    lcd.println("ERROR");
    do{
      if(boton.isPressed()){
        aux=1;
      }else{
        aux= 0;
      }
    }while(aux==0);
    return 3;
  }
}


int Terminales(int tipo){
  if(tipo==1){
    Serial.println("OKA");
    digitalWrite(6,HIGH);//COLECTOR
    digitalWrite(7,HIGH);//BASE
    digitalWrite(8,LOW);//EMISOR
    delay(200);
    V1=analogRead(A0);
    VC=(V1*5)/1023;
    IC=((5-VC)/2200)*1000;
    //Serial.println(IC);
    if(IC <= 2.31 && IC >= 2){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("B:2 C:1 E:3");
      lcd.setCursor(0,1);
      lcd.print("BJT BIEN POLARIZADO");
      do{
        if (boton.isPressed()){
          aux=1;
        }else{
          aux=0;
        }
        //Serial.println(IC);
      }while(aux==0);
      return 0;     
    }else if(IC < 1.5 || IC > 2.31){
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("B:2 C:3 E:1");
      lcd.setCursor(0,2);
      lcd.print("INVERTIR EL BJT");
      do{
        if (boton.isPressed()){
          aux=1;
        }else{
          aux=0;
        }
       // Serial.println(IC);
      }while(aux==0);
      return 1;
    }else{
      lcd.setCursor(0,1);
      lcd.print("ERROR, REVISAR");
      return 1;
    }

  }else if(tipo==2){
    digitalWrite(6,HIGH);//COLECTOR
    digitalWrite(7,LOW);//BASE
    digitalWrite(8,LOW);//EMISOR
    delay(200);
    V1=analogRead(A0);
    VC=(V1*5)/1023;
    IC=((5-VC)/2200)*1000;
    Serial.println(VC);
    if(VC <= 5 && VC >= 3){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("B:2 C:1 E: 3");
      lcd.setCursor(0,1);
      lcd.print("BJT BIEN POLARIZADO");
      delay(200);
      do{
        if (boton.isPressed()){
          aux=1;
        }else{
          aux=0;
        }
       // Serial.println(IC);
      }while(aux==0);
      return 0;
    }else if(IC < 2.22 || IC > 2.31){
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("B:2 C:3 E:1");
      lcd.setCursor(0,1);
      lcd.print("INVERTIR EL BJT");
      delay(2000);
      do{
        if (boton.isPressed()){
          aux=1;
        }else{
          aux=0;
        }
       // Serial.println(IC);
      }while(aux==0);
      return 1;
    }else{
      lcd.setCursor(0,1);
      lcd.print("ERROR, REVISAR");
      delay(200);
      do{
        if (boton.isPressed()){
          aux=1;
        }else{
          aux=0;
        }
       // Serial.println(IC);
      }while(aux==0);
      return 1;
    }
  }else if(tipo==3){
    lcd.setCursor(0,1);
    lcd.print("ERROR, REVISAR");
    delay(200);
    do{
        if (boton.isPressed()){
          aux=1;
        }else{
          aux=0;
        }
       // Serial.println(IC);
      }while(aux==0);
    return 1;
  }else{
    lcd.setCursor(0,1);
    lcd.print("ERROR, REVISAR");
    delay(200);
    do{
        if (boton.isPressed()){
          aux=1;
        }else{
          aux=0;
        }
       // Serial.println(IC);
      }while(aux==0);
    return 1;
  }
}

void BETA(){
  Serial.println("BETA");
  delay(200);
  V1=analogRead(A0);
  VC=(V1*5)/1023;
  V2=analogRead(A1);
  VB=(V2*5)/1023;
  IC=((5-VC)/2200)*1000;
  IB=((5-VB)/100000)*1000;
  HFE=IC/IB;
  //Serial.println(IC);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HFE=");
  lcd.print(HFE);
  lcd.setCursor(0,1);
  do{
    if(boton.isPressed()){
      aux=1;
    }else{
      aux=0;
    }
  }while(aux==0);
}

void Voltaje_Early(int tipo){
  if(tipo==1){
    digitalWrite(6,HIGH);//COLECTOR
    digitalWrite(7,HIGH);//BASE
    digitalWrite(8,LOW);//EMISOR
    delay(200);
    V1=analogRead(A0);
    VC=(V1*5)/1023;
    IC=((5-VC)/2200);
    Serial.println(VC);
    lcd.setCursor(0,0);
    lcd.print("VCE MENOR QUE 1");
    do{
      V1=analogRead(A0);
      VC=(V1*5)/1023;
      V2=analogRead(A3);
      VRC=(V2*5)/1023;
      IC=((VRC-VC)/2200)*1000;
      Serial.println(IC);
      lcd.setCursor(0,1);
      lcd.print(VC);
      if(boton.isPressed()){
        aux=1;
      }else{
        aux=0;
      }
    }while(aux==0);
    IC=((VRC-VC)/2200);
    VCE1=VC;
    delay(200);
    lcd.setCursor(0,0);
    lcd.print("VCE MAYOR QUE 2");
    Serial.println(IC);
    do{
      V1=analogRead(A0);
      VC=(V1*5)/1023;
      V2=analogRead(A3);
      VRC=(V2*5)/1023;
      IC=((VRC-VC)/2200)*1000;
      Serial.println(IC);
      lcd.setCursor(0,1);
      lcd.print(VC);
      if(boton.isPressed()){
        aux=1;
      }else{
        aux=0;
      }
    }while(aux==0);
    IC2=((VRC-VC)/2200);
    VCE2=VC;
    if(VCE2>VCE1){
        ro=((VCE2-VCE1)/(IC2-IC));
        ro=ro*10;
        Serial.println(ro);
    }
    else{
        ro=((VCE1-VCE2)/(IC-IC2));
        ro=ro*10;
        Serial.println(ro);
    }
    VAF=(IC*ro)-VCE1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Voltaje Early");
    lcd.setCursor(0,1);
    lcd.print(VAF);
    Serial.println(ro);
    Serial.println(IC);
    delay(300);
    do{
      if(boton.isPressed()){
        aux=1;
      }else{
       aux=0;
      }
    }while(aux==0);


  }else if(tipo==2){//Inicia el tipo pnp
    digitalWrite(6,LOW);//COLECTOR
    digitalWrite(7,HIGH);//BASE
    digitalWrite(8,HIGH);//EMISOR
    delay(200);
    V1=analogRead(A0);
    VC=(V1*5)/1023;
    IC=((5-VC)/2200)*1000;
    Serial.println(VC);
    lcd.setCursor(0,0);
    lcd.print("VCE MENOR QUE 1");
    do{
      V1=analogRead(A0);
      VC=(V1*5)/1023;
      V2=analogRead(A3);
      VRC=(V2*5)/1023;
      lcd.setCursor(0,1);
      lcd.print(VC);
      if(boton.isPressed()){
        aux=1;
      }else{
        aux=0;
      }
    }while(aux==0);
    IC=((VRC-VC)/2200);
    VCE1=VC;
    delay(200);
    lcd.setCursor(0,0);
    lcd.print("VCE MAYOR QUE 2");
    do{
      V1=analogRead(A0);
      VC=(V1*5)/1023;
      V2=analogRead(A3);
      VRC=(V2*5)/1023;
      lcd.setCursor(0,1);
      lcd.print(VC);
      if(boton.isPressed()){
        aux=1;
      }else{
        aux=0;
      }
    }while(aux==0);
    IC2=((VRC-VC)/2200);
    VCE2=VC;
    if(VCE2>VCE1){
        ro=((VCE2-VCE1)/(IC2-IC));
    }
    else{
        ro=((VCE1-VCE2)/(IC-IC2));
    }
    VAF=(IC*ro)-VCE1;//(IC*ro)-VCE1;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Voltaje Early");
    lcd.setCursor(0,1);
    lcd.print(VAF);
    delay(300);
    do{
      if(boton.isPressed()){
        aux=1;
      }else{
       aux=0;
      }
    }while(aux==0);
  }

}
