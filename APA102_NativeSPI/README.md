SPIDEV Device
On line 29 make sure you define the correct SPI Device
    
    const char *device = "/dev/spidev0.0";


Compile with:

    g++ -o apatest APA102_NativeSPI.cpp apa102LEDStrip.cpp
