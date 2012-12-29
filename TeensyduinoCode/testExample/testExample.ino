 /* Basic Raw HID Example
   Teensy can send/receive 64 byte packets with a
   dedicated program running on a PC or Mac.

   You must select Raw HID from the "Tools > USB Type" menu

   Optional: LEDs should be connected to pins 0-7,
   and analog signals to the analog inputs.-

   This example code is in the public domain.
*/
#include <Servo.h>



// RawHID packets are always 64 bytes
byte buffer[64];

Servo servo;


void setup() {
  DDRD |= (1 << 6);  // make PD6 (the LED) an output.
  
  servo.attach(0);  
  servo.write(0);  

}



void loop() {
  if ( RawHID.recv(buffer, 0) > 0) // 0 timeout = do not wait
  {
    PORTD ^= (1 << 6);  // Toggle the pin 
    
    switch ( (PIND >> 6) & 0x01 ){
      case 0:
        servo.write(0);
        break;
      case 1:
        servo.write(30);
        break;
      default:
        servo.write(30);
  }
  }
    
  // every 1 second, send a packet to the computer
  
    // load buffer again: 
    for (uint8_t i=0;i<64;i++)
    {
      buffer[i] = i; 
    }
    
    // send the packet
    RawHID.send(buffer, 100);
}
