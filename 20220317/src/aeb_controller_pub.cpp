#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"
#include "std_msgs/Bool.h"

#include <sstream>

std_msgs::Bool flag_AEB;

void UltraCallBack(const sensor_msgs::Range::ConstPtr& msg)
{
  if(msg->range <= 1.0)
  {
		flag_AEB.data = true;
		ROS_INFO("AEB_Activated_TRUE");
  }
  else
  {
		flag_AEB.data = false;
		ROS_INFO("AEB_Activated_FALSE");
  }
}

int main(int argc, char **argv)
{
  int count=0;
  ros::init(argc, argv, "aeb_controller_pub");
  ros::NodeHandle n;
  ros::Publisher chatter_pub = n.advertise<std_msgs::Bool>("chatter1", 1000);
  ros::Subscriber sub = n.subscribe("range", 1000, UltraCallBack);
  ros::Rate loop_rate(1);
	
  while (ros::ok())
  {
    chatter_pub.publish(flag_AEB);
	loop_rate.sleep();
	ros::spinOnce();
	++count;
  }


  return 0;
}
