//include guard
#ifndef COM_DRIVER_SPI_H
#define COM_DRIVER_SPI_H

//SPI communication driver class
//834 Bytes Flash
//10 Bytes RAM

/*
   1.1.4
   Changed: Comments reworked 

   1.1.3
   New:
   Delay Mikroseconds for leading, trailing data write operations

   1.1.2
   Added:
   uint32_t getFrequency();
   Changed:
   Deselected slave for special applications as SD Card - clock only data out

   1.1.1
   Changed:
   SPI.begin before initalize the chip select pin

   1.0.1
   New:
   receivedVal = SPI.transfer(val)
   receivedVal16 = SPI.transfer16(val16)

   1.1.0
   void writeSpiManual8(uint8_t data);
   uint8_t readSpiManual8(void);
   void writeSpiManual16(uint16_t data);
   uint16_t readSpiManual16(void);
   void writeSpiManual32(uint32_t data);
   uint32_t readSpiManual32(void);
*/

//Include SPI.h
#include <SPI.h>

//uint8_t
#include "stdint.h"

class ComDriverSpi
{
  public:

    ComDriverSpi();
    ComDriverSpi(uint8_t slaveSelectPin);
    ComDriverSpi(uint8_t slaveSelectPin, uint32_t frequency);

    //SPI transfer option to (de-)select slave
    enum transferOption_t {transferEnd, transferStart, transferStartEnd};

    const uint8_t slaveSelectPin = 4;

    //Write 8 bit data vector
    bool writeSpi(uint8_t data[], uint32_t sizeToWrite, transferOption_t transferOption = transferStartEnd);
    //Read 8 bit data vector
    bool readSpi(uint8_t data[], uint32_t sizeToRead, transferOption_t transferOption = transferStartEnd);
    //Write and than read 8 bit data vector
    bool writeReadSpi(uint8_t dataWrite[], uint32_t sizeToWrite, uint8_t dataRead[], uint32_t sizeToRead, transferOption_t transferOption = transferStartEnd);

    //Manual slave select pin low to start
    void startWriteSpiManual(void);
    //Manual slave select pin low to end
    void endWriteSpiManual(void);

    //User needs to care about slave select pin manually with startWriteSpiManual() and endWriteSpiManual()
    //Manual write 8 bit data vector
    void writeSpiManual(uint8_t dataWrite[], uint32_t sizeToWrite);
    //Manual read 8 bit data vector
    void readSpiManual(uint8_t dataRead[], uint32_t sizeToRead);
    
    //Manual write data 8 bit
    void writeSpiManual8(uint8_t data);
    //Manual read Spi data 8 bit
    uint8_t readSpiManual8(void);
    //Manual write data 16 bit
    void writeSpiManual16(uint16_t data);
    //Manual read data 16 bit
    uint16_t readSpiManual16(void);
    //Manual write data 32 bit
    void writeSpiManual32(uint32_t data);
    //Manual read data 32 bit
    uint32_t readSpiManual32(void);


    //Slave Select Pin
    void setSlaveSelectPin(uint8_t slaveSelectPin);
    uint8_t getSlaveSelectPin();
    //Frequency
    void setFrequency(uint32_t frequency);
    uint32_t getFrequency();
    //Data mode SPI_MODE0 SPI_MODE1 SPI_MODE2 SPI_MODE3
    void setDataMode(uint8_t dataMode);
    uint8_t getDataMode();
    //Data order LSBFIRST, MSBFIRST
    void setDataOrder(uint8_t dataOrder);
    uint8_t getDataOrder();
    //Transferoption Start, End or Start and End by selecting slave pin
    void setTransferOption(transferOption_t transferOption);
    transferOption_t getTransferOption();

  private:
    uint8_t 		_slaveSelectPin;
    uint32_t 		_frequency;
    uint8_t 		_dataOrder;
    uint8_t 		_dataMode;
    transferOption_t  _transferOption;
    uint32_t 		_delayLeading;
    uint32_t 		_delayTrailing;

};
#endif /*COM_DRIVER_SPI_H*/
