/** @file ZSwitchs.h

An library that manage switches(up to 32 by object/topic) and generate a ros topic std_msgs/UInt32.


*/
#ifndef _PZSwitchs_H
#define _PZSwitchs_H

#include <Arduino.h>
#include <assert.h>
#define ROS_USED 1
#ifdef ROS_USED 
#include <ros.h>
#include <ros/time.h>
#include <std_msgs/UInt32.h>
   
#endif 

/*==================================================================================================*/
/*==================================================================================================*/
/*==================================================================================================*/

class ZSwitchs {

public:
  //  ZEncoder(int pinA,int pinB, void (*optionalCallBack)(int));
  /** constructor 
  **/
  ZSwitchs(
  );
  /** setup a debug channel to have output on serial
  the drawback it that it waste cpu cycle, and you an lost some tick
  but usefull for debug.
  */
  void setSerialDebug(HardwareSerial * SerialDebug);
   int ZSwitchs::attach(uint32_t buttonPin);
//@{
	/** setup the refresh rate of the topic speed
	*/
  void setRefreshRateUs(uint32_t intervalTime //!< duration between 2 topic in Micro Seconde
  );  
#ifdef ROS_USED 
   /** @name ROS IPA
*/

  /** the ros initialisation	
	*/
    void setup( ros::NodeHandle  *myNodeHandle//!< the ROS node handler
	,	const char   *	topic//!< the topic for position displayed in ROS
	);
	
	/** function to be called in your main loop.
	*/
	void loop();
//@}
#endif
 
  private:
    uint32_t rate;
  
    uint32_t pintable[32];
  uint32_t indexMax=0;
  uint32_t output=0;
  
#ifdef ROS_USED 
    ros::NodeHandle  *nh;
    std_msgs::UInt32 output_msg;//speed //deltaD//D
    ros::Publisher *pub_output;
    unsigned long timestamp;
   
  
#endif
  HardwareSerial * SerialDebug;

};
#endif
