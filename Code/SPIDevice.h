#ifndef SPIDEVICE_H_
#define SPIDEVICE_H_
#include<string>
#include<stdint.h>
#include"BusDevice.h"

#define SPI_PATH "/dev/spidev"

namespace exploringRPI

{

//class declaration
class SPIDevice:public BusDevice
{
public:
	
	//spi modes
	enum SPIMODE
	{
		MODE0 = 0, //low at idle, capture on rising clk edge
		MODE1 = 1, //low at idle, capture on falling clk edge
		MODE2 = 2, //high at idle, captuer on falling clk edge
		MODE3 = 3 //high at idle, capture on rising clk edge
	};

private:
	std::string filename;

public:
	SPIDevice(unsigned int bus, unsigned int device);
	virtual int open();
	virtual unsigned char readRegister(unsigned int registerAddress);
	virtual unsigned char* readRegisters(unsigned int number, unsigned int fromAddress=0);
	virtual int writeRegister(unsigned int registerAddress, unsigned char value);
	virtual void debugDumpRegisters(unsigned int number = 0xff);
	virtual int write(unsigned char value);
	virtual int write(unsigned char value[], int length);
	virtual int setSpeed(uint32_t speed);
	virtual int setMode(SPIDevice::SPIMODE mode);
	virtual int setBitsPerWord(uint8_t bits);
	virtual void close();
	virtual ~SPIDevice();
	virtual int transfer(unsigned char readp[], unsigned char write[], int length);

private:
	SPIMODE mode;
	uint8_t bits;
	uint32_t speed;
	uint32_t delay;
};
}//namespace
#endif


