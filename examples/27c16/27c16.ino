/* =========================================================================================================

   WR Kits vídeo aula de Engenharia Eletrônica (a clássica de sexta) número 291

   Gravador de EEPROM com Arduino

   Compilador: Arduino IDE 1.8.4

   Autor: Eng. Wagner Rambo
   Data: Setembro de 2019

   Adaptação do código original do autor Ben Eater
   Link: https://www.youtube.com/watch?v=K88pgWhEb1M

  ========================================================================================================= */

#include "EEPROM27C16.h"

// =========================================================================================================
// --- Preencha o vetor com os dados que deseja escrever na EEPROM ---
byte data[] = {0xAB, 0x5E, 0x6F, 0xB4, 0xEC, 0x88, 0x25, 0x3D,
               0xC0, 0x12, 0x55, 0x79, 0xBC, 0xAA, 0x11, 0x99
              };

EEPROM27C16 e2716;

// =========================================================================================================
// --- Configurações Iniciais ---
void setup()
{
  Serial.begin(250000);           //inicializa Serial em 250000 bits por segundo

  e2716.erase();                //apaga EEPROM

  // ===============================================================

  //escreve nos 16 primeiros endereços da EEPROM
  //OBS.: utilizamos 16 apenas como exemplo
  for (int address = 0; address <= 15; address++)
    e2716.write(address, data[address]);

  printData();                    //imprime o conteúdo da EEPROM no monitor serial
} //end setup

// =========================================================================================================
// --- Loop Infinito ---
void loop()
{
  //nenhum processamento em loop infinito
} //end loop

// =========================================================================================================
// --- Desenvolvimento das Funções ---

void printData()
{
  //imprime os primeiros 256 endereços de dados
  for (int base = 0; base <= 15; base += 16)
  {
    byte data[16];

    for (int offset = 0; offset <= 15; offset++)
      data[offset] = e2716.read(base + offset);

    char buf[100];
    sprintf(buf, "%03x: %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  } //end for
} //end printData
