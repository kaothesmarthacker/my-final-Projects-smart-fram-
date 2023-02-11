
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLhRs-XSkm"
#define BLYNK_DEVICE_NAME "smart farmzaza007"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include "BlynkEdgent.h"

int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd_1(0x27, lcdColumns, lcdRows);  
const int Button_pinA = 14; //เดินหน้า
const int Button_pinB = 35; //off/on
const int Button_pinC = 32; //ถอยหลัง
int relay1 = 25; //pump
int relay2; 
int relay6 = 26; //light
int relay3 = 13; //fans
int soil = 33;
int lastbuttonstateA;
int lastbuttonstateB;
int lastbuttonstateC;
int lastbuttonstateD;
int nowbuttonstateA;
int nowbuttonstateB;
int nowbuttonstateC;
int nowbuttonstateD;
int menu = 1;
int menustateA = 0;
int menustateB = 0;
int menustateC = 0;
int menustateD = 0;
int connectbuttonpump;
int soilPer;
int h;
int t;
int auto_soil = 50 ;
int auto_hum = 50 ;
int auto_tem = 35 ;
#define DHT11_PIN 27
#define DHTTYPE DHT11
DHT dht(DHT11_PIN, DHTTYPE);

BlynkTimer timer;
#define INTERVAL 1000L

BLYNK_CONNECTED() {
    Blynk.syncAll();
}

void automode(){
  if(menustateD ==1){
    if (soilPer > auto_soil) { //50
    digitalWrite(relay1, HIGH);
    Blynk.virtualWrite(V1, 0);
  } else {
    digitalWrite(relay1, LOW);
    Blynk.virtualWrite(V1, 1);
  }
  if (h < auto_hum) {  //50
    digitalWrite(relay2, HIGH);
    Blynk.virtualWrite(V5, 0);
  } else {
    digitalWrite(relay2, LOW);
    Blynk.virtualWrite(V5, 1);
  }
  if (t < auto_tem) { //35
    digitalWrite(relay3, HIGH);
    Blynk.virtualWrite(V0, 0);
  } else {
    digitalWrite(relay3, LOW);
    Blynk.virtualWrite(V0, 1);
  }
}
}

void automode_off(){
  if(menustateD ==0){
  if(menustateA == 0){
        digitalWrite(relay1,HIGH);
        Blynk.virtualWrite(V1, 0);
        menustateA = 0;}else{
        digitalWrite(relay1,LOW);
       Blynk.virtualWrite(V1, 1);
       menustateA = 1;}
  if(menustateB == 0){
        digitalWrite(relay2,HIGH);
        Blynk.virtualWrite(V5, 0);
        menustateB = 0;}else{
        digitalWrite(relay2,LOW);
        Blynk.virtualWrite(V5, 1);
        menustateB = 1;}
  if(menustateC == 0){
        digitalWrite(relay3,HIGH);
        Blynk.virtualWrite(V0, 0);
        menustateC = 0;}else{
        digitalWrite(relay3,LOW);
        Blynk.virtualWrite(V0, 1);
        menustateC = 1;}
 }
 }

void setup()
{
  lcd_1.init();                     
  lcd_1.backlight();
  lcd_1.begin(16, 2);
  lcd_1.setCursor(0, 0);
  lcd_1.print("System Start");
  lcd_1.setCursor(0, 1);
  lcd_1.print("smart farm");
  pinMode(Button_pinA,INPUT_PULLUP);
  pinMode(Button_pinB,INPUT_PULLUP);
  pinMode(Button_pinC,INPUT_PULLUP);
  nowbuttonstateA = digitalRead(Button_pinA);
  nowbuttonstateB = digitalRead(Button_pinB);
  nowbuttonstateC = digitalRead(Button_pinC);
  delay(500);lcd_1.clear();  lcd_1.setCursor(0, 0);
  lcd_1.print("smart farm");
  lcd_1.setCursor(0, 1);
  lcd_1.print("Mode1 : Pump OFF");
  pinMode(relay1, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay3, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay6, HIGH);
  digitalWrite(relay3, HIGH);
  pinMode(soil,INPUT);
  Serial.begin(115200);
  DHT dht(DHT11_PIN, DHTTYPE);
  pinMode(DHT11_PIN,INPUT);
  Blynk.virtualWrite(V0, 0);
  Blynk.virtualWrite(V1, 0);
  Blynk.virtualWrite(V5, 0);
  dht.begin();
  BlynkEdgent.begin();
}


BLYNK_WRITE(V0) //light
{
  if(param.asInt() == 1)
  {
    digitalWrite(relay3,LOW); 
    menustateC = 1;
  }
  else
  {
    digitalWrite(relay3,HIGH);
    menustateC = 0;
  }
  
}

BLYNK_WRITE(V1) //pump
{
  if(param.asInt() == 1)
  {
    digitalWrite(relay1,LOW);
    menustateA = 1;
  }
  else
  {
    digitalWrite(relay1,HIGH); 
    menustateA = 0;
  }
  
}

BLYNK_WRITE(V5) //fan
{
  if(param.asInt() == 1)
  {
    digitalWrite(relay2,LOW); 
    menustateB = 1;
  }
  else
  {
    digitalWrite(relay2,HIGH);
    menustateB = 0;
  }
}

BLYNK_WRITE(V7){ //humidity
  lcd_1.clear();
  auto_hum = param.asInt();
  lcd_1.setCursor(0,0);
  lcd_1.print("Automode setting");
  lcd_1.setCursor(0,1);
  lcd_1.print("Humidity:");
  char buff_hum[4];
  sprintf(buff_hum, "%3d", auto_hum);
  lcd_1.print(buff_hum);
  lcd_1.print("%");
  delay(1500);
}

BLYNK_WRITE(V6){ //Temperature
  auto_tem = param.asInt();
  lcd_1.setCursor(0,0);
  lcd_1.print("Automode setting");
  lcd_1.setCursor(0,1);
  lcd_1.print("Temperature:");
  char buff_tem[4];
  sprintf(buff_tem, "%3d", auto_tem);
  lcd_1.print(buff_tem);
  lcd_1.print("c");
  delay(1500);
}

BLYNK_WRITE(V8){ //Soil
  auto_soil = param.asInt();
  lcd_1.setCursor(0,0);
  lcd_1.print("Automode setting");
  lcd_1.setCursor(0,1);
  lcd_1.print("Soil:");
  char buff_soil[4];
  sprintf(buff_soil, "%3d", auto_soil);
  lcd_1.print(buff_soil);
  lcd_1.print("%       ");
  delay(1500);
}

BLYNK_WRITE(V9){
  if(param.asInt() == 1)
  {
  menu = 4;
  menustateD = 1;
  }else{
  menustateD = 0;
  }
}

void loop() {
h = dht.readHumidity();
   t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));

  BlynkEdgent.run();
  timer.run();
  int soilVal = analogRead(soil); //รับค่าความชื้น
  soilPer = map(soilVal,900,4095,100,0); //แปลงค่าความชื้นเป็น%
  Blynk.virtualWrite(V2, soilPer);
  Blynk.virtualWrite(V3, h);
  Blynk.virtualWrite(V4, t);
  Serial.print("soilPer = ");  
  Serial.print(soilPer);
  Serial.print(" ");
  Serial.println(soilVal);
  delay(100);

lastbuttonstateA = nowbuttonstateA;//ปุ่มA
nowbuttonstateA = digitalRead(Button_pinA);
  if(lastbuttonstateA == HIGH && nowbuttonstateA == LOW){
  	Serial.println("ButtonA is Pressed");
    menu = menu + 1; 
    if (menu > 4){menu = 1;}
    if (menu < 1){menu = 4;}
    if (menu == 1){
  		lcd_1.setCursor(0,0); 
    	lcd_1.setCursor(0,1); lcd_1.print("Mode1 : Pump");
      if(menustateA == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
        digitalWrite(relay1,HIGH);
        Blynk.virtualWrite(V1, 0);
        menustateD = 0;
        automode_off();
        Blynk.virtualWrite(V9, 0);}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
        digitalWrite(relay1,LOW);
        Blynk.virtualWrite(V1, 1);}}
    Serial.println(menu);
    if (menu == 2){
  		lcd_1.setCursor(0,0); 
    	lcd_1.setCursor(0,1); lcd_1.print("Mode2 : Fans    ");
      if(menustateB == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
    	digitalWrite(relay2,HIGH);
      Blynk.virtualWrite(V5, 0);}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
        digitalWrite(relay2,LOW);
      Blynk.virtualWrite(V5, 1);}}
  	Serial.println(menu);
    if (menu == 3){
  		lcd_1.setCursor(0,0); 
    	lcd_1.setCursor(0,1); lcd_1.print("Mode3 :Light    ");
      if(menustateC == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
    	digitalWrite(relay3,HIGH);
      Blynk.virtualWrite(V0, 0);}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      digitalWrite(relay3,LOW);
      Blynk.virtualWrite(V0, 1);}}
  	Serial.println(menu);
    if (menu == 4){
  		lcd_1.setCursor(0,0); 
    	lcd_1.setCursor(0,1); lcd_1.print("Mode4 :Auto    ");
      if(menustateD == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
      automode_off();}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      }}
  	Serial.println(menu);
  }
  lastbuttonstateB = nowbuttonstateB;//ปุ่มB
	nowbuttonstateB = digitalRead(Button_pinB);
  if(lastbuttonstateB == HIGH && nowbuttonstateB == LOW){
  	Serial.println("ButtonB is Pressed");
    	  Serial.println(menu);
  if(menu == 1){
    	menustateA = menustateA + 1;
  		if (menustateA > 1){menustateA = 0;}
    if(menustateA == 0){
    	lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
    digitalWrite(relay1,HIGH);
    Blynk.virtualWrite(V1, 0);}
  	if(menustateA == 1){
    	lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
    digitalWrite(relay1,LOW);
    Blynk.virtualWrite(V1, 1);}
      			Serial.println(menustateA);
  }
  if(menu == 2){
    	menustateB = menustateB + 1;
  		if (menustateB > 1){menustateB = 0;}
    if(menustateB == 0){
    	lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
    digitalWrite(relay6,HIGH);
    Blynk.virtualWrite(V5, 0);}
  
  	if(menustateB == 1){
    	lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
    digitalWrite(relay6,LOW);
    Blynk.virtualWrite(V5, 1);}
          	    Serial.println(menustateB);
  }
  if(menu == 3){
    	menustateC = menustateC + 1;
  		if (menustateC > 1){menustateC = 0;}
    if(menustateC == 0){
    	lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
    digitalWrite(relay3,HIGH);
    Blynk.virtualWrite(V0, 0);}
  
  	if(menustateC == 1){
    	lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
    digitalWrite(relay3,LOW);
    Blynk.virtualWrite(V0, 1);}
         		Serial.println(menustateC);
  }if(menu == 4){
    	menustateD = menustateD + 1;
  		if (menustateD > 1){menustateD = 0;}
    if(menustateD == 0){
    	lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
      automode_off();}
  
  	if(menustateD == 1){
    	lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");}
         		Serial.println(menustateD);
      }
  }
  
  lastbuttonstateC = nowbuttonstateC; //ปุ่มC
	nowbuttonstateC = digitalRead(Button_pinC);
  if(lastbuttonstateC == HIGH && nowbuttonstateC == LOW){
  	Serial.println("ButtonC is Pressed");
    menu = menu - 1; 
    if (menu > 4){menu = 1;}
    if (menu < 1){menu = 4;}
    if (menu == 1){
  		lcd_1.setCursor(0,0); 
    	lcd_1.setCursor(0,1); lcd_1.print("Mode1 : Pump");
      if(menustateA == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
      digitalWrite(relay1,HIGH);
      Blynk.virtualWrite(V1, 0);}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      digitalWrite(relay1,LOW);
      Blynk.virtualWrite(V1, 1);}}
    Serial.println(menu);
    if (menu == 2){
  		lcd_1.setCursor(0,0); 
    	lcd_1.setCursor(0,1); lcd_1.print("Mode2 : Fans");
      if(menustateB == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
    	digitalWrite(relay2,HIGH);
      Blynk.virtualWrite(V5, 0);}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      digitalWrite(relay2,LOW);}
      Blynk.virtualWrite(V5, 1);}
  	Serial.println(menu);
    if (menu == 3){
  		lcd_1.setCursor(0,0); 
    	lcd_1.setCursor(0,1); lcd_1.print("Mode3 :Light    ");
      if(menustateC == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");
    	digitalWrite(relay3,HIGH);
      Blynk.virtualWrite(V0, 0);
      menustateD = 0;
      automode_off();
      Blynk.virtualWrite(V9, 0);}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      digitalWrite(relay3,LOW);
      Blynk.virtualWrite(V0, 1);}}
  	Serial.println(menu);
        if (menu == 4){
  		lcd_1.setCursor(0,0);
    	lcd_1.setCursor(0,1); lcd_1.print("Mode4 :Auto    ");
      if(menustateD == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");}}
  	Serial.println(menu);
  }
  if (menu == 4 && menustateD == 1){    
     Serial.println("Auto on");
     automode();
     Blynk.virtualWrite(V9,1);}
  if (menu == 4 && menustateD == 0){    
     automode_off();
     Blynk.virtualWrite(V9,0);}
  lcd_print();
}

void lcd_print(){
  lcd_1.setCursor(0,0);
  lcd_1.print(F("H:"));
  lcd_1.print(h);
  lcd_1.print(F("%T:"));
  lcd_1.print(t);
  lcd_1.print("cS:");
  lcd_1.setCursor(12,0);
  char buff[4];
  sprintf(buff, "%3d", soilPer);
  lcd_1.print(buff);
  lcd_1.print("%");

  if(menu ==1){
    lcd_1.setCursor(0,0);
    	lcd_1.setCursor(0,1); lcd_1.print("Mode1 : Pump    ");
      if(menustateA == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      }
  }
  if(menu ==2){
    lcd_1.setCursor(0,0);
    	lcd_1.setCursor(0,1); lcd_1.print("Mode2 : Fans    ");
      if(menustateB == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      }
  }
  if(menu ==3){
    lcd_1.setCursor(0,0);
    	lcd_1.setCursor(0,1); lcd_1.print("Mode3 : Light    ");
      if(menustateC == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      }
  }
  if(menu ==4){
  		lcd_1.setCursor(0,0);
    	lcd_1.setCursor(0,1); lcd_1.print("Mode4 :Auto    ");
      if(menustateD == 0){lcd_1.setCursor(13,1);
  		lcd_1.print("OFF");}else{lcd_1.setCursor(13,1);
  		lcd_1.print(" ON");
      }
  }
}
  
