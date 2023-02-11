
#include <Arduino.h>
#include "EEPROM28C16.h"

EEPROM28C16::EEPROM28C16(uint8_t data, uint8_t latch, uint8_t clk, uint8_t write, uint8_t databus)
{
  shift_data  = data;        //dados do shift register 74HC595
  shift_latch = latch;       //latch do shift register 74HC595
  shift_clk   = clk;         //clock do shift register 74HC595
  write_en    = write;       //sinal de escrita para EEPROM
  EEPROM_D0   = databus;     //bit menos significativo de dados da EEPROM
  EEPROM_D7   = databus + 7; //bit mais significativo de dados da EEPROM

  begin();
}

EEPROM28C16::EEPROM28C16()
{
  shift_data  = 10; //dados do shift register 74HC595
  shift_latch = 11; //latch do shift register 74HC595
  shift_clk   = 12; //clock do shift register 74HC595
  write_en    = 13; //sinal de escrita para EEPROM
  EEPROM_D0   =  2; //bit menos significativo de dados da EEPROM
  EEPROM_D7   =  9; //bit mais significativo de dados da EEPROM

  begin();
}

void EEPROM28C16::begin()
{
  pinMode(shift_latch, OUTPUT);   //sa�da para latch
  pinMode(shift_data,  OUTPUT);   //sa�da para dados
  pinMode(shift_clk,   OUTPUT);   //sa�da para clock
  digitalWrite(write_en, HIGH);   //pullup interno em write_en
  pinMode(write_en, OUTPUT);      //sa�da para write_en
}

void EEPROM28C16::setAddress(uint16_t address, bool outEnable)
{
  // Seleciona os 3 bits mais significativos de endere�o com bit outEnable
  shiftOut(shift_data, shift_clk, MSBFIRST, (address >> 8) | (outEnable ? 0x00 : 0x80));

  //Seleciona os 8 bits menos significativos de endere�o
  shiftOut(shift_data, shift_clk, MSBFIRST, address);

  //gera pulso de latch para escrever dados nas sa�das dos shift registers
  digitalWrite(shift_latch,  LOW);
  digitalWrite(shift_latch, HIGH);
  digitalWrite(shift_latch,  LOW);
} //end setAddress

byte EEPROM28C16::read(uint16_t address)
{
  //configura pinos de dados como entrada
  for (uint8_t pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1)
    pinMode(pin, INPUT);

  setAddress(address, true);   //seleciona endere�o para leitura

  byte data = 0;               //vari�vel local para armazenar dados

  //realiza a leitura dos dados
  for (uint8_t pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1)
    data = (data << 1) + digitalRead(pin);

  return data;                 //retorna o dado lido
} //end read

byte EEPROM28C16::write(uint16_t address, byte data)
{
  //configura os pinos de dados como sa�da
  for (uint8_t pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1)
    pinMode(pin, OUTPUT);

  setAddress(address, false);   //seleciona endere�o para escrita

  //envia os dados para as sa�das
  for (uint8_t pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1)
  {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  } //end for

  //gera o pulso de escrita
  digitalWrite(write_en, LOW);
  delayMicroseconds(1);
  digitalWrite(write_en, HIGH);
  delay(10);
} //end write

void EEPROM28C16::erase()
{
  for (int address = 0; address <= 2047; address += 1)  //apaga EEPROM escrevendo FFh em
    write(address, 0xFF);                           //todos os endere�os
} //end erase
