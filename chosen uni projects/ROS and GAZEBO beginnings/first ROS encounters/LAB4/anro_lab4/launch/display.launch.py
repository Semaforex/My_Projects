from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import xacro
import yaml
import os

def generate_launch_description():
    pkg_share = get_package_share_directory('anro_lab4')  
    xacro_file = os.path.join(pkg_share, 'urdf', 'dobot.urdf.xacro')
    param_file = os.path.join(pkg_share, 'config', 'robot_params.yaml')

    with open(param_file, 'r') as f:
        params = yaml.safe_load(f)
    print("Loaded Parameters:")
    print(params)
    robot_description_config = xacro.process_file(xacro_file, mappings=params)
    robot_description = {'robot_description': robot_description_config.toxml()}
    
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[robot_description]
    )

    robot_control = Node(
        package='anro_lab4',  
        executable='robot_control',  
        name='robot_control',
        remappings=[]
    )

    # joint_state_publisher = Node(
    #     package = 'anro_lab4',  
    #     executable = 'robot_control',
    #     name = 'joint_state_publisher',
    #     output = 'screen',
    #     parameters = [robot_description, params]  
    # )

    rvizToDobot = Node(
        package='anro_lab4',
        executable='rvizToDobot',
        name='RvizToDobotConvert',
        output='screen',
    )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', os.path.join(pkg_share, 'config', 'display.rviz')],
        parameters=[robot_description]
    )

    return LaunchDescription([
        # joint_state_publisher,
        robot_state_publisher,
        rviz_node,
        rvizToDobot,
        robot_control
    ])
