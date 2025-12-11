import launch
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, LogInfo, SetEnvironmentVariable
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node


def generate_launch_description():
    # Deklarowanie argumentów dla nazw żółwi
    return LaunchDescription([
        DeclareLaunchArgument('turtle2_name', default_value='turtle2', description='Name of the second turtle'),
        DeclareLaunchArgument('turtle3_name', default_value='turtle3', description='Name of the third turtle'),
        Node(
            package='turtlesim',
            executable='turtlesim_node',
            name='turtlesim_node',
            output='screen',
            parameters=[],
        ),

        Node(
            package='lab2_pd_pkg',  
            executable='turtle_control_node',  
            name='turtle_control_node',
            output='screen',
            parameters=[{
                'turtle_names.turtle2_name': LaunchConfiguration('turtle2_name'),
                'turtle_names.turtle3_name': LaunchConfiguration('turtle3_name'),
            }],
            remappings=[],
        ),
    ])
