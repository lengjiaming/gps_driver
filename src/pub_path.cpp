#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ros/ros.h>
#include <ros/console.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
 
#include <nav_msgs/Odometry.h>
 
 
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
 
 
nav_msgs::Path  path;
 
ros::Publisher  path_pub;
ros::Subscriber odomSub;
ros::Subscriber odom_raw_Sub;
 
 void odomCallback(const nav_msgs::Odometry::ConstPtr& odom)
 {
    geometry_msgs::PoseStamped this_pose_stamped;
    this_pose_stamped.pose.position.x = odom->pose.pose.position.x;
    this_pose_stamped.pose.position.y = odom->pose.pose.position.y;
 
    this_pose_stamped.pose.orientation = odom->pose.pose.orientation;
 
    this_pose_stamped.header.stamp = ros::Time::now();
    this_pose_stamped.header.frame_id = "odom";
 
    path.poses.push_back(this_pose_stamped);
 
    path.header.stamp = ros::Time::now();
    path.header.frame_id="odom";
    path_pub.publish(path);
    printf("path_pub ");
 
    printf("odom %.3lf %.3lf\n",odom->pose.pose.position.x,odom->pose.pose.position.y);
 }
 
 
int main (int argc, char **argv)
{
    ros::init (argc, argv, "showpath");
 
    ros::NodeHandle ph;
 
    path_pub = ph.advertise<nav_msgs::Path>("trajectory",10, true);
    odomSub  = ph.subscribe<nav_msgs::Odometry>("/navsat/odom", 10, odomCallback);
 
 
    ros::Rate loop_rate(50);
 
    while (ros::ok())
    {
        ros::spinOnce();               // check for incoming messages
        loop_rate.sleep();
    }
 
    return 0;
}
