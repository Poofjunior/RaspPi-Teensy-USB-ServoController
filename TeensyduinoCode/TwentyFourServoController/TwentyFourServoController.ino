/******************************************************************************
  UNTESTED UNTESTED UNTESTED UNTESTED
  
  24-Servo Dedicated USB Controller
  
  Joshua Vasquez
  December 29, 2012
  
  This sketch transforms the TEENSY 2.0 into a USB servo controller, allowing
the user to drive up to 24 servos from a USB interface such as PyUSB.  f
Unfortunately, because of the tight timing, the TEENSY can't do much else
other than drive the servos and listen to the USB input packets.

  Note: this sketch MUST be run on a TEENSY 2.0 to make use of its RawHID
capabilities.
******************************************************************************/

/* Global Variables */
uint8_t USB_Buffer[64];
uint16_t servoPulseWidth[24];

const uint8_t RefreshLoopTime = 25;  // 1/(40 [Hz])

uint32_t currTime = 0;
uint32_t nextTime = 0;
uint32_t startLoopTime = 0;



/* Function Prototypes */
void newDigitalWriteLow( uint8_t pin, boolean highOrLow);
void processNewAngles();




/******************************************************************************
******************************************************************************/
void setup()
{
  // Set all available I/O pins as outputs:
  DDRB = 0xFF;
  DDRC = (1<<6)|(1<<7);
  DDRD = 0xFF;
  DDRF = 0xF1;
}


/******************************************************************************
******************************************************************************/
void loop()
{ 
  // check for new servo commands:
  if (RawHID.recv(USB_Buffer, 0) > 0)
  {
    processNewAngles();
  }
  
  
// the main time loop:
  currTime = micros();
  if (currTime >= nextTime ) 
  {
    // reset nextTime:
    nextTime = micros() + RefreshLoopTime;
    startLoopTime = micros();
    
    // Write all ports high:
    PORTB = 0xFF;
    PORTC |= (1<<6)|(1<<7);
    PORTD = 0xFF;
    PORTF |= 0xF1;
  }
  else
  {
   // check if PWM-signal-ON time is up for any servo, and write it low if so:
   for (uint8_t i=0;i<24;i++)
    {
      if (currTime >= (startLoopTime + servoPulseWidth[i]))
      { // write that port low:
        newDigitalWriteLow(i);
      }
    } 
  }
  
}



/******************************************************************************
void processNewAngles()
  Convert each servo (from 0 through 23) input angle (from 0 to 180) to a value
From 1000 to 2000 (in [us])
******************************************************************************/
void processNewAngles()
{
  // convert each input angle (from 0 through 23) to a time in 
  // microseconds, and place new delay times in servoPulseWidth[];
  for (uint8_t i=0;i<24;i++)
  {
    servoPulseWidth[i] = map(USB_Buffer[i],0,180,1000,2000); 
  }
  
}



/******************************************************************************
void newDigitalWriteLow( uint8_t pin)
    a faster implementation of digitalWrite(pin, LOW) for timing's sake
******************************************************************************/
void newDigitalWriteLow( uint8_t pin)
{
  switch (pin) {
    case 0:
      PORTB &= ~(1<<0);
      break;
    case 1:
      PORTB &= ~(1<<1);
      break;
    case 2:
      PORTB &= ~(1 << 2);
      break;
    case 3:
      PORTB &= ~(1 << 3);
      break;
    case 4:
      PORTB &= ~(1 << 7);
      break;
    case 5:
      PORTD &= ~(1 << 0);
      break;
    case 6:
      PORTD &= ~(1 << 1);
      break;
    case 7:
      PORTD &= ~(1 << 2);
      break;
    case 8:
      PORTD &= ~(1 << 3);
      break;
    case 9:
      PORTC &= ~(1 << 6);
      break;
    case 10:
      PORTC &= ~(1 << 7);
      break;
    case 11:
      PORTD &- ~(1 << 6);
      break;
    case 12:
      PORTD &= ~(1 << 7);
      break;
    case 13:
      PORTB &= ~(1 << 4);
      break;
    case 14:
      PORTB &= ~(1 << 5);
      break;
    case 15:
      PORTB &= ~(1 << 6);
      break;
    case 16:
      PORTF &= ~(1 << 7);
      break;
    case 17:
      PORTF &= ~(1 << 6);
      break;
    case 18:
      PORTF &= ~(1 << 5);
      break;
    case 19:
      PORTF &= ~(1 << 4);
      break;
    case 20:
      PORTF &= ~(1 << 1);
      break;
    case 21:
      PORTF &= ~(1 << 0);
      break;
    case 22:
      PORTD &= ~(1 << 4);
      break;
    case 23:
      PORTD &= ~(1 << 5);
      break;
    return;
  }
    
}
