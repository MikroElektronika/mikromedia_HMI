#include <built_in.h>
#include "SF_driver.h"

extern sfr sbit CS_Serial_Flash_bit;
/**************************************************************************************************
* Function SerialFlash_init()
* -------------------------------------------------------------------------------------------------
* Overview: Function that initializes SerialFlash by setting Chip select
* Input: none
* Output: none
**************************************************************************************************/
void SerialFlash_init(){
  CS_Serial_Flash_bit = 1;


}

/**************************************************************************************************
* Function SerialFlash_WriteEnable()
* -------------------------------------------------------------------------------------------------
* Overview: Function that sends write enable command to the chip
* Input: none
* Output: none
**************************************************************************************************/
void SerialFlash_WriteEnable(){
  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_WREN);
  CS_Serial_Flash_bit = 1;
}

/**************************************************************************************************
* Function SerialFlash_IsWriteBusy()
* -------------------------------------------------------------------------------------------------
* Overview: Function that checks whether chip finished write operation
* Input: none
* Output: 1 - if still busy, 0 - if write completed
**************************************************************************************************/
unsigned char SerialFlash_IsWriteBusy(){
  unsigned char temp;

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_RDSR);
  temp = SPIM_Read(0);
  CS_Serial_Flash_bit = 1;

  return (temp&0x01);
}

/**************************************************************************************************
* Function SerialFlash_WriteByte()
* -------------------------------------------------------------------------------------------------
* Overview: Function that writes a single byte
* Input: Data to be written and the address to which to store the data
* Output: none
**************************************************************************************************/
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

/**************************************************************************************************
* Function SerialFlash_WriteWord()
* -------------------------------------------------------------------------------------------------
* Overview: Function that writes 2 succesive bytes of a word variable
* Input: Word data to be written and the address to which to store the data
* Output: none
**************************************************************************************************/
void SerialFlash_WriteWord(unsigned int _data, unsigned long address){
  SerialFlash_WriteByte(Hi(_data),address);
  SerialFlash_WriteByte(Lo(_data),address+1);
}

/**************************************************************************************************
* Function SerialFlash_ReadID()
* -------------------------------------------------------------------------------------------------
* Overview: Function that reads the CHIP ID
* Input: none
* Output: ID byte value
**************************************************************************************************/
unsigned char SerialFlash_ReadID(void){
  unsigned char temp;

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_RDID);
  temp = SPIM_Read(0);
  CS_Serial_Flash_bit = 1;

  return temp;
}

/**************************************************************************************************
* Function SerialFlash_ReadByte()
* -------------------------------------------------------------------------------------------------
* Overview: Function that reads the byte from the address
* Input: address to be read
* Output: byte data from the address
**************************************************************************************************/
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

/**************************************************************************************************
* Function SerialFlash_ReadWord()
* -------------------------------------------------------------------------------------------------
* Overview: Function that reads the word from the address
* Input: address to be read
* Output: word data stored in two successive addresses
**************************************************************************************************/
unsigned int SerialFlash_ReadWord(unsigned long address){
  unsigned int temp;

  Hi(temp) = SerialFlash_ReadByte(address);
  Lo(temp) = SerialFlash_ReadByte(address+1);

  return temp;
}

/**************************************************************************************************
* Function SerialFlash_WriteArray()
* -------------------------------------------------------------------------------------------------
* Overview: Function that writes data to successive addresses
* Input: address of the begining, pointer to buffer containing data,
         number of bytes to be written
* Output: 1 - if write succeeded, 0 - if write failed
**************************************************************************************************/
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

/**************************************************************************************************
* Function SerialFlash_ReadArray()
* -------------------------------------------------------------------------------------------------
* Overview: Function that reads data from successive addresses
* Input: address of the begining, pointer to buffer where to store read data,
         number of bytes to be read
* Output: none
**************************************************************************************************/
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

/**************************************************************************************************
* Function SerialFlash_ChipErase()
* -------------------------------------------------------------------------------------------------
* Overview: Function that sends Chip Erase command
* Input: none
* Output: none
**************************************************************************************************/
void SerialFlash_ChipErase(void){

  SerialFlash_WriteEnable();

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_ERASE);
  CS_Serial_Flash_bit = 1;

  // Wait for write end
  while(SerialFlash_IsWriteBusy());
}

/**************************************************************************************************
* Function SerialFlash_ResetWriteProtection()
* -------------------------------------------------------------------------------------------------
* Overview: Function that sends Reset Write Protection command
* Input: none
* Output: none
**************************************************************************************************/
void SerialFlash_ResetWriteProtection(){

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_EWSR);
  CS_Serial_Flash_bit = 1;

  CS_Serial_Flash_bit = 0;
  SPIM_Write(_SERIAL_FLASH_CMD_EWSR);
  SPIM_Write(0);
  CS_Serial_Flash_bit = 1;
}

/**************************************************************************************************
* Function SerialFlash_SectorErase()
* -------------------------------------------------------------------------------------------------
* Overview: Function that sends Sector Erase command
* Input: address of the sector to be erased
* Output: none
**************************************************************************************************/
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