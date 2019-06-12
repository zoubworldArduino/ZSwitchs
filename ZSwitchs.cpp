/** @file ZSwitchs.cpp

*/

#include <ZSwitchs.h>
//#define DEBUG(a) a
#define DEBUG(a) {}



/**
 * This will enumerate a GroveEncoder on a particular pin.
 * You can provide an optional callback, or poll the "getValue()" API.
 * mode equal to QUARTER count 4 for a cycle(1 per phase), equal to full it count 1 per cycle(less accurate, but support high frequency).
 */
ZSwitchs::ZSwitchs( ) {
#ifdef ROS_USED 
    nh=0;
    pub_output=0;
    
    rate=100000;//100ms
#endif
output=0;
SerialDebug=0;
  if (SerialDebug) 
  {
    SerialDebug->print("ZSwitchs(");
    SerialDebug->print(") : ");
  }
}

void ZSwitchs::setSerialDebug(HardwareSerial * mySerialDebug)
{
  SerialDebug=mySerialDebug;
}

#ifdef ROS_USED 

void ZSwitchs::setRefreshRateUs(uint32_t intervalTime)
{
	rate=intervalTime;
        timestamp=micros();
}
/** setup :
  At setup after NodeHandle setup, call this to initialise the topic
*/
void ZSwitchs::setup( ros::NodeHandle * myNodeHandle,	const char   *	topic)
{
  nh=myNodeHandle;
  pub_output=new ros::Publisher(topic, &output_msg);
  
  nh->advertise(*pub_output); 
  DEBUG(nh->loginfo("ZSwitchs::setup()")); 
  DEBUG(nh->loginfo(topic));
  
}
/** define a pin as a switch

return the bit location.

*/
 int ZSwitchs::attach(uint32_t buttonPin)
  {
  pinMode(buttonPin, INPUT_PULLUP);
  uint8_t buttonState = digitalRead(buttonPin);
  if (buttonState==HIGH)
    output|=1<<indexMax;
  else
    output&=~(1<<indexMax);
  
    pintable[indexMax]=buttonPin;
   uint32_t index=indexMax;
    indexMax++;
return index;
}

/** loop :
  on loop  before NodeHandle refresh(spinOnce), call this to update the topic
*/
void ZSwitchs::loop()
{
	
	
	uint32_t oldoutput=output;
for(int i=0;i<indexMax;i++)
{
 uint8_t buttonState = digitalRead(pintable[i]);
  if (buttonState==HIGH)
    output|=1<<i;
  else
    output&=~(1<<i);
}
	

  if(pub_output!=0)
  {
    unsigned long duree=micros();
 if(((duree)>timestamp) || (oldoutput!=output))
  {
      output_msg.data = output;
      pub_output->publish(&output_msg);
        
        
      timestamp+=rate;
      if (timestamp<micros())
        timestamp=micros()+rate;
  }
  }
}
#endif 

