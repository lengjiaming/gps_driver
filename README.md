# gps_driver适用于华测导航610款组合导航模式
# Clone repository:
    cd ~/catkin_ws/src
    git clone https://github.com/lengjiaming/gps_driver
    cd ..
    catkin_make
    source ~/catkin_ws/devel/setup.bash
    
# Launch ROS
    roslaunch chccgi610 chccgi610_nav.launch
    
# If you want to see the trajectory at the same time, you should run:
    roslaunch chccgi610 realtime_trajectory.launch
    
# Using a rosbag to offline test, you should run 
    chccgi610_trajectory.launch
  and change the path at rosbag node in launch
