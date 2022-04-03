#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"

int range_count = 0;
float sum = 0.0;
std_msgs::Float32 range_avg ;
void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
	if(range_count < 5.0)
	{
		sum += msg->range;
		range_count++;
	}
	else
	{
		range_avg.data = sum / 5;
		range_count = 0;
		sum = 0;
		ROS_INFO("range_average: [%.2f] \n",range_avg.data);
	}
	ROS_INFO("Sonar Range: [%f]", msg->range);
}

int main(int argc, char **argv)
{
	int count = 0;
	ros::init(argc, argv, "sonar_avg_filter");
	ros::NodeHandle n;
	ros::Rate loop_rate(10); //10
	ros::Subscriber sub = n.subscribe("range", 100,UltraSonarCallback);
	ros::Publisher pub =  n.advertise<std_msgs::Float32>("/range_avg",10);
	
	while(ros::ok())
	{
		pub.publish(range_avg);
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	return 0;
}
© 2022 GitHub, Inc.
Terms
Privacy
Security
Status
Docs
Contact GitHub
Pricing
API
Training
Blog
About
