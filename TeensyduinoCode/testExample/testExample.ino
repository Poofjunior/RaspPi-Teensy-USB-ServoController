 /* 
   Teensyduino USB servo controller Example
*/
#include <Servo.h>



// RawHID packets are always 64 bytes
byte buffer[64];

Servo* servos[12];

// Stuff servo default positions here:
uint8_t defaultPos[12] = {0,0,0,0,0,90,0,0,0,0,0,0};  
uint8_t servoPin[12] = {0,1,2,3,4,5,6,7,8,9,10,12}; // skip pin 11;


void setup() {
  DDRD |= (1 << 6);  // make PD6 (the LED) an output.
  
  // attach 12 servos and set them to the default position:

  for (uint8_t i=0;i<12;i++)
  {
    servos[i] = new Servo;
    servos[i]->attach(servoPin[i]);  
    servos[i]->write(defaultPos[i]);  
  }
  delay(1500);
}



void loop() {
  if ( RawHID.recv(buffer, 0) > 0) // 0 timeout = do not wait
  {
    PORTD ^= (1 << 6);  // Toggle the pin 
    
    switch ( (PIND >> 6) & 0x01 ){
      case 0:
        servos[5]->write(0);
        break;
      case 1:
        servos[5]->write(30);
        break;
      default:
        servos[5]->write(30);
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
