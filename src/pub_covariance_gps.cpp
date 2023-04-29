#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>

nav_msgs::Odometry vo;

double cov_x =  0.0;
double cov_y =  0.0;
double cov_z =  0.0;

void odometryGpsCallback(const nav_msgs::Odometry& odometryGps){
  vo = odometryGps;
  cov_x = odometryGps.pose.covariance[0];
  cov_y = odometryGps.pose.covariance[7];
  cov_z = odometryGps.pose.covariance[14];
//  vo.header.frame_id = "base_footprint";
  vo.pose.covariance = {  cov_x, 0, 0, 0, 0, 0,
                         0, cov_y, 0, 0, 0, 0,
                         0, 0, cov_z, 0, 0, 0,
                         0, 0, 0, 99999, 0, 0,
                         0, 0, 0, 0, 99999, 0,
                         0, 0, 0, 0, 0, 99999};
}

int main(int argc, char** argv){
  ros::init(argc, argv, "pub_covariance_gps");

  ros::NodeHandle n;
  ros::Publisher  gps_odom_pub = n.advertise<nav_msgs::Odometry>("vo", 10);
  ros::Subscriber gps_odom_sub = n.subscribe("/odometry/gps", 100, odometryGpsCallback);
  
  ros::Rate loop_rate(10);
  while(ros::ok()){
    //publish the message
    gps_odom_pub.publish(vo);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}
