#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h> 
#include <math.h>
#include <stdint.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <string.h>
#include "apa102LEDStrip.h"

//Pixel Config
const char maxBrightness=255, maxValue=64, bytesPerLED=4;
const unsigned short int numLeds=14*14;
apa102LEDStrip leds;
char currentCol[3];
const short int rainbowSize = maxValue*8;  
unsigned short int colIndex=0, ledIndex=0, runTimes=500;

//SPI Device
//SPI Device
int spiDevice;
const char *device = "/dev/spidev0.0";
uint8_t spiMode;
uint8_t bits = 8;
uint32_t speed = 10000000;
uint8_t returnBuffer;

void initLEDs();
void initSPI();
void renderLEDs();
void getColour(short int colourIndex, char *currentColour);


int main(int argc, char *argv[]) 
{
	initLEDs();
	initSPI();

	//Clear out Pixels
	currentCol[0]=0;
	currentCol[1]=0;
	currentCol[2]=0;
	for(ledIndex=0; ledIndex<leds._numLEDs; ledIndex++)
	{
		leds.setPixel(ledIndex, currentCol);
	}
	renderLEDs();
	
	while(runTimes>0)
	{
		getColour(colIndex%rainbowSize, currentCol);
		leds.setPixel(0, currentCol);
		for(ledIndex=leds._numLEDs-1; ledIndex>0; ledIndex--)
		{
			leds.getPixel(ledIndex-1, currentCol);
			leds.setPixel(ledIndex, currentCol);
		}
		colIndex+=5;
		renderLEDs();
		usleep(10000);
		runTimes--;
	}

	//Clear out Pixels
	currentCol[0]=0;
	currentCol[1]=0;
	currentCol[2]=0;
	for(ledIndex=0; ledIndex<leds._numLEDs; ledIndex++)
	{
		leds.setPixel(ledIndex, currentCol);
	}
	renderLEDs();
}

void renderLEDs()
{
	struct spi_ioc_transfer spiDataTransferBlock;
	uint8_t dataBlock[leds._frameLength];
	
	memcpy(dataBlock, leds.LEDs, leds._frameLength);
	spiDataTransferBlock.tx_buf = (unsigned long)dataBlock;
	spiDataTransferBlock.rx_buf = (unsigned long)returnBuffer;
	spiDataTransferBlock.len = leds._frameLength;
	spiDataTransferBlock.delay_usecs = 0;
	spiDataTransferBlock.speed_hz = speed;
	spiDataTransferBlock.bits_per_word = bits;
	ioctl(spiDevice, SPI_IOC_MESSAGE(1), &spiDataTransferBlock);	
}

void initSPI()
{
	spiDevice = open(device, O_RDWR);
	if(spiDevice<0){printf("\r\nYOU MUST RUN AS ROOT\r\n\tSPI DEVICE FAILED TO OPEN\r\n");}
	spiMode = SPI_MODE_0;
	ioctl(spiDevice, SPI_IOC_WR_MODE, &spiMode);
	if( ioctl(spiDevice, SPI_IOC_RD_MODE, &spiMode)==-1 ){printf("\r\nYOU MUST RUN AS ROOT\r\n\tMODE NOT SET\r\n");}
	ioctl(spiDevice, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if( ioctl(spiDevice, SPI_IOC_RD_BITS_PER_WORD, &bits)==-1 ){printf("\r\nYOU MUST RUN AS ROOT\r\n\tBITS NOT SET\r\n");}
	ioctl(spiDevice, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if( ioctl(spiDevice, SPI_IOC_RD_MAX_SPEED_HZ, &speed)==-1 ){printf("\r\nYOU MUST RUN AS ROOT\r\n\tSPEED NOT SET\r\n");}
}

void initLEDs()
{
	leds.newBlock(numLeds, 4, maxBrightness);
}

void getColour(short int colourIndex, char *currentColour)
{

 if( colourIndex>=0 && colourIndex<maxValue )
 {
    currentColour[0] = maxValue;
    currentColour[1] = colourIndex;
    currentColour[2] = 0; 
 }
 else if(colourIndex>=maxValue && colourIndex<maxValue*2)
 {
    currentColour[0] = maxValue-(colourIndex-maxValue);
    currentColour[1] = maxValue;
    currentColour[2] = 0; 
 }
 else if(colourIndex>=maxValue*2 && colourIndex<maxValue*3)
 {
    currentColour[0] = 0;
    currentColour[1] = maxValue;
    currentColour[2] = colourIndex-maxValue*2; 
 }
 else if(colourIndex>=maxValue*3 && colourIndex<maxValue*4)
 {
    currentColour[0] = 0;
    currentColour[1] = maxValue-(colourIndex-maxValue*3);
    currentColour[2] = maxValue; 
 }
 else if(colourIndex>=maxValue*4 && colourIndex<maxValue*5)
 {
    currentColour[0] = colourIndex-maxValue*4;
    currentColour[1] = colourIndex-maxValue*4;
    currentColour[2] = maxValue; 
 }
 else if(colourIndex>=maxValue*5 && colourIndex<maxValue*6)
 {
    currentColour[0] = maxValue;
    currentColour[1] = maxValue-(colourIndex-maxValue*5);
    currentColour[2] = maxValue-(colourIndex-maxValue*5); 
 }
 else if(colourIndex>=maxValue*6 && colourIndex<maxValue*7)
 {
    currentColour[0] = maxValue-(colourIndex-maxValue*6);
    currentColour[1] = 0;
    currentColour[2] = 0; 
 }
 else if(colourIndex>=maxValue*7 && colourIndex<maxValue*8)
 {
    currentColour[0] = (colourIndex-maxValue*7);
    currentColour[1] = 0;
    currentColour[2] = 0; 
 }
 return;
}