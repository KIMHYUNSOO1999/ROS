#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"
#include "std_msgs/Bool.h"

std_msgs::Bool flag_AEB;

void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  ROS_INFO("Sonar Seq: [%d]", msg->header.seq);
  ROS_INFO("Sonar Range: [%f]", msg->range);
}

void UltraSonarCallback_truefalse(const std_msgs::Bool::ConstPtr&msg)
{
  if(msg->data == true)
	{
		ROS_INFO("AEB_Activated");
	}
	else {
		ROS_INFO("NOT_AEB_Activated");
	}
}

int main(int argc, char **argv)
{
	int count = 0;
	
	ros::init(argc, argv,"aeb_controller");
	
	ros::NodeHandle n;
	
	ros::Rate loop_rate(1);
	
	ros::Subscriber sub = n.subscribe("range", 1000, UltraSonarCallback);
	
	ros::Subscriber sub1 = n.subscribe("chatter1", 1000, UltraSonarCallback_truefalse);
	
	while(ros::ok())
	{
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	
	return 0;
}
