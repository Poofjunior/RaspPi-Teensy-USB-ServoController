# JVasquez
# December 23, 2012
# Python Teensyduin HID test

# A couple of notes: 
#   To find the 1st argument of Teensy.read( arg, arg), I used 
#     lsusb -vv >lsusbOut.txt
#   and looked for the "bEndpointAddress EP 3 IN" for the Teensy.
#   In the documentation, it looks like there are 4 (?) endpoints, but we're 
#   interested in the rawHID endpoint, not the "Emulated Arduino Serial" 
#   endpoint.
#   For the second argument, we need to know how much data is being output from
#   the Teensy, which is 64 bytes.
#
#
#   To find 1st argument of Teensy.write(arg, arg), we're lookin for the 
#   "bEndpoint Address" for the rawHID device (not the Emulated Arduino
#   Serial address).  Since we're writing out to the Teensy, we're looking for 
#   the OUT address.
#


import usb.core
import usb.util
import time

# find the Teensy.  (idVendor and idProduct are set in the Teensy Code.)

interface = 0

DATA_SIZE = 64
VENDOR_ID = 0x16C0
PRODUCT_ID = 0x0486	# Arduino Version

Teensy = usb.core.find(idVendor = VENDOR_ID, idProduct = PRODUCT_ID)


if Teensy is None:
	raise ValueError( 'Teensy not found')



if Teensy.is_kernel_driver_active(interface) is True:
        print "but we need to detach kernel driver"
        Teensy.detach_kernel_driver(interface)
        print "claiming device"
        usb.util.claim_interface(Teensy, interface)


# 0x81 is the type of endpoint (interrupt-based, in our case)
# 64 is the number of bytes we want back from the Teensy

# Make some dummy buffer of zeros.
outBuffer = [0]*64

while True:
	time.sleep(1)
# send over the dummy buffer and print the Teensy's reply:
	Teensy.write(0x04,outBuffer)
	reply = Teensy.read(0x83, 64)
	print reply

