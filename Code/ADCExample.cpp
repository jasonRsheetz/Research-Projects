#include<iostream>
#include<sstream>
#include"SPIDevice.h"

using namespace std;
using namespace exploringRPI;

short combineValues(unsigned char upper, unsigned char lower)
{
	return ((short)upper<<8|(short)lower);
}

int main()
{
	cout << "starting RPI SPI ADC Example"<< endl;

	//create pointer to new spidevice object
	SPIDevice *busDevice = new SPIDevice(0,0);

	//set the speed
	busDevice->setSpeed(488000);

	//set the mode
	busDevice->setMode(SPIDevice::MODE0);

	
	cout << "SPI ADC is setup" << endl;

	//create send and recieve buffers
	unsigned char send[3], recieve[3];

	//start bit
	send[0] = 0b00000001;

	//set the SGL/DIFF and D mode 
	send[1] = 0b1000000;

	//clear display, this byte doesn't need to be set
	send[2] = 0;

	//transfer send buffer
	busDevice->transfer(send, recieve, 3);

	cout << "response butes are" << (int)recieve[1] << (int)recieve[2] << endl;

	//use the 8-bits of the second value and the two LSBs of the first value
	int value = combineValues(recieve[1]&0b00000011, recieve[2]);

	cout << "this is the value" << value << "out of 1024" << endl;
	cout << "end of this example" << endl;

return 0;
}
