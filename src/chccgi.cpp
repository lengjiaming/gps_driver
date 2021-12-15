#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ros/ros.h>
#include <ros/console.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
 
#include <nav_msgs/Odometry.h>
 #include<chccgi610/chccgi610nav.h>
 
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include"../include/conversion.h"
 
 
nav_msgs::Path  path;
 
ros::Publisher  path_pub;
ros::Subscriber odomSub;
ros::Subscriber odom_raw_Sub;
//欧拉角转化为四元数
#define PI  3.14159265
 double getQua_x(double  roll_deg, double yaw_deg, double pitch_deg)
{
	return sin(roll_deg*PI/180.0/2)*cos(yaw_deg*PI/180.0/2)*cos(pitch_deg*PI/180.0/2)-cos(roll_deg*PI/180.0/2)*sin(yaw_deg*PI/180.0/2)*sin(pitch_deg*PI/180.0/2);
}
 
double getQua_y(double  roll_deg, double yaw_deg, double pitch_deg)
{
	return cos(roll_deg*PI/180.0/2)*sin(yaw_deg*PI/180.0/2)*cos(pitch_deg*PI/180.0/2)+sin(roll_deg*PI/180.0/2)*cos(yaw_deg*PI/180.0/2)*sin(pitch_deg*PI/180.0/2);
}
double getQua_z(double  roll_deg, double yaw_deg, double pitch_deg)
{
	return cos(roll_deg*PI/180.0/2)*cos(yaw_deg*PI/180.0/2)*sin(pitch_deg*PI/180.0/2)-sin(roll_deg*PI/180.0/2)*sin(yaw_deg*PI/180.0/2)*cos(pitch_deg*PI/180.0/2);
}
double getQua_w(double  roll_deg, double yaw_deg, double pitch_deg)
{
	return cos(roll_deg*PI/180.0/2)*cos(yaw_deg*PI/180.0/2)*cos(pitch_deg*PI/180.0/2)+sin(roll_deg*PI/180.0/2)*sin(yaw_deg*PI/180.0/2)*sin(pitch_deg*PI/180.0/2);
}
int times =0;
double intial_x=0;
double intial_y=0;
 void odomCallback(const chccgi610::chccgi610nav::ConstPtr& odom)
 {
    geometry_msgs::PoseStamped this_pose_stamped;
    double lat = odom->lat;
    double lon = odom->lon;
    //std::cout<<"lat= "<<lat<<"        lon="<<lon<<std::endl;
    double  x= 0;
    double  y= 0;
    gps_common::UTM(lat, lon, &x, &y);
    //std::cout<<"x= "<<x<<"    y="<<y<<std::endl;
    if(times==0)
    {
        intial_x = x;
        intial_y = y;
        times=1;
    }
    else
    {
         x -=  intial_x ;
         y -=  intial_y;
         
    }
    //std::cout<<"x= "<<x<<"    y="<<y<<std::endl;
    //this_pose_stamped.header.stamp = odom->gpstime;
    this_pose_stamped.pose.position.x = x;
    this_pose_stamped.pose.position.y = y;
    this_pose_stamped.pose.position.z = 0;
  if(x<1000&&x>-1000){
    this_pose_stamped.pose.orientation.x = getQua_x(odom->roll, odom->heading, odom->pitch);
    this_pose_stamped.pose.orientation.y = getQua_y(odom->roll, odom->heading, odom->pitch);
    this_pose_stamped.pose.orientation.z = getQua_z(odom->roll, odom->heading, odom->pitch);
    this_pose_stamped.pose.orientation.w = getQua_w(odom->roll, odom->heading, odom->pitch);
 
    this_pose_stamped.header.stamp = ros::Time::now();
    this_pose_stamped.header.frame_id = "camera_init";
 
    path.poses.push_back(this_pose_stamped);
 
    path.header.stamp = ros::Time::now();
    path.header.frame_id="camera_init";
   
    path_pub.publish(path);
    printf("path_time= %f\n",odom->gpstime);
    printf("path_pub ");
    
    printf("odom %.3lf %.3lf\n", this_pose_stamped.pose.position.x ,this_pose_stamped.pose.position.y);
    }
 }
 
 
int main (int argc, char **argv)
{
    ros::init (argc, argv, "showpath");
 
    ros::NodeHandle ph;
 
    path_pub = ph.advertise<nav_msgs::Path>("trajectory",10, true);
    odomSub  = ph.subscribe<chccgi610::chccgi610nav>("/chccgi610_nav", 10, odomCallback);
 
 
    ros::Rate loop_rate(100);
 
    while (ros::ok())
    {
        ros::spinOnce();               // check for incoming messages
        loop_rate.sleep();
    }
 
    return 0;
}
