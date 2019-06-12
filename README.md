# ZSwitchs
Arduino  Lib for buttons and switches with ros.

The main advantage is to manage up to 32 button on 1 ros message uint_32.

This basic code is :

ZSwitchs switches;
void setup()
{
switches.setup(&nh,"/r1/pilo/switches");
switches.attach(P_ANA1.Pin.IO0);// attach the pin to bit 0
switches.attach(P_ANA1.Pin.IO1);// attach the pin to bit 1
//... up to 32 pin befor to instanciate an other class.
switches.setRefreshRateUs(1000);// refresh each milli second
}

void loop()
{
switches.loop();//publish topic if needed

}

on ros you can do
roscore &
rosrun rosserial_python serial_node.py /dev/ttyS4 
rostopic info "/r1/pilo/switches"
rostopic echo "/r1/pilo/switches"
rostopic hz rostopic info "/r1/pilo/switches"


so you get a message at refresh rate, and on edge.
see refman.pdf for advance doc.