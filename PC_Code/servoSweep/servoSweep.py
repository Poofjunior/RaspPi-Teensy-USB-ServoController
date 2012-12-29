# JVasquez
# December 23, 2012
# Python Teensyduino HID test

# This code simply sweeps a servo back and forth 180 degrees via a usb 
# connnection (provided by the Teensy.)
# Note that the servo is connected to the Teensy's digital pin 5.


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

DATA_SIZE = 64
VENDOR_ID = 0x16C0
PRODUCT_ID = 0x0486	# Arduino Version

class TeensyInterface:
	def __init__(self, dataSize, vendorId, productId):
		self.interface = 0
		self.dataSize = dataSize
		# find the Teensy.  (idVendor and idProduct are set in the Teensy Code.)
		self.Teensy = usb.core.find(idVendor = vendorId, idProduct = productId)
		if self.Teensy is None:
			raise ValueError( 'Teensy not found')
		if self.Teensy.is_kernel_driver_active(self.interface) is True:
      			print "but we need to detach kernel driver"
      			self.Teensy.detach_kernel_driver(self.interface)
      			print "claiming device"
     			usb.util.claim_interface(self.Teensy, self.interface)
      
	def run(self):
		# 0x81 is the type of endpoint (interrupt-based, in our case)
		# 64 is the number of bytes we want back from the Teensy
		# Make some dummy buffer of zeros.
		outBuffer = [0]*self.dataSize
		while True:
			for angle in range(0,180):
				time.sleep(.005)
			# digital pin 5 data goes in the 5th index of the array
				outBuffer[5] = angle #bytes(angle)
				self.Teensy.write(0x04,outBuffer)
			for angleBack in range(180,0,-1):
				time.sleep(.005)
				outBuffer[5] = angleBack #bytes(angleBack)
				self.Teensy.write(0x04,outBuffer)
				

if __name__ == "__main__":
	teensy = TeensyInterface(DATA_SIZE,VENDOR_ID,PRODUCT_ID)
	teensy.run()
