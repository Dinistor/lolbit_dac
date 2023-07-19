//TODO
//КАЛИБРОВКУ
//КАЛИБРОВОЧНОЕ МЕНЮ? ВВОД КАЛИБРОВОЧНЫХ ЗНАЧЕНИЙ?
//хранение калибровочных значений в EEPROM. тут надо аккуратно, дабы не получилось бесконечного цикла записи.
//для проверки, вместе с записью использовать мигание светодиодом на 13й ноге

#include <LiquidCrystal_I2C.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <EncButton.h>
#define I2CADDR 0x20

EncButton<EB_TICK, 2> btn;    

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
byte rowPins[rows] = {3, 4, 5, 6};
byte colPins[cols] = {0, 1, 2};
Keypad_I2C keypad ( makeKeymap(keys), rowPins, colPins, rows, cols, I2CADDR, PCF8574 );


LiquidCrystal_I2C lcd(0x27, 16, 2);

int32_t tmr;
//калибровочные переменные
int32_t PWM_max = 15030459L;
int32_t PWM_min = 1610774L;
float U_max = 8.99937;
float U_min = 0.999835;
bool calibrating = 0;
int calmode = 0;
//рабочие переменные
int32_t PWM = 0L;
float U_set = 0;



struct Str
{
  uint32_t senD;
};

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("* voltage set");
  //lcd.setCursor(0,1);
  //lcd.print("# calibration");
  lcd.createChar(0, customChar);
  keypad.begin(makeKeymap(keys));
}

void loop() {
  //btn.tick();
  //первая версия калибровки. Нажатие * даёт ввод, нажатие # даёт режим калибровки

  char keyk = keypad.getKey();

  if(keyk =='*')
  {
  input_voltage();
  if(millis()-tmr>=5000)//если прошло больше 5 сек после ввода, выключаем подсветку
  {
    lcd.noBacklight();
  }
  }





/*
  if(keyk == '.')
  {
    calibrating = 1;
  }
  while(calibrating = 1)
  {
    //btn.tick();
    if(btn.click())
    {
      calmode ++;
      if(calmode > 4)
      calmode = 0;
    }
    switch(calmode)
    {
      case 0:
      {
      //шаг 1. Ввести значение минимального ШИМа. 
      //* - ентер, # - "переввод"
      if(keyk == '*')
      {
        lcd.clear();
        break;
        //вкинуть сюда запись в память
      }
      if(keyk == '.')
      {
      PWM_min = calPWMinput();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save PWM MIN?");
      lcd.setCursor(0,1);
      lcd.print(PWM_min);
      }
      keyk = keypad.getKey();
      }
      case 1:
      {
      //шаг 2. Ввести показания эталонного вольтметра на минимальном ШИМе.
      if(keyk == '*')
      {
        lcd.clear();
        break;
        //вкинуть сюда запись в память
      }
      if(keyk == '.')
      {
      U_min = amogus(U_min);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Umin?");
      lcd.setCursor(0,1);
      lcd.print(U_min);
      }
      keyk = keypad.getKey();
      }
      case 2:
      {
        //шаг 1. Ввести значение максимального ШИМа. 
      //* - ентер, # - "переввод"
      if(keyk == '*')
      {
        lcd.clear();
        break;
        //вкинуть сюда запись в память
      }
      if(keyk == '.')
      {
      PWM_max = calPWMinput();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save MAX PWM?");
      lcd.setCursor(0,1);
      lcd.print(PWM_max);
      }
      keyk = keypad.getKey();
      }
      case 3:
      {
        //шаг 2. Ввести показания эталонного вольтметра на максимальном ШИМе.
      if(keyk == '*')
      {
        lcd.clear();
        break;
        //вкинуть сюда запись в память
      }
      if(keyk == '.')
      {
      U_max = amogus(U_max);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Save Umax?");
      lcd.setCursor(0,1);
      lcd.print(U_max);
      }
      keyk = keypad.getKey();
      }
      }
    }*/
}


void input_voltage()
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
   Str buf;//структура для отправки
   buf.senD = PWM;//ввод переменной для отправки
   Serial.write((byte*)&buf, sizeof(buf));//отправка
  tmr = millis();
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
///////////////////// you spin me right round 
//TODO переделать на массив чаров в цикле
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
///////////////////   i live in spain without a
    key = keypad.getKey();
    if((key != NO_KEY))
    {
    /*if(key == '.')//решетка - отмена ввода. не задействовано, если решетка ставит точку.
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
      lcd.print("Voltage:");
      lcd.setCursor(0,1);
      str = str + key;
      lcd.print(str);
      //lcd.setCursor(0,1);//дебаг
      //lcd.print(str.length());
      }

     }
      
     if((key == '*')or(str.length() >= 6))//нажат ввод (*), достигнут лимит строки
    {
      
      str.toCharArray(str1, 12);
      amogus = atof(str1);
      
      str = "";
      if((amogus>9.999)or(amogus<0.01))
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

















uint32_t calPWMinput()
{
uint32_t PWM_val = 0ul;

char key;
char str1[12];
String str = "";
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter LOW PWM");
    while(1)
  {
    key = keypad.getKey();
    if((key != NO_KEY))
    {
    if(key == '.')//решетка - отмена ввода
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Input cancelled");
      lcd.setCursor(0,1);
      lcd.print("RETRY input");
      return 0;
    }
    if((key != '*')and((key != '.')))
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PWM value: ");
      lcd.setCursor(0,1);
      str = str + key;
      lcd.print(str);
     }
      
     if((key == '*')or(str.length() >= 8))//нажат ввод (*), достигнут лимит строки
    {
      
      str.toCharArray(str1, 12);
      PWM_val = atof(str1);
      
      str = "";
      if(PWM_val>16777215)
      {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("OVER RANGE");
      lcd.setCursor(0,1);
      lcd.print("RETRY INPUT");
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
    return PWM_val;

}


