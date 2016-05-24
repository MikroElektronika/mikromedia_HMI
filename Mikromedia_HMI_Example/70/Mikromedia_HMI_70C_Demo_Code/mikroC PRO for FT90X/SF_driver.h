/**************************************************************************************************
* File: SF_driver.h
* File Type: C - Header File
/**************************************************************************************************
* Serial Flash Chip Select connection
**************************************************************************************************/
// Constans
static const unsigned short _SERIAL_FLASH_CMD_RDID  = 0x9F;    // 25P80
static const unsigned short _SERIAL_FLASH_CMD_READ  = 0x03;
static const unsigned short _SERIAL_FLASH_CMD_WRITE = 0x02;
static const unsigned short _SERIAL_FLASH_CMD_WREN  = 0x06;
static const unsigned short _SERIAL_FLASH_CMD_RDSR  = 0x05;
static const unsigned short _SERIAL_FLASH_CMD_ERASE = 0xC7;    // 25P80
static const unsigned short _SERIAL_FLASH_CMD_EWSR  = 0x06;    // 25P80
static const unsigned short _SERIAL_FLASH_CMD_WRSR  = 0x01;
static const unsigned short _SERIAL_FLASH_CMD_SER   = 0xD8;    //25P80

// Functions

/**
 * @brief SerialFlash_init
 * Initializes Serial Flash by setting Chip select
 */
void SerialFlash_init();

/**
 * @brief SerialFlash_WriteEnable
 * Sends write enable command to the chip
 */
void SerialFlash_WriteEnable();

/**
 * @brief SerialFlash_IsWriteBusy
 * Checks whether chip finished write operation
 * @return 1 - if still busy, 0 - if write completed
 */
unsigned char SerialFlash_IsWriteBusy();

/**
 * @brief SerialFlash_WriteByte
 * Writes a single byte
 * @param _data
 * Data to be written
 * @param address
 * Address to which to store the data
 */
void SerialFlash_WriteByte(unsigned char _data, unsigned long address);

/**
 * @brief SerialFlash_WriteWord
 * Writes 2 succesive bytes of a word variable
 * @param _data
 * Word of data to be written
 * @param address
 * Address to which to store the data
 */
void SerialFlash_WriteWord(unsigned int _data, unsigned long address);

/**
 * @brief SerialFlash_ReadID
 * Reads the CHIP ID
 * @return
 * ID byte value
 */
unsigned char SerialFlash_ReadID(void);

/**
 * @brief SerialFlash_ReadByte
 * Reads the byte from the address
 * @param address
 * Address from which to read
 * @return
 * Data byte read from the address
 */
unsigned char SerialFlash_ReadByte(unsigned long address);

/**
 * @brief SerialFlash_ReadWord
 * Function that reads the word from the address
 * @param address
 * Address to be read
 * @return
 * Word of data read from two successive addresses
 */
unsigned int SerialFlash_ReadWord(unsigned long address);

/**
 * @brief SerialFlash_WriteArray
 * Writes data to successive addresses
 * @param address
 * Address from where to begin writing
 * @param pData
 * Pointer to buffer containing the data to write
 * @param nCount
 * Number of bytes to be written
 * @return
 * 1 - if write succeeded, 0 - if write failed
 */
unsigned char SerialFlash_WriteArray(unsigned long address, unsigned char* pData, unsigned int nCount);

/**
 * @brief SerialFlash_ReadArray
 * Reads data from successive addresses
 * @param address
 * Address of the begining of the array to be read
 * @param pData
 * Pointer to buffer where to store read data
 * @param nCount
 * Number of bytes to be read
 */
void SerialFlash_ReadArray(unsigned long address, unsigned char* pData, unsigned int nCount);

/**
 * @brief SerialFlash_ChipErase
 * Sends Chip Erase command
 */
void SerialFlash_ChipErase(void);

/**
 * @brief SerialFlash_ResetWriteProtection
 * Sends Reset Write Protection command
 */
void SerialFlash_ResetWriteProtection();

/**
 * @brief SerialFlash_SectorErase
 * Sends Sector Erase command
 * @param address
 * Address of the sector to be erased
 */
void SerialFlash_SectorErase(unsigned long address);

/**************************************************************************************************
* End of File
**************************************************************************************************/