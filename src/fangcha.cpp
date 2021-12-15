#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ros/ros.h>
#include <ros/console.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
 #include<iostream>
#include <nav_msgs/Odometry.h>
 #include<chccgi610/chccgi610nav.h>
 
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>
#include <tf/tf.h>
#include"../include/conversion.h"
 

ros::Subscriber odomSub1;
//欧拉角转化为四元数
#define PI  3.14159265

int times =0;
double intial_x=0;
double intial_y=0;
double  fangchax=0;
int num=0;
double avgx=0;
double  fangchay=0;
double avgy=0;
void compute(double x,double y)
{
    fangchax=(num-1.0)/(num*num)*(x-avgx)*(x-avgx)+(num-1)/num*fangchax;
    std::cout<<"num= "<<num<<std::endl;
    std::cout<<"x的标准差为"<<sqrt(fangchax)<<"      ";
    fangchay=(num-1.0)/(num*num)*(y-avgy)*(y-avgy)+(num-1)/num*fangchay;
    std::cout<<"y的标准差为"<<sqrt(fangchay)<<"      "<<std::endl;
    avgx=avgx+(x-avgx)/num;
    std::cout<<"x的均值为"<<avgx<<"     ";
    avgy=avgy+(y-avgy)/num;
    std::cout<<"y的均值为"<<avgy<<"     "<<std::endl<<std::endl;
}
 void odomCallback(const chccgi610::chccgi610nav::ConstPtr& odom)
 {
    num+=1;
    geometry_msgs::PoseStamped this_pose_stamped;
    double lat = odom->lat;
    double lon = odom->lon;
    double  x= 0;
    double  y= 0;
    gps_common::UTM(lat, lon, &x, &y);
    
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
    if(num>1)
        compute(x,y);
    
  }
 
 
int main (int argc, char **argv)
{
    ros::init (argc, argv, "fangcha");
 
    ros::NodeHandle ph;
 
    
    odomSub1  = ph.subscribe<chccgi610::chccgi610nav>("/chccgi610_nav", 10, odomCallback);
 
 
    ros::Rate loop_rate(100);
 
    while (ros::ok())
    {
        ros::spinOnce();               // check for incoming messages
        loop_rate.sleep();
    }
 
    return 0;
}
