//24 бит предел по разрешающей способности

#include <Wire.h>

volatile uint32_t VoltageValue = 0;
volatile uint32_t pwm_Voltage_accum =0;

struct Str//структура для приема
{
  uint32_t senD;
};
Str buf;//инициализация структуры приема


void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);//выход таймера 2 канал В
  SetupTimer2();
  VoltageValue = 0;
}

void loop()
{

if(Serial.readBytes((byte*)&buf, sizeof(buf)))//прием в структуру
{
  VoltageValue = buf.senD;
}

}

void SetupTimer2() {
  // Disable interrupts while setting registers
  
  OCR2B = 0;
  TCCR2A = 0xA3;
  VoltageValue = 0L ;
  pwm_Voltage_accum = VoltageValue;
  TCCR2B = 0b00000001;//прескалер х1 = шим на 62,5кгц
  TIMSK2 = 0x01;   // Enable interrupt when Timer reaches OCRA
  sei();

}

ISR(TIMER2_OVF_vect) {
 
  OCR2B = (uint32_t) (pwm_Voltage_accum >> 16 ) ;//24-8=16 
  pwm_Voltage_accum &= 0x00FFFFL;// Delete the top byte leaving just the error
  //я так подозреваю, в изначальном коде первые 2 нуля означают первые 8 бит (разрядность программного ШИМ)
  pwm_Voltage_accum +=  VoltageValue ;
  if( pwm_Voltage_accum > 0xFFFFFFL ) pwm_Voltage_accum = 0x000000L;//24 бит
}