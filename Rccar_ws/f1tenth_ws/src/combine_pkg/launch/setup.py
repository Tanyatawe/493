from launch import LaunchDescription

from launch.actions import IncludeLaunchDescription, ExecuteProcess

from launch.launch description sources import PythonLaunchDescriptionSource

import os

from ament index_python import get_package_share_directory

def generate launch description(): ld = LaunchDescription()

# Task 1: Including vesc driver package 
vesc driver launch = IncludeLaunchDescription(

PythonLaunchDescriptionSource(os.path.join(get_package share directory( 'vesc_driver'),

'launch', 'vesc driver_node.launch.py'))
 )
ld.add action(vesc_driver launch)

# Task 2: Including joy package

joy node = ExecuteProcess(

cmd=['ros2', 'run', 'joy', 'joy_node' ],

output='screen'
)
ld.add action(joy_node)

# Add more IncludeLaunchDescription actions or ExecuteProcess commands for other packages

return ld
