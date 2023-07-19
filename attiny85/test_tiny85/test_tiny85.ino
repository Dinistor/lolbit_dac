//24 бит предел по разрешающей способности

#include <Wire.h>
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>
//#include <TinySoftwareSerial.h>
volatile uint32_t VoltageValue = 0UL;
volatile uint32_t pwm_Voltage_accum =0UL;
/*
struct Str//структура для приема
{
  uint32_t senD = 0UL;
};
Str buf;//инициализация структуры приема
*/

void setup()
{
  //Serial.begin(9600);
  pinMode(PB1, OUTPUT);
  SetupTimer();
}

void loop()
{
/*
if(Serial.readBytes((byte*)&buf, sizeof(buf)))//прием в структуру
{
  VoltageValue = buf.senD;
}
*/
}

void SetupTimer() {
  TCNT0=0;
  OCR0A = 0;
  TCCR0A = 0b10100011;
  TCCR0B = 0b0000010;
  TIMSK|=(1<<TOIE0); //| (1<<OCIE0A);
  SREG |= (1<<SREG_I);
  sei();
  VoltageValue = 0xFFFFF0UL;
  //VoltageValue = 0UL;
  pwm_Voltage_accum = 0UL;
}

ISR(TIMER0_OVF_vect) {

  OCR0A = (byte) (pwm_Voltage_accum >> 16 ) ;
  pwm_Voltage_accum &= 0x00FFFFUL ;
  pwm_Voltage_accum +=  VoltageValue ;
}