from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument, LogInfo, IncludeLaunchDescription
from launch.substitutions import LaunchConfiguration, FindExecutable
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python.packages import get_package_share_directory
import os
import yaml


def generate_launch_description():
    pkg_share = get_package_share_directory('anro_lab3')
    
    params_file = os.path.join(pkg_share, 'config', 'robot_params.yaml')
    
    with open(params_file, 'r') as file:
        robot_params = yaml.safe_load(file)

    info = LogInfo(msg=f"Starting forward kinematics node with parameters from {params_file}")
    
    forward_kin_node = Node(
        package='anro_lab3',
        executable='forward_kin',
        name='forward_kinematics',
        output='screen',
        parameters=[robot_params]
    )

    # Use a different approach for the rvizToDobot node
    rvizToDobot = Node(
        package='anro_lab3',
        executable='rvizToDobot',
        name='RvizToDobotConvert',
        output='screen',
    )
    
    display_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource([
            os.path.join(pkg_share, 'launch', 'display.launch.py')
        ])
    )
    
    return LaunchDescription([
        info,
        forward_kin_node,
        rvizToDobot,
        display_launch
    ])