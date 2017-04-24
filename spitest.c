#include <stdio.h>
#include "libfahw.h"


const short int numLeds = 12;
const unsigned char bytesPerLED = 4;
const short int frameLength = 56;
unsigned char LEDs[56];              
short int ledIndex=0;
const unsigned char  maxValue=64;                    //Maximum value to be used PER COLOUR on each Pixel
unsigned char  maxBrightness = 255;//255-15;            //Maximum Globbal Brightness Level (range is 225-254)
unsigned short int colIndex=0;                //Used to store compressed colour index
unsigned char  currentCol[3] = {0,0,0};        //Colour array
const short int rainbowSize = 384;  //Maximum number of individual colours based on the maxValue
//SPI Device
int devFD;
static unsigned char spiMode = 0;
static unsigned int spiSpeed = 5000000;

void initLEDs();
void initSPI();
void renderLEDs();
void getColour(short int colourIndex, unsigned char *currentColour);


int main(int argc, char *argv[]) 
{
	initLEDs();
	initSPI();
	renderLEDs();
	while(1)
	{
		getColour(colIndex, currentCol);
		LEDs[4+1] = currentCol[2];
		LEDs[4+2] = currentCol[1];
		LEDs[4+3] = currentCol[0];
		for(ledIndex=frameLength-bytesPerLED; ledIndex>4; ledIndex-=bytesPerLED)
		{
			LEDs[ledIndex+1] = LEDs[ledIndex-bytesPerLED+1];
			LEDs[ledIndex+2] = LEDs[ledIndex-bytesPerLED+2];
			LEDs[ledIndex+3] = LEDs[ledIndex-bytesPerLED+3];
		}
		//check if colIndex greater than or equal to the maximum volour range value if so reset it to 0 else increment ot get next colour
		if(colIndex>=rainbowSize)
		{
			 colIndex=0;
		}
		else
		{
			colIndex+=2;
		} 
		//renred LED data to LED strip
		renderLEDs();
		usleep(5000);
	}
	
}

void renderLEDs()
{
    writeBytesToSPI(devFD, LEDs, frameLength, 0, spiSpeed, 8);
}

void initSPI()
{
	boardInit();
	if ((devFD = openHW(SPI0_PATH, O_RDWR)) < 0)
	{
        //setLastError("Fail to open SPI device MAX7219");
    }
	setSPIDataMode(devFD, spiMode);
	setSPIWriteBitsPerWord(devFD, 8);
	setSPIMaxSpeed(devFD, spiSpeed);	
}

void initLEDs()
{
	//Initialize the Start Frame
  LEDs[0] = 0;
  LEDs[1] = 0;
  LEDs[2] = 0;
  LEDs[3] = 0;
  //Clear out the LED data Frames
  for(ledIndex=bytesPerLED; ledIndex<frameLength-1; ledIndex+=bytesPerLED)
  {
    LEDs[ledIndex] = maxBrightness;
    LEDs[ledIndex+1] = 0;
    LEDs[ledIndex+2] = 0;
    LEDs[ledIndex+3] = 0;
  }
  //Initialize the end frames
  for(ledIndex=frameLength-1; ledIndex<frameLength; ledIndex+=bytesPerLED)
  {
    LEDs[ledIndex] = 255;
    LEDs[ledIndex+1] = 255;
    LEDs[ledIndex+2] = 255;
    LEDs[ledIndex+3] = 255;
  }
}

void getColour(short int colourIndex, unsigned char *currentColour)
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
 return;
}