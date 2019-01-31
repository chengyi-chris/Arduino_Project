#include <Servo.h> 
int pinLF=17;     //  左前
int pinLB=16;     //  左後

int pinRF=15;    //  右前
int pinRB=14;    //  右後

int PWMEN1=5;   //左動力
int PWMEN2=3;   //右動力

const int SensorLeft=6;
const int SensorRight=2;
int lastSL;//上次左感測器狀態
int lastSR;//上次右感測器狀態

void setup()
 {
  Serial.begin(9600);     // 定義馬達輸出腳位 
  pinMode(pinLB,OUTPUT); // 腳位 6 
  pinMode(pinLF,OUTPUT); // 腳位 9 
  pinMode(pinRB,OUTPUT); // 腳位 10 
  pinMode(pinRF,OUTPUT); // 腳位 11 
  
  pinMode(PWMEN1,OUTPUT); // 腳位 3 (PWM) 
  pinMode(PWMEN2,OUTPUT); // 腳位 5 (PWM)

 }
 
//#####################################################
 
void Lback(int power){                   //左退
  digitalWrite(pinLF,HIGH);  
  digitalWrite(pinLB,LOW);
            analogWrite(PWMEN1,power);  //power:0~250
            analogWrite(pinRB, 0);
}
void Lgo(int power){                       //左進
  digitalWrite(pinLF,LOW);  
  digitalWrite(pinLB,HIGH);
             analogWrite(PWMEN1,power);
             analogWrite(pinRF, 0);
}
void Rgo(int power){
  digitalWrite(pinRF,HIGH);   
  digitalWrite(pinRB,LOW);
             analogWrite(pinRB,0);
             analogWrite(PWMEN2,power);
}
void Rback(int power){
  digitalWrite(pinRF,LOW);   
  digitalWrite(pinRB,HIGH);
             analogWrite(pinRF,0);
             analogWrite(PWMEN2,power);
}
void stopp()         //停止
    {
     digitalWrite(pinRB,LOW);
     digitalWrite(pinRF,LOW);
     digitalWrite(pinLB,LOW);
     digitalWrite(pinLF,LOW);
    }

//################################################//

void advance(int power){
  Lgo(power);
  Rgo(power);
}
void back(int power){
  Lback(power);
  Rback(power);
}
void turnL(int power){
  Lback(power);
  Rgo(power);
}
void turnR(int power){
  Lgo(power);
  Rback(power);
}

//#######################################################//
int n;
void loop()
{
int nowSL=digitalRead(SensorLeft); //LOW表示白紙有反射
int nowSR=digitalRead(SensorRight); //HIGH表示黑紙吸收

   if(nowSL==LOW&&nowSR==LOW{//線右偏
	turnR(240);
	}else if(nowSL==LOW && nowSR==HIGH){//直走
	advance(100);
	}else if(nowSL==HIGH && nowSR==LOW){ //線左偏
	turnL(240);
	}else if(nowSL==HIGH && nowSR==HIGH){ //全黑
	advance(100);
	}
	lastSL=nowSL;
	lastSR=nowSR;
	delay(50);
	}
