//24 бит предел по разрешающей способности

#include <Wire.h>

volatile uint32_t VoltageValue = 0UL;
volatile uint32_t pwm_Voltage_accum =0UL;

struct Str//структура для приема
{
  uint32_t senD = 0UL;
};
Str buf;//инициализация структуры приема


void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);//выход таймера 2 канал В
  SetupTimer2();
  VoltageValue = 0UL;
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
  cli();
  OCR2B = 0;
  TCCR2A = 0b10100011;
  VoltageValue = 0UL ;
  pwm_Voltage_accum = VoltageValue;
  TCCR2B = 0b00000011;//прескалер
  TIMSK2 = 0x01;   // Enable interrupt when Timer reaches OCR 2B
  sei();

}

/*
ISR(TIMER2_OVF_vect) {
 
  OCR2B = (uint32_t) (pwm_Voltage_accum >> 16 ) ;//24-8=16 тут мы отправили верхние 8 бит в регистр формирования ШИМа
  pwm_Voltage_accum &= 0x00FFFFL;// Delete the top byte leaving just the error - удалили отправленные в регистр биты
  //я так подозреваю, в изначальном коде первые 2 нуля означают первые 8 бит (разрядность программного ШИМ)
  pwm_Voltage_accum +=  VoltageValue ;
  if( pwm_Voltage_accum > 0xFFFFFFL ) pwm_Voltage_accum = 0x000000L;//24 бит
}
*/

ISR(TIMER2_OVF_vect) {

  //OCR2B = (byte) (pwm_Voltage_accum / 0x01000000L ) ; 
  OCR2B = (byte) (pwm_Voltage_accum >> 16 ) ;
  pwm_Voltage_accum &= 0x00FFFFUL ;
  pwm_Voltage_accum +=  VoltageValue ;
}