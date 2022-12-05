#include <Wire.h>
#include <BreezyArduCAM.h>
#include <SPI.h>

static const int CS = 5;

Serial_ArduCAM_FrameGrabber fg;

/* Choose your camera */
ArduCAM_Mini_2MP myCam(CS, &fg);
//ArduCAM_Mini_5MP myCam(CS, &fg);

void setup(void) 
{
    // ArduCAM Mini uses both I^2C and SPI buses
    Wire.begin();
    SPI.begin();

    // Fastest baud rate (change to 115200 for Due)
    Serial.begin(921600);

    // Start the camera in JPEG mode with a specific image size
    myCam.beginJpeg320x240();
}

void loop(void) 
{
    myCam.capture();
}
