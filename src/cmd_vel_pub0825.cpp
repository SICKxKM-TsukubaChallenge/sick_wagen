
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

geometry_msgs::Twist cmd_vel;
geometry_msgs::Twist sub_cmd_vel;
geometry_msgs::Twist cmd_vel_nav;
geometry_msgs::Twist cmd_vel_joy;
geometry_msgs::Twist former_cmd_vel_joy;
geometry_msgs::Twist cmd_vel_whill;
sensor_msgs::Joy joy_msg;

void nav_callback(const geometry_msgs::Twist& cmd_vel_nav)
{
//	sub_cmd_vel = cmd_vel_nav;
	sub_cmd_vel.linear.x = cmd_vel_nav.linear.x;
	sub_cmd_vel.angular.z = cmd_vel_nav.angular.z;
}

void joy_callback(const sensor_msgs::Joy& joy_msg)
{
	cmd_vel_joy.linear.x = joy_msg.axes[3]*0.9 + joy_msg.buttons[4];//-(joy_msg.axes[3] * 1.5 - former_cmd_vel_joy.linear.x)/0.1;
	cmd_vel_joy.angular.z = joy_msg.axes[2]*1.0;//-(joy_msg.axes[2] * 1.3 - former_cmd_vel_joy.angular.z)/0.1;
	//former_cmd_vel_joy = cmd_vel_joy;
}

void whill_callback(const sensor_msgs::Joy& whill_joy_msg)
{
	cmd_vel_whill.linear.x = whill_joy_msg.axes[1];
	cmd_vel_whill.angular.z = whill_joy_msg.axes[0];
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "gamepad_pub");
	ros::NodeHandle nh;
	ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1000);
	ros::Subscriber move_base_cmd_vel_sub = nh.subscribe("/cmd_vel_move_base", 10, nav_callback);
	ros::Subscriber gamepad_joy_sub = nh.subscribe("/joy", 10, joy_callback);
	ros::Subscriber whill_joy_sub = nh.subscribe("/whill/states/joy", 10, whill_callback);

	ros::Rate loop_rate(10);
	while (ros::ok())
	{
		if(cmd_vel_whill.linear.x != 0 || cmd_vel_whill.angular.z != 0){
			cmd_vel = cmd_vel_whill;
		}else
		{
			if(cmd_vel_joy.linear.x != 0 || cmd_vel_joy.angular.z != 0){
				cmd_vel = cmd_vel_joy;
			}else{
				cmd_vel = sub_cmd_vel;
			}
		}
		cmd_pub.publish(cmd_vel);
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
