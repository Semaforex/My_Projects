import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo, SetEnvironmentVariable
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('height', default_value='1', description='height of a tower'),
        Node(
            package='lab2_robot_controller',  
            executable='robot_control_node',  
            name='robot_control_node',
            parameters=[{
                'tower_height': LaunchConfiguration('height'),
            }],
            remappings=[],
        ),
    ])
