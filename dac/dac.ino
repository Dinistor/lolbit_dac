//12 bit version

#include <Wire.h>

volatile uint16_t VoltageValue = 0L;
volatile uint16_t pwm_Voltage_accum =0L;


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
  TCCR2B = 0b00000010;//прескалер х8 = шим на 7.8кгц
  TIMSK2 = 0x01;   // Enable interrupt when Timer reaches OCRA
  sei();

}
ISR(TIMER2_OVF_vect) {
 
  OCR2B = (uint16_t) (pwm_Voltage_accum >> 4 ) ;//12-8=4
  pwm_Voltage_accum &= 0x00F;// Delete the top byte leaving just the error
  //я так подозреваю, в изначальном коде первые 2 нуля означают первые 8 бит (разрядность программного ШИМ)
  //
  pwm_Voltage_accum +=  VoltageValue ;
  if( pwm_Voltage_accum > 0xFFF ) pwm_Voltage_accum = 0xFFF;//12 бит
}