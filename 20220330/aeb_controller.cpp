#include "ros/ros.h"
#include "std_msgs/Bool.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/Twist.h"
#include "nav_msgs/Odometry.h"
#include <math.h>

#define frequency_odom_pub 50

geometry_msgs::Twist cmd_vel_msg;
nav_msgs::estimated_odom;
std_msgs::Bool flag_AEB;
std_msgs::Float32 delta_range;
std_msgs::Float32 old_sonar_range;
std_msgs::Float32 delta_t;

float x,y;
float delta_x, delta_y;
float vx, vy;
float aeb_collision_distance  = 200;
float dis = 0;
float DistanceToStop = 3.0; 


void odomCallback(const nav_msgs::Odometry& msg)
{ 
	estimated_odom.twist.twist.linear.x = msg.twist.twist.linear.x;
	estimated_odom.twist.twist.linear.y = msg.twist.twist.linear.y;
	estimated_odom.twist.twist.linear.z = msg.twist.twist.linear.z;
	
	estimated_odom.twist.twist.angular.x = msg.twist.twist.angular.x;
	estimated_odom.twist.twist.angular.y = msg.twist.twist.angular.y;
	estimated_odom.twist.twist.angular.z = msg.twist.twist.angular.z;
	
		
	float old_x, old_y;
	old_x = x;
	old_y = y;
	ROS_INFO("POS : x:[%.2lf] y:[%.2lf]",msg.pose.pose.position.x, msg.pose.pose.position.y);
	
	x = msg.pose.pose.position.x;
	y = msg.pose.pose.position.y;
	
	delta_x = x-old_x;
	delta_y = y-old_y;
	
	vx =delta_x * frequency_odom_pub;
	vx =delta_y * frequency_odom_pub;
	
	aeb_collision_distance = 4.0 - (vx * (0.7 + 0.1 ) * 0.22778 * 2.5);
	
	if(delta_x > 0){
		dis +=delta_x;
	 }
	else if(delta_x<0){
	   dis += -delta_x;
	 }
	ROS_INFO("delta_x: %0.2f", dis);

	if(dis >=aeb_collision_distance)
	{
		ROS_INFO("AEB_Activated");
		flag_AEB.data = true;
	}
	else {
		flag_AEB.data = false;
	}
}

/*void OdomCallback(const nav_msgs::Odometry& msg)
{
	float old_x, old_y;
	old_x = x;
	old_y = y;
	
	//ROS_INFO("%.2lf %.2lf", msg.pose.pose.position.x, msg.pose.pose.position.y);
	
	x = msg.pose.pose.position.x;
	y = msg.pose.pose.position.y;
	
	delta_x = x - old_x;
	delta_y = y - old_y;
	
	vx = delta_x * frequency_odom_pub;
	vy = delta_y * frequency_odom_pub;
	
	//ROS_INFO("delta x = [%.2lf]", vx);
	//ROS_INFO("delta y = [%.2lf]", vy);
	
	aeb_collision_distance = vx*(0.7+0.1)*0.22778*8;
	
	if(delta_x > 0)
	{		
		if(x == (DistanceToStop + aeb_collision_distance))
		{
			cmd_vel_msg.linear.x = 0;
			ROS_INFO("AEB Activated.");
		}
	}	
}*/

void UltraSonarCallback(const sensor_msgs::Range::ConstPtr& msg) 
{
	ROS_INFO("Soner Seq: [%d]", msg->header.seq);
	ROS_INFO("Soner Range: [%f]", msg->range);	
	
	/*aeb_collision_distance = vx*(0.7+0.1)*0.22778*3;
	if(msg->range <= aeb_collision_distance)
	{
		ROS_INFO("AEB_Activated");
		flag_AEB.data = true;
	}
	else {
		flag_AEB.data = false;
	}*/
	//delta_range.data = old_sonar_range.data - msg->range;
	//ROS_INFO("delta range : %f", delta_range.data);
	//old_sonar_range.data = msg->range;
	
}

void CarControlCallback(const geometry_msgs::Twist& msg) 
{
	cmd_vel_msg = msg;
	ROS_INFO("Cmd_vel : linear x [%f]", cmd_vel_msg.linear.x);
}

int main(int argc, char **argv) {
	int count = 0;
	
	old_sonar_range.data = 0;
	
	std::string odom_sub_topic="/ackermann_steering_controller/odom";
	
	ros::init(argc, argv, "aeb_controller");
	ros::NodeHandle n;
	ros::Rate loop_rate(1);
	
	ros::Subscriber sub = n.subscribe("/range", 1000, UltraSonarCallback);
	ros::Subscriber sonar_sub = n.subscribe("/RangeSonar1", 1000, UltraSonarCallback);
	ros::Subscriber cmd_vel_sub = n.subscribe("/cmd_vel", 10, &CarControlCallback);
	ros::Subscriber sub_odom = n.subscribe(odom_sub_topic, 10, &OdomCallback);
	
	
	
	ros::Publisher pub_aeb_activation_flag = n.advertise<std_msgs::Bool>("/aeb_activation_flag", 1);
	ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel",10);
	ros::Publisher pub_delta_range = n.advertise<std_msgs::Float32>("/delta_range",1);
	ros::Publisher pub_estimated_odom = n.advertise<nav_msgs::Odometry>("/estimated_odom");
	
	
	while (ros::ok()) {
		if((count%1)==0)
		{
			pub_aeb_activation_flag.publish(flag_AEB);
		}
		
		if(flag_AEB.data == true)
		{
			if(cmd_vel_msg.linear.x>0) {
				cmd_vel_msg.linear.x = 0;}
				
			pub_cmd_vel.publish(cmd_vel_msg);
		}
		else
		{
			pub_cmd_vel.publish(cmd_vel_msg);
		}
		
		//ROS_INFO("Collision Distance : %6.3f", aeb_collision_distance);
		
		loop_rate.sleep();
		ros::spinOnce();
		++count;
	}
	return 0;
}
