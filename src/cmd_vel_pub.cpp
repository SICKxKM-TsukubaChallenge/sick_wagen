#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

geometry_msgs::Twist cmd_vel;
geometry_msgs::Twist cmd_vel_move_base;
geometry_msgs::Twist cmd_vel_whill;
sensor_msgs::Joy joy;


void joy_callback(const geometry_msgs::Twist& cmd_vel_msg)
{
	cmd_vel_move_base = cmd_vel_msg;
//	cmd_vel_joy.linear.x = joy_msg.axes[3]/1.5; //3
//	cmd_vel_joy.angular.z = joy_msg.axes[2]/1; //1.5
//
//	if(joy_msg.buttons[5] == 1){
//	    cmd_vel_joy.linear.x = joy_msg.axes[3]/0.75; //1.5
//	    cmd_vel_joy.angular.z = joy_msg.axes[2]/0.75; //1
//  }
//    if(joy_msg.buttons[4] == 1){
//	    cmd_vel_joy.linear.x = joy_msg.axes[3]/8;
//	    cmd_vel_joy.angular.z = joy_msg.axes[2]/4;
//    }
}

void joy_callback2(const sensor_msgs::Joy& joy_msg)
{
	cmd_vel_whill.linear.x = joy_msg.axes[1]/2;
	cmd_vel_whill.angular.z = joy_msg.axes[0]/1.5;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "gamepad_pub");
	ros::NodeHandle nh;
	ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	ros::Subscriber gamepad_joy_sub = nh.subscribe("/cmd_vel_move_base", 10, joy_callback);
	ros::Subscriber whill_joy_sub = nh.subscribe("/whill/states/joy", 10, joy_callback2);

	ros::Rate loop_rate(10);
	while (ros::ok())
	{
		if(cmd_vel_whill.linear.x == 0 && cmd_vel_whill.angular.z == 0)
		{
			cmd_vel = cmd_vel_move_base;
		}else{
			cmd_vel = cmd_vel_whill;
		}
		cmd_pub.publish(cmd_vel);
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
