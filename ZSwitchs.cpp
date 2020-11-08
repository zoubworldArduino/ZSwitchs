/** @file ZSwitchs.cpp

This class is use to create ros topic that monitor button/switches.

This library is mainly agnostic regarding arduino architecture it should work on all.
if HardwareSerial class is missing, create HardwareSerial that extend the Serial class.
the following define must exist ROS_USED.
*/

#include <ZSwitchs.h>
//#define DEBUG(a) a
/** define to manage debug of lib
*/
#define DEBUG(a) {}



/**
  create the object.
 The default refresh rate 10 hz.
 you have to call setup() once to use it.
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
/** setup a serial channel to output some debug stuff.

*/
void ZSwitchs::setSerialDebug(HardwareSerial * mySerialDebug)
{
  SerialDebug=mySerialDebug;
}

#ifdef ROS_USED 
/** define the refresh rate for the topic.

*/
void ZSwitchs::setRefreshRateUs(
			uint32_t intervalTime//!< refreh period in using
			)
{
	rate=intervalTime;
        timestamp=micros();
}
/** setup :
  After NodeHandle setup, call this to initialize the topic
  The topic is of type std_msgs::UInt32, each bit is link to a button, a 1 means state HIGH, 0 means state LOW.
  
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

As in servo lib, it attach a pin to the object to sent the status of the pin inside the ros topic.

Return the bit location 
  - between 0 to 31
  - -1 means error.
  .

*/
 int ZSwitchs::attach(
 uint32_t buttonPin //!< the pin number or alias to be attach that is connected to the button or ON/OFF sensor

 )
  {
	  if (indexMax>=32)
		  return -1;
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
/** provide the data
*/
uint32_t ZSwitchs::getData()
{
  loop();
  return output;
}
/** loop :
  on loop  before NodeHandle refresh(spinOnce), call this to update the topic 
  the topic is send only if the refresh rate time is reach or if there is a toggle.
  
  call this inside loop() main function.
*/
void ZSwitchs::loop()
{
	
	
	uint32_t oldoutput=output;
for(int i=0;i<indexMax;i++)
{
 volatile uint8_t buttonState = digitalRead(pintable[i]);
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

