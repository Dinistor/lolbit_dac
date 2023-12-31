//16 бит версия одноканальная.

#include <Wire.h>

volatile uint16_t VoltageValue = 0;
volatile uint16_t pwm_Voltage_accum =0;


struct Str//структура для приема
{
  uint16_t senD;
};
Str buf;//инициализация структуры приема


void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT );//выход таймера 2 канал B
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
  VoltageValue = 0;
  pwm_Voltage_accum = VoltageValue ;
  TCCR2B = 0b00000001;//62.5kHz
  TIMSK2 = 0x01;   // Enable interrupt when Timer reaches OCRA
  sei();

}
ISR(TIMER2_OVF_vect) {
 
  OCR2B = (uint16_t) (pwm_Voltage_accum >> 8 ) ;//16-8=8
  pwm_Voltage_accum &= 0x00FF;// Delete the top byte leaving just the error
  //я так подозреваю, в изначальном коде первые 2 нуля означают первые 8 бит (разрядность программного ШИМ)
  //
  pwm_Voltage_accum +=  VoltageValue ;
  if( pwm_Voltage_accum > 0xFFFF ) pwm_Voltage_accum = 0xFFFF;//16 бит
}