//EXAMPLE OF MOTOR WITH ROS.
// do
// roscore &
// for raspberry pi : rosrun rosserial_python serial_node.py /dev/ttyUSB0 &
// or for windows on COM4 : 
//        sudo chmod 666 /dev/ttyS4 if COM4
//        sudo chmod 666 /dev/ttyS24 if COM24
//        rosrun rosserial_python serial_node.py /dev/ttyS4 & 
//        rosrun rosserial_python serial_node.py /dev/ttyS24 & 

//        rostopic list
//        rostopic echo /1/counter 

//        rostopic echo -p /1/counter


// NOTE : WINDOWS 10 with ubuntu 16.04 as subsystem, after ros install do this :
//      sudo apt-get install ros-kinetic-rosserial-windows
//     sudo apt-get install ros-kinetic-rosserial-server

// validated on Captor


#include <Wire.h>
#include <SPI.h>
#include <variant.h>
#include <bootloaders/boot.h>

#if defined(BOARD_ID_Pilo)
#include <Wire.h>
#include <SPI.h>
#include <variant.h>

#include <WireUtility.h>


#define ROS_SERIAL (P_COM3.serial2)
#define ROS_BAUDRATE 57600
#include "ros.h"
ros::NodeHandle  nh;

#elif defined(BOARD_ID_Captor)
#include <Wire.h>
#include <SPI.h>
#include <variant.h>

#include <WireUtility.h>


#define ROS_SERIAL (P_COM0.serial2)
#define ROS_BAUDRATE 57600

#include "ros.h"
ros::NodeHandle  nh;

#else
//#include <Servo.h> 
#include "ros.h"
ros::NodeHandle  nh;
#endif

#include <ZCmdMotor.h>

#define MySerial P_COM0.serial2
#define PcomSerial MySerial









ZSwitchs switches;





void setup()
{/*
MySerial.begin(115200);//9600
MySerial.println("Setup");
   */

 nh.initNode(); 

/*
   MySerial.println("Setup End");
*/



   //wait until you are actually connected
    while (!nh.connected())
    {
      nh.spinOnce();
      delay(10);
    }
    
switches.setup(&nh,"/r1/pilo/switches");
switches.attach(P_ANA1.Pin.IO0);//bleu/noir_rouge : ventouse gauche enforcé
switches.attach(P_ANA1.Pin.IO1);//noir_bleu : ventouse centre enforcé
switches.attach(P_ANA1.Pin.IO2);//noir_vert : ventouse droite enforcé
switches.attach(P_ANA1.Pin.IO3);
switches.attach(P_ANA1.Pin.IO4);//blanc? pond div Vbat
switches.attach(P_ANA1.Pin.IO5);//bleu? = gnd_R
switches.attach(P_ANA1.Pin.IO6);//vert(gnd): bouton arret d'urgence no percuté(robot alimenté)
switches.attach(P_ANA1.Pin.IO7);//jaune: tirette de demarrage presente

switches.attach(P_COM3.Pin.IO0);//bit 9  : noir/bleu_rouge: palet gauche present
switches.attach(P_COM3.Pin.IO1);//bit 10 : fil noir_bleu: palet centre present
switches.attach(P_COM3.Pin.IO5);//bit 11 : fil noir_vert: palet droite present
	
}

void loop()
{
switches.loop();//publish topic if needed
  //  nh.loginfo("loop()");
   nh.spinOnce();
    delay(2);
}




