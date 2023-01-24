//SPI communication layer
#include "ComDriverSpi.h"

ComDriverSpi::ComDriverSpi()
//Initalization list
  : _slaveSelectPin{slaveSelectPin},
    _frequency {8000000},
    _dataOrder {MSBFIRST},
    _dataMode {SPI_MODE0},
    _transferOption {transferStartEnd}
{
  //start the SPI library
  SPI.begin();

  //initalize the slave select pin
  pinMode(_slaveSelectPin, OUTPUT);
  digitalWrite(_slaveSelectPin, HIGH);

}

ComDriverSpi::ComDriverSpi(uint8_t slaveSelectPin)
  : _slaveSelectPin(slaveSelectPin),
    _frequency {8000000},
    _dataOrder {MSBFIRST},
    _dataMode {SPI_MODE0},
    _transferOption {transferStartEnd}
{
  //start the SPI library
  SPI.begin();

  //initalize the slave select pin
  pinMode(_slaveSelectPin, OUTPUT);
  digitalWrite(_slaveSelectPin, HIGH);
}

ComDriverSpi::ComDriverSpi(uint8_t slaveSelectPin, uint32_t frequency)
  : _slaveSelectPin(slaveSelectPin),
    _frequency(frequency),
    _dataOrder {MSBFIRST},
    _dataMode {SPI_MODE0},
    _transferOption {transferStartEnd}
{
  //start the SPI library
  SPI.begin();

  //initalize the slave select pin
  pinMode(_slaveSelectPin, OUTPUT);
  digitalWrite(_slaveSelectPin, HIGH);
}

void ComDriverSpi::setSlaveSelectPin(uint8_t slaveSelectPin)
{
  _slaveSelectPin = slaveSelectPin;
}

uint8_t ComDriverSpi::getSlaveSelectPin()
{
  return _slaveSelectPin;
}

void ComDriverSpi::setFrequency(uint32_t frequency)
{
  /*SPI_CLOCK_DIV2
    SPI_CLOCK_DIV4
    SPI_CLOCK_DIV8
    SPI_CLOCK_DIV16
    SPI_CLOCK_DIV32
    SPI_CLOCK_DIV64
    SPI_CLOCK_DIV128
  */
  _frequency = frequency;
}

uint32_t ComDriverSpi::getFrequency()
{
  return _frequency;
}

void ComDriverSpi::setDataMode(uint8_t dataMode)
{
  _dataMode = dataMode;
}

uint8_t ComDriverSpi::getDataMode()
{
  return _dataMode;
}

void ComDriverSpi::setDataOrder(uint8_t dataOrder)
{
  _dataOrder = dataOrder;
}

uint8_t ComDriverSpi::getDataOrder()
{
  return _dataOrder;
}

void ComDriverSpi::setTransferOption(transferOption_t transferOption)
{
  _transferOption = transferOption;
}

ComDriverSpi::transferOption_t ComDriverSpi::getTransferOption()
{
  return _transferOption;
}

bool ComDriverSpi::writeSpi(uint8_t data[], uint32_t sizeToWrite, transferOption_t transferOption)
{
  //Set member
  _transferOption = transferOption;

  //Control the bus
  SPI.beginTransaction(SPISettings(_frequency, _dataOrder, _dataMode));

  //Select slave by pin low
  if (_transferOption == transferStart || _transferOption == transferStartEnd)
  {
    digitalWrite(_slaveSelectPin, LOW);
  }

  //send data with datasize
  SPI.transfer(data, sizeToWrite);

  //Deselect slave by pin high at end of transfer
  if (_transferOption == transferEnd || _transferOption == transferStartEnd)
  {
    digitalWrite(_slaveSelectPin, HIGH);
    //Release the bus
    SPI.endTransaction();
  }

  return true;
}


bool ComDriverSpi::readSpi(uint8_t data[], uint32_t sizeToRead, transferOption_t transferOption)
{
  //Set member
  _transferOption = transferOption;

  //Control the bus
  SPI.beginTransaction(SPISettings(_frequency, _dataOrder, _dataMode));

  //Select slave by pin low
  if (_transferOption == transferStart || _transferOption == transferStartEnd)
  {
    digitalWrite(_slaveSelectPin, LOW);
  }

  //Read data with datasize
  SPI.transfer(data, sizeToRead);

  //Deselect slave by pin high at end of transfer
  if (_transferOption == transferEnd || _transferOption == transferStartEnd)
  {
    digitalWrite(_slaveSelectPin, HIGH);
    //Release the bus
    SPI.endTransaction();
  }

  return true;
}

bool ComDriverSpi::writeReadSpi(uint8_t dataWrite[], uint32_t sizeToWrite, uint8_t dataRead[], unsigned long sizeToRead, transferOption_t transferOption)
{

  _transferOption = transferOption;

  //Control the SPI bus
  SPI.beginTransaction(SPISettings(_frequency, _dataOrder, _dataMode));

  //Select slave by pin low
  if (_transferOption == transferStart || _transferOption == transferStartEnd)
  {
    digitalWrite(_slaveSelectPin, LOW);
  }

  //send data and datasize
  SPI.transfer(dataWrite, sizeToWrite);
  SPI.transfer(dataRead, sizeToRead);

  //Slave select pin high
  if (_transferOption == transferEnd || _transferOption == transferStartEnd)
  {
    digitalWrite(_slaveSelectPin, HIGH);
    //Release the SPI bus
    SPI.endTransaction();
  }

  return true;
}

void ComDriverSpi::startWriteSpiManual(void)
{
  //Control the SPI bus
  SPI.beginTransaction(SPISettings(_frequency, _dataOrder, _dataMode));
  //Slave select pin low
  digitalWrite(_slaveSelectPin, LOW);
}

void ComDriverSpi::endWriteSpiManual(void)
{
  digitalWrite(_slaveSelectPin, HIGH);
  SPI.endTransaction();
}

void ComDriverSpi::writeSpiManual8(uint8_t data)
{
  SPI.transfer(data);
}

uint8_t ComDriverSpi::readSpiManual8(void)
{
  uint8_t data = 0xff;
  return SPI.transfer(data);
}

void ComDriverSpi::writeSpiManual16(uint16_t data)
{
  SPI.transfer16(data);
}

uint16_t ComDriverSpi::readSpiManual16(void)
{
  uint16_t data = 0xffff;
  return SPI.transfer16(data);
}

void ComDriverSpi::writeSpiManual32(uint32_t data)
{
  SPI.transfer16(uint16_t(data >> 0xf));
  SPI.transfer16(uint16_t(data && 0xffff));
}

uint32_t ComDriverSpi::readSpiManual32(void)
{
  uint16_t dataLow = 0xffff;
  uint16_t dataHigh = 0xffff;
  uint32_t data = 0xffffffff;

  dataLow = SPI.transfer16(dataLow);
  dataHigh = SPI.transfer16(dataHigh);

  data = (uint32_t) dataHigh << 0xf & dataLow;

  return data;
}

void ComDriverSpi::writeSpiManual(uint8_t dataWrite[], uint32_t sizeToWrite)
{
  SPI.transfer(dataWrite, sizeToWrite);
}

void ComDriverSpi::readSpiManual(uint8_t dataRead[], uint32_t sizeToRead)
{
  SPI.transfer(dataRead, sizeToRead);
}
