#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int8.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"

geometry_msgs::Twist cmd_vel_msg;
std_msgs::Bool flag_AEB;
std_msgs::Float32 delta_range;
std_msgs::Float32 before_range;
nav_msgs::Odometry pose, delta_pose, before_pose;
nav_msgs::Odometry v;


void odomCallback(const nav_msgs::Odometry& msg)
{
	pose.pose.pose.position.x = msg.pose.pose.position.x;
	pose.pose.pose.position.y = msg.pose.pose.position.y;
	
	delta_pose.pose.pose.position.x = pose.pose.pose.position.x - before_pose.pose.pose.position.x;
	delta_pose.pose.pose.position.y = pose.pose.pose.position.y - before_pose.pose.pose.position.y;
	
	before_pose.pose.pose.position.x = msg.pose.pose.position.x;
	before_pose.pose.pose.position.y = msg.pose.pose.position.y;
	
	
		
	v.twist.twist.linear.x = delta_pose.pose.pose.position.x / 0.02;
	v.twist.twist.linear.y = -delta_pose.pose.pose.position.y / 0.02;
	

	
}


void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg) 
{
	ROS_INFO("Soner Seq: [%d]", msg->header.seq);
	ROS_INFO("Soner Range: [%f]", msg->range);	
	
	if(msg->range <= 1.0)
	{
		ROS_INFO("AEB_Activated");
		flag_AEB.data = true;
	}
	else {
		flag_AEB.data = false;
	}
	delta_range.data = before_range.data-msg->range;
	ROS_INFO("DELTA SONAR RANGE:[%f]",delta_range.data);
	before_range.data= msg->range;
}

void CarControlCallback(const geometry_msgs::Twist& msg) 
{
	ROS_INFO("Cmd_vel :linear x [%f}", msg.linear.x);
	
	cmd_vel_msg = msg;
	ROS_INFO("Cmd_vel : linear x [%f]", cmd_vel_msg.linear.x);
	
}

void UltraSonarCallback2(const sensor_msgs::Range::ConstPtr& msg)
{
	ROS_INFO("Sonar2 Seq: [%d]",msg->header.seq);
	ROS_INFO("Sonar2 Range:[%f]",msg ->range);
}

int main(int argc, char **argv) {
	int count = 0;
	
	before_pose.pose.pose.position.x=0.0;
	before_pose.pose.pose.position.y=0.0;
	v.twist.twist.linear.x =0.0;
	v.twist.twist.linear.x =0.0;

	ros::init(argc, argv, "aeb_controller");
	ros::NodeHandle n;
	ros::Rate loop_rate(10);
	
	
	ros::Publisher pub_aeb_activation_flag = n.advertise<std_msgs::Bool>("/aeb_activation_flag", 1);
	ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel",10);
	ros::Publisher delta_range_pu = n.advertise<std_msgs::Float32>("/delta_range", 1000);
	ros::Publisher pub_v = n.advertise<nav_msgs::Odometry>("/v",10);
	
	ros::Subscriber sub = n.subscribe("range", 1000, UltraSonarCallback); 
	ros::Subscriber sonar_sub = n.subscribe("/RangeSonar1",1000,UltraSonarCallback2);
	ros::Subscriber cmd_vel_sub = n.subscribe("/cmd_vel", 10, &CarControlCallback);
	ros::Subscriber sub_odom = n.subscribe("odom_sub_topic",10,odomCallback);
	
	
	
	std::string odom_sub_topic = "/ackermann_steering_controller/odom";
	
	
	while (ros::ok()) {
		if((count%10)==0)
		{
			pub_aeb_activation_flag.publish(flag_AEB);
		}
		
		if(flag_AEB.data == true)
		{
			cmd_vel_msg.linear.x = 0;
			pub_cmd_vel.publish(cmd_vel_msg);
		}
		else
		{
			pub_cmd_vel.publish(cmd_vel_msg);
		}
		
		pub_v.publish(v);
		
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	return 0;
}
