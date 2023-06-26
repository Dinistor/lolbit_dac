//TODO
//КАЛИБРОВКУ
//КАЛИБРОВОЧНОЕ МЕНЮ? ВВОД КАЛИБРОВОЧНЫХ ЗНАЧЕНИЙ?
//хранение калибровочных значений в EEPROM. тут надо аккуратно, дабы не получилось бесконечного цикла записи.
//для проверки, вместе с записью использовать мигание светодиодом на 13й ноге
//DONE энергосбережение - отключение подсветки через noBacklight() через 5 сек после ввода
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include "ADS1X15.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX



byte customChar[] = {
  B00000,
  B10000,
  B01000,
  B00100,
  B00010,
  B00001,
  B00000,
  B00000
};

const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','.'} //"*" это ввод
};
byte rowPins[rows] = {3, 8, 7, 5};
byte colPins[cols] = {4, 2, 6};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

LiquidCrystal_I2C lcd(0x27, 16, 2);
ADS1115 ads(0x48);

uint32_t tmr;
//калибровочные переменные
uint16_t PWM_max = 53526;
uint16_t PWM_min = 5603;
float U_max = 4.4822;
float U_min = 0.4975;
//рабочие переменные
uint16_t PWM = 0;
float U_set = 0;

float voltage = 0;
float percent = 0;

int x = 0;

struct Str
{
  uint16_t senD;
};
Str buf;//структура для отправки

void setup() {
  ads.setGain(0);
  ads.setDataRate(3);
  Serial.begin(28800);
  mySerial.begin(9600);
  lcd.init();                      // initialize the lcd
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.print("enter");
  lcd.createChar(0, customChar);


  Serial.println("CLEARDATA"); // очистка листа excel
  Serial.println("LABEL,Time,code,Vset,voltage,percent"); // заголовки столбцов




  for(int i = 5; i<500; i++)
  {
    
    
    U_set = i*0.01;
    PWM = (PWM_min+((U_set-U_min)*(PWM_max-PWM_min)/(U_max-U_min)));//вычисление шим значения исходя из установки
    buf.senD = PWM;//ввод переменной для отправки
    mySerial.write((byte*)&buf, sizeof(buf));//отправка
    delay(2000);//пусть значения устаканятся немного
    voltage = 0;
  voltage += ads.toVoltage(ads.readADC_Differential_0_1());
  percent = (abs(voltage - U_set))/U_set;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(U_set,3);
    lcd.print("V");
    lcd.print(" ");
    lcd.print(PWM);
    lcd.setCursor(0,1);
    lcd.print(voltage,3);
    lcd.print("Vr");
    lcd.print(" ");
    lcd.print(percent*100);
    lcd.print("%");
    Serial.print("DATA,TIME,");
    Serial.print(PWM);
    Serial.print(",");
    Serial.print(U_set,4);
    Serial.print(",");
    Serial.print(voltage,4);
    Serial.print(",");
    Serial.print(percent,5);
    Serial.println();
  }
}

void loop() {
 /*
  char keyk = keypad.getKey();
  if(keyk =='*')
  {
    lcd.backlight();
  //ввод, вычисление и отправка данных
   U_set = amogus(U_set);//ввод
   PWM = (PWM_min+((U_set-U_min)*(PWM_max-PWM_min)/(U_max-U_min)));//вычисление шим значения исходя из установки
   //это предусматривает некоторое "начальное" значение смещения. его нельзя скомпенсировать. для текущей итерации это около 40мВ.
   //ставить код ниже значения смещения нельзя - это приведет к уходу в минус, переполнению и выдаче максимальных значений 9+В
   lcd.setCursor(0,0);
   lcd.print("VOLTAGE ");//установка
   lcd.print(U_set,4);
   lcd.setCursor(0,1);
   lcd.print("PWM ");//шим значение
   lcd.print(PWM);
   buf.senD = PWM;//ввод переменной для отправки
   Serial.write((byte*)&buf, sizeof(buf));//отправка
  tmr = millis();
  }
  if(millis()-tmr>=5000)//если прошло больше 5 сек после ввода, выключаем подсветку
  {
    lcd.noBacklight();
  }
  */
}





//ввод
float amogus(float amogus)
{
uint32_t tmr1;
byte cntr=0;
char key;
char str1[12];
String str = "";
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("enter VOLTAGE");
    while(1)
  {
///////////////////// you spin me right round TODO переделать на массив чаров в цикле
    if (millis() - tmr1 >= 420) 
  {
    tmr1 = millis();
    switch(cntr)
    {
      case 0:
      {
        lcd.setCursor(15,0);
        lcd.print("|");
        break;
      }
      case 1:
      {
        lcd.setCursor(15,0);
        lcd.print("/");
        break;
      }
      case 2:
      {
        lcd.setCursor(15,0);
        lcd.print("-");
        break;
      }
      case 3:
      {
        lcd.setCursor(15,0);
        lcd.write(0);
        break;
      }
      case 4:
      {
        lcd.setCursor(15,0);
        lcd.print("|");
        break;
      }
      case 5:
      {
        lcd.setCursor(15,0);
        lcd.print("/");
        break;
      }
      case 6:
      {
        lcd.setCursor(15,0);
        lcd.print("-");
        break;
      }
      case 7:
      {
        lcd.setCursor(15,0);
        lcd.write(0);
        break;
      }
      case 8:
      {
        lcd.setCursor(15,0);
        lcd.print("|");
        cntr = 0;
        break;}
      default :
      {
        cntr = 0;
        lcd.setCursor(15,0);
        lcd.print("|");
        break;
      }
    }
    cntr++;
   
  }
///////////////////end of spin
    key = keypad.getKey();
    if((key != NO_KEY))
    {
    /*if(key == '.')
    {
      lcd.clear();
      return amogus;
    }*/
    if(key != '*')
    {
      if((key!='.')and(str.length()==1)and(str.lastIndexOf('.')==-1))//обязательно тыкать точку вторым символом, если её нет
      {str = str + '.';}
      if((key != '.')or((key == '.') and (str.length()==1)))
      {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("VOLTAGE ");
      lcd.setCursor(9,0);
      str = str + key;
      lcd.print(str);
      //lcd.setCursor(0,1);
      //lcd.print(str.length());
      }

     }
      
     if((key == '*')or(str.length() >= 5))//нажат ввод (*), достигнут лимит строки
    {
      
      str.toCharArray(str1, 12);
      amogus = atof(str1);
      
      str = "";
      if((amogus>9.9999)or(amogus<0.01))
      {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("OVER RANGE");
      delay(500);
        return 0;
      }
      
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("set");
      delay(500);
      lcd.clear();
      break;
    }
    }
    }
      return amogus;
}
