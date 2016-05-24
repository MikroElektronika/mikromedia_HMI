#include <built_in.h>
#include "SF_driver.h"

extern sfr sbit CS_Serial_Flash_bit;
void SerialFlash_init(){
  CS_Serial_Flash_bit = 1;
}

void SerialFlash_WriteEnable(){
  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_WREN);
  CS_Serial_Flash_bit = 1;
}

unsigned char SerialFlash_IsWriteBusy(){
  unsigned char temp;

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_RDSR);
  temp = SPIM_Read(0);
  CS_Serial_Flash_bit = 1;

  return (temp&0x01);
}

void SerialFlash_WriteByte(unsigned char _data, unsigned long address){
    SerialFlash_WriteEnable();

    CS_Serial_Flash_bit = 0;
    SPIM_Write(_SERIAL_FLASH_CMD_WRITE);
    SPIM_Write(Higher(address));
    SPIM_Write(Hi(address));
    SPIM_Write(Lo(address));
    SPIM_Write(_data);
    CS_Serial_Flash_bit = 1;

    // Wait for write end
    while(SerialFlash_isWriteBusy());
}

void SerialFlash_WriteWord(unsigned int _data, unsigned long address){
  SerialFlash_WriteByte(Hi(_data),address);
  SerialFlash_WriteByte(Lo(_data),address+1);
}

unsigned char SerialFlash_ReadID(void){
  unsigned char temp;

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_RDID);
  temp = SPIM_Read(0);
  CS_Serial_Flash_bit = 1;

  return temp;
}

unsigned char SerialFlash_ReadByte(unsigned long address){
  unsigned char temp;

  CS_Serial_Flash_bit = 0;

  SPIM_Write(_SERIAL_FLASH_CMD_READ);
  SPIM_Write(Higher(address));
  SPIM_Write(Hi(address));
  SPIM_Write(Lo(address));
  temp = SPIM_Read(0);

  CS_Serial_Flash_bit = 1;
  return temp;
}

unsigned int SerialFlash_ReadWord(unsigned long address){
  unsigned int temp;

  Hi(temp) = SerialFlash_ReadByte(address);
  Lo(temp) = SerialFlash_ReadByte(address+1);

  return temp;
}

unsigned char SerialFlash_WriteArray(unsigned long address, unsigned char* pData, unsigned int nCount){
  unsigned long addr;
  unsigned char* pD;
  unsigned int counter;

  addr = address;
  pD   = pData;

  // WRITE

  for(counter = 0; counter < nCount; counter++){
      SerialFlash_WriteByte(*pD++, addr++);
  }


  // VERIFY

  for (counter=0; counter < nCount; counter++){
    if (*pData != SerialFlash_ReadByte(address))
        return 0;
    pData++;
    address++;
  }

  return 1;
}


void SerialFlash_ReadArray(unsigned long address, unsigned char* pData, unsigned int nCount){
  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_READ);
  SPIM_Write(Higher(address));
  SPIM_Write(Hi(address));
  SPIM_Write(Lo(address));

  while(nCount--){
    *pData++ = SPIM_Read(0);
  }
  CS_Serial_Flash_bit = 1;
}

void SerialFlash_ChipErase(void){

  SerialFlash_WriteEnable();

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_ERASE);
  CS_Serial_Flash_bit = 1;

  // Wait for write end
  while(SerialFlash_IsWriteBusy());
}

void SerialFlash_ResetWriteProtection(){

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_EWSR);
  CS_Serial_Flash_bit = 1;

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_EWSR);
  SPIM_Write(0);
  CS_Serial_Flash_bit = 1;
}

void SerialFlash_SectorErase(unsigned long address){

  SerialFlash_WriteEnable();

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_SER);
  SPIM_Write(Higher(address));
  SPIM_Write(Hi(address));
  SPIM_Write(Lo(address));
  CS_Serial_Flash_bit = 1;

  // Wait for write end
  while(SerialFlash_IsWriteBusy());
}

/**************************************************************************************************
* End of File
**************************************************************************************************/