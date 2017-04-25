# NanoPi_APA102
A simple SPI driver for the APA102/SK9822 Pixel Shipset on the NanoPi Neo Air board

Hardware you need:
- NanoPi Neo Air board with WIFI Antena connected http://www.friendlyarm.com/index.php?route=product/product&path=69&product_id=151
- A Serial link from your pc to the Nano pi Air http://www.friendlyarm.com/index.php?route=product/product&product_id=178
- An 8GB or larger Micro SD Card

Steps to get the board Running:
1. Download the eflasher image from the friendly arm wiki site located at https://www.mediafire.com/folder/sr5d0qpz774cs/NanoPi-NEO_Air in the Official_ROMS folder named nanopi-air-eflasher-sd8g-20170220.img.zip
2. Extract the IMG file
3. Format the MicroSD card to 1 Fat32 partition and make sure you can write to it
4. Use Win32Disk imager https://sourceforge.net/projects/win32diskimager/ to write the image to the MicroSD Card
5. Insert the MicroSD card into the NanoPi Board, power it up, and connect it to your PC via serial link
6. Open the serial port @ 115200 speed
7. Login as U:root P:fa <-these are the image defaults
8. type in "eflasher", select the system you want to burn(there is only 1) and burn it. This will load the OS onto the onboard 8gb EMMC chip. Once completed, itw ill tell you to power off. Disconnect the Serial link(as it will keep the board powered), then power off the boards power link
9. Remove the SD card, power it on and reconect the Serial link log on again
10. Set up WIFI as per http://wiki.friendlyarm.com/wiki/index.php/NanoPi_NEO_Air#Wireless_Connection then shutdown the board, disconect the serial link and power off
11. Power it on and connect via SSH to verify its online

Steps to installing the C Hardware library provided by Friendly Arm https://github.com/friendlyarm/matrix
1. Log in via SSH as U: root P: fa , feel free to change the root password :)
2. run 
  apt-get update
3. run
  apt-get upgrade
4. If you want FTP acces for file uploading run
  apt-get install pure-ftpd
5. Install GIT by running 
  apt-get install git
6. Clone the matrix library
  git clone https://github.com/friendlyarm/matrix.git
7. CD into the directory 
  cd matrix
8. Compile and install the matrix library
  make && make install

At this stage you can try out any of the Matrix demos and see how to use the GPI and SPI port.
NOTE YOU MUST RUN ANYTHING YOU COMPILE AS ROOT OR SUDO

To get the APA driver "spitest.c" to ccompile an test it that it works:
-You will need a small strip of APA102 pixels, this demo tests 12 pixels
-A seperate 5V pwoer supply

1. Connect up the LEDs as shown in the diagram https://github.com/leonyuhanov/NanoPi_APA102/blob/master/nanopiapa102.jpg
2. make sure the GND of the LEDs, the GND of the external power supply and the GND of the NanoPi are SHARED
3. Create a folder on the nano pi and save "spitest.c" inside
4. copy the matrix/lib folder behind the project folder
5. compile in G++ using
    g++ -L/usr/local/lib spitest.c -o spitest -lfahw -I ../lib/includes/
6. run the compiled program as 
  "sudo spitest"
