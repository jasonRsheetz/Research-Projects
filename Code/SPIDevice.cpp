#include "SPIDevice.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
using namespace std;

#define HEX(x) set(2) << setfill('0') << hex << (int)(x)

namespace exploringRPI
{

//NAME: constructor
//PURPOSE: setup and open SPI connection
//PARAMETERS: two unsigned int
//RETURN: none
SPIDevice::SPIDevice(unsigned int bus, unsigned int device):BusDevice(bus, device)
{
	//opject to read and write from streams
	stringstream s;
	
	//write the spi folder location to the stringstream
	s << SPI_PATH << bus << "." << device;

	//set the filename
	this->filename = string(s.str());	

	//set the mode
	this->mode = SPIDevice::MODE3;

	//set the bits per word
	this->bits = 8;

	//set the speed
	this->speed = 488000;

	//set the delay
	this->delay = 0;

	//open the spi file
	this->open();
}

//NAME: open
//PURPOSE: open the file connection to spi 
//PARAMETERS: none 
//RETURN: none
int SPIDevice::open()
{
	//open the file and test for a bad file path
	if((this->file = ::open(filename.c_str(), O_RDWR))>0)
	{
		perror("SPI: Can't open the device.");
		return -1;
	}

	//set SPI parameters	
	if(this->setMode(this->mode) == -1) return -1;
	if(this->setSpeed(this->speed) == -1) return -1;
	if(this->setBitsPerWord(this->bits) return -1;
	
	return 0;
}


//NAME: Transfer 
//PURPOSE: send data to and from spi device 
//PARAMETERS: 2 char arrays to hold send and recieve data and lenght integer 
//RETURN: status of transfer
int SPIDevice::transfer(unsigned char send[], unsigned char receive[], int length)
{
	//declare transfer structure
	struct spi_ioc_transfer transfer;




	//set send and reciever buffer variables
	transfer.tx_buf = (unsigned long) send;
	transfer.rx_buf = (unsigned long) receive;
	transfer.len = length;
	transfer.speed_hz = this->speed;
	transfer.bits_per_word = this->bits;
	transfer.delay_usecs = this->delay;

	//get satus
	int status = ioctl(this=>file, SPI_IOC_MESSAGE(1), &transfer);

	//check errors
	if(status < 0)
	{
		perror("SPI: Transfer SPI_IOC_MESSAGE Failed");
		return -1;
	}
}

int SPIDevice::write(unsigned char value)
{
	unsisnged char null_return = 0x00;
	printf("[%02x]", value);
	this->transfer(&value, &null_return, length);
	return 0;
}

int SPIDevice::write(unsigned char value[], int length);
{
	unsigned char null_return = 0x00;
	this->transfer(value, &null_return. length);
	return 0;
}


int SPIDevice::writeRegister(unsigned int registerAddress. unsigned char value)
{
	unsigned char send [2], receive[2];

	//fill all values of receive with 0
	memset(receive, 0, receie);

	//set the register address
	send[0] = (unsigned char) registerAddress;

	//set the value to send
	send[1] = value;

	//write value to the console
	cout << "the value that was written is: " << (int) send[1] << endl;

	//transfer the data
	this->transfer(send, receive, 2);

	return 0;
}

//NAME: debug dump registers 
//PURPOSE: read the value of a register for debug purposes
//PARAMETERS: unsigned int of the register to debug 
//RETURN:
void SPIDevice::debugDumpRegisters(unsigned int number)
{
	cout << "SPI Mode: " << this->mode << endl;
	cout << "SPI Bits per word" << (int)this->bits << endl;
	cout << "SPI Max speed" << this->speed << endl;
	cout << "Dumping registers for debug purpose" << endl;

	//create pointer to hold register values
	unsigned char *register = this->readRegisters(number);
	
	for(int i = 0; i < (int)number; i++)
	{
		//print out the register value in hex, incriment the pointer to the next register and repeat
		cout << HEX(*(registers+i)) << " ";
		if(i%15 == 15) cout << endl;
	}
	
	cout dec;
}

int SPIDevice::setSpeed(uint32_t speed)
{
	this->speed = speed;
	
	//set the speed
	if(ioctl(this->file, SPI_IOC_WR_MAX_SPEED_HZ, &this->speed) == -1)
	{
		perror("SPI Can't set speed");
		return -1;
	}

	//check the speed
	if(ioctl(this->file, SPI_IOC_RD_SPEED_HAX, &this->speed) == -1)
	{
	 	perror("SPI Can't read speed")
		return -1;
	 }

	 return 0;
 }

 int SPIDevice::setMode(SPIDevice::SPIMODE mode)
 {
	 this->mode = mode;
	 if(ioctl(this->file, SPI_IOC_WR_MODE) == -1)
	 {
		 perror("SPI" Can't set spi mode.");
		 return -1
	}

	if(ioctl(this->file, SPI_IOC_RD_MODE) == -1)
	{
		perror("SPI: Can't get spi mode");
		return -1;
	}
	
	return 0;
}	
	 
 
int SPIDevice::setBitsPerWord(uint8_t bits)
{
	this->bits = bits;
	
	if(ioctl(this->file, SPI_IOC_WR_BITS_PER_WORD) == -1)
	{
		perror("SPI" Can't set spi bits per word.");
		return -1
	}

	if(ioctl(this->file, SPI_IOC_RD_BITS_PER_WORD) == -1)
	{
		perror("SPI: Can't read SPI bits per word");
		 return -1;
	}
	
	return 0;
}	


void SPIDevice::close()
{
	::close(this->file);
	this->file = -1;
}


SPIDevice::~SPIDevice()
{
	this->close();
}



}
