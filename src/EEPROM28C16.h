#ifndef EEPROM28C16_h
#define EEPROM28C16_h

#include <Arduino.h>

class EEPROM28C16 {
  public:
    EEPROM28C16();
    EEPROM28C16(uint8_t data, uint8_t latch, uint8_t clk, uint8_t write, uint8_t databus);

    byte read(uint16_t address);               //função para leitura da EEPROM
    byte write(uint16_t address, byte data);   //função para escrita da EEPROM
    void erase();                              //função para apagar EEPROM
  private:
    // --- Mapeamento de Hardware ---
    uint8_t shift_data;    //dados do shift register 74HC595
    uint8_t shift_latch;   //latch do shift register 74HC595
    uint8_t shift_clk;     //clock do shift register 74HC595
    uint8_t write_en;      //sinal de escrita para EEPROM
    uint8_t EEPROM_D0;     //bit menos significativo de dados da EEPROM
    uint8_t EEPROM_D7;     //bit mais significativo de dados da EEPROM

    void begin();
    void setAddress(uint16_t address, bool outEnable);         //função para seleção do endereço
};

#endif
