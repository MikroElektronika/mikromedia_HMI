// Global Variables
unsigned char Refresh_Cmd_Menu = 1;
unsigned char write_array[_DATA_ARRAY_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
unsigned char read_array[_DATA_ARRAY_SIZE] = {0};

// Functions
void SF_Start(void);
void SF_Test(char *err);

//- Imported from SF_DRIVER.H ---------------------------------------------------------------------
extern void SerialFlash_init();
extern void SerialFlash_WriteEnable();
extern unsigned char SerialFlash_IsWriteBusy();
extern void SerialFlash_WriteByte(unsigned char _data, unsigned long address);
extern void SerialFlash_WriteWord(unsigned int _data, unsigned long address);
extern unsigned char SerialFlash_ReadID(void);
extern unsigned char SerialFlash_ReadByte(unsigned long address);
extern unsigned int SerialFlash_ReadWord(unsigned long address);
extern unsigned char SerialFlash_WriteArray(unsigned long address, unsigned char* pData, unsigned int nCount);
extern void SerialFlash_ReadArray(unsigned long address, unsigned char* pData, unsigned int nCount);
extern void SerialFlash_ChipErase(void);
extern void SerialFlash_ResetWriteProtection();
extern void SerialFlash_SectorErase(unsigned long address);

/**************************************************************************************************
* End of File
**************************************************************************************************/