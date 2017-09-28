#ifndef apa102LEDStrip_h
#define apa102LEDStrip_h

class apa102LEDStrip
{
  public:
	apa102LEDStrip();
	void newBlock(unsigned short int numLEDs, char bytesPerLED, char globalBrightness);
    void setPixel(unsigned short int pixelIndex, char *pixelColour);
    void getPixel(unsigned short int pixelIndex, char *pixelColour);

	unsigned char* LEDs;
	
    char _globalBrightness;
    int _frameLength;
    short int _endFrameLength;
    short int _numLEDs;
    char _bytesPerLED;
    short int _counter;
	
  private:
};

#endif
