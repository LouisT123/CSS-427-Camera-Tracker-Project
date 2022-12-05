import time
import serial
import cv2

from helpers import *

from sys import stdout

def getack(port):
    stdout.write(port.readline().decode())

def sendbyte(port, value):
    port.write(bytearray([value]))

def ackcheck(port, msg):
    line = port.readline().decode()
    assert(msg in line)

# Modifiable params --------------------------------------------------------------------

object_detector = cv2.createBackgroundSubtractorMOG2(history=100, varThreshold=40)

#PORT = '/dev/ttyACM0' # Ubuntu
PORT = 'COM3'         # Windows

BAUD = 921600       # Change to 115200 for Due


# main ------------------------------------------------------------------------------

if __name__ == '__main__':

    # Open connection to Arduino with a timeout of two seconds
    port = serial.Serial(PORT, BAUD, timeout=2)

    # Report acknowledgment from camera
    getack(port)

    # Wait a spell
    time.sleep(0.2)

    # Send start flag
    sendbyte(port, 1)

    # We'll report frames-per-second
    start = time.time()
    count = 0

    # Loop over images user hits ESC
    done = False
    while not done:

        # Open a temporary file that we'll write to and read from
        tmpfile = open("tmp.jpg", "wb")

        # Loop over bytes from Arduino for a single image
        written = False
        prevbyte = None
        while not done:

            # Read a byte from Arduino
            currbyte = port.read(1)

            # If we've already read one byte, we can check pairs of bytes
            if prevbyte:

                # Start-of-image sentinel bytes: write previous byte to temp file
                if ord(currbyte) == 0xd8 and ord(prevbyte) == 0xff:
                    tmpfile.write(prevbyte)
                    written = True

                # Inside image, write current byte to file
                if written:
                    tmpfile.write(currbyte) 

                # End-of-image sentinel bytes: close temp file and display its contents
                if ord(currbyte) == 0xd9 and ord(prevbyte) == 0xff:
                    tmpfile.close()
                    img = cv2.imread("tmp.jpg")
                    #image = cv2.imread("tmp.jpg")
                    #ret, img = image.read()
                    height, width, _ = img.shape
                    mask = object_detector.apply(img)
                    #_, mask - cv2.threshold(mask,254,255,cv2.THRESH_BINARY)

                    contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
                    for cnt in contours:
                        area = cv2.contourArea(cnt)
                        if area > 200:
                            cv2.drawContours(img, [cnt], -1, (0,255,0), -1)
                            x,y,w,h = cv2.boundingRect(cnt)
                            cv2.rectangle(img, (x,y), (x+w, y+h), (0,255,0),3)
                    
                    try:
                        cv2.imshow("Mask", mask)
                        cv2.imshow("ArduCAM [ESC to quit]", img)
                        
                    except:
                        pass
                    if cv2.waitKey(1) == 27:
                        done = True
                        break
                    count += 1
                    break

            # Track previous byte
            prevbyte = currbyte

    # Send stop flag
    sendbyte(port, 0)

    # Report FPS
    elapsed = time.time() - start
    print('%d frames in %2.2f seconds = %2.2f frames per second' % (count, elapsed, count/elapsed))
  
    # Close the window
    cv2.destroyAllWindows()