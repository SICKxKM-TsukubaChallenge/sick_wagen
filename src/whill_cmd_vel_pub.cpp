
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

geometry_msgs::Twist cmd_vel;
geometry_msgs::Twist sub_cmd_vel_nav;
geometry_msgs::Twist cmd_vel_joy;
geometry_msgs::Twist cmd_vel_whill;
sensor_msgs::Joy joy_msg;

void cmd_vel_callback(const geometry_msgs::Twist& cmd_vel_nav)
{
	sub_cmd_vel_nav = cmd_vel_nav;
}

void joy_callback(const sensor_msgs::Joy& joy_msg)
{
	if(cmd_vel_whill.linear.x == 0 && cmd_vel_whill.angular.z == 0){
	cmd_vel_joy.linear.x = joy_msg.axes[3]/15;
	cmd_vel.angular.z = joy_msg.axes[2]/10;	
	}	
}


void whill_joy_callback(const sensor_msgs::Joy& whill_joy_msg)
{
	cmd_vel_whill.linear.x = whill_joy_msg.axes[1]/5;
	cmd_vel_whill.angular.z = whill_joy_msg.axes[0]/5;
}


int main(int argc, char** argv)
{
	ros::init(argc, argv, "gamepad_pub");
	ros::NodeHandle nh;
	ros::Publisher cmd_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 1000);
	ros::Subscriber move_base_cmd_vel_sub = nh.subscribe("/cmd_vel_nav", 10, cmd_vel_callback);
	ros::Subscriber gamepad_joy_sub = nh.subscribe("/joy", 10, joy_callback);
	ros::Subscriber whill_joy_sub = nh.subscribe("/whill/states/joy", 10, whill_joy_callback);

	ros::Rate loop_rate(10);
	while (ros::ok())
	{
		if(cmd_vel_whill.linear.x == 0 && cmd_vel_whill.angular.z == 0){
			//sub_cmd_vel_navが出てcmd_vel_whillがゼロのとき、cmd_velを流す  
			cmd_vel = sub_cmd_vel_nav ;
			ROS_INFO("Navigation now!!");
		}else{
		
			cmd_vel = cmd_vel_whill;
		}
		cmd_pub.publish(cmd_vel);
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}
