import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, Command
from launch_ros.actions import Node

from launch.conditions import IfCondition, UnlessCondition
from launch_ros.parameter_descriptions import ParameterValue

def generate_launch_description():

    urdf_file_name = 'dobot_visualization.urdf'
    rviz_name = "urdf.rviz"
    params_name = "dobot_params.yaml"
    urdf = os.path.join(
        get_package_share_directory('pd_lab6'),
        urdf_file_name)
    rviz = os.path.join(
        get_package_share_directory('pd_lab6'),
        rviz_name)
    
    params_path = os.path.join(
        get_package_share_directory('pd_lab6'),
        params_name)
    
    gui_arg = DeclareLaunchArgument(name='gui', default_value='true', choices=['true', 'false'],
                                    description='Flag to enable joint_state_publisher_gui')
    model_arg = DeclareLaunchArgument(name='model', default_value=str(urdf),
                                      description='Absolute path to robot urdf file')
    rviz_arg = DeclareLaunchArgument(name='rvizconfig', default_value=str(rviz),
                                     description='Absolute path to rviz config file')
    param_arg = DeclareLaunchArgument(name='model', default_value=str(params_path),
                                      description='Absolute path to robot urdf file')

    robot_description = ParameterValue(Command(['xacro ', LaunchConfiguration('model'), " params_path:=", params_path]),
                                       value_type=str)

    robot_state_publisher_node = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )

    # joint_state_publisher_node = Node(
    #     package='joint_state_publisher',
    #     executable='joint_state_publisher',
    #     condition=UnlessCondition(LaunchConfiguration('gui'))
    # )

    # joint_state_publisher_gui_node = Node(
    #     package='joint_state_publisher_gui',
    #     executable='joint_state_publisher_gui',
    #     condition=IfCondition(LaunchConfiguration('gui'))
    # )

    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        name='rviz2',
        output='screen',
        arguments=['-d', LaunchConfiguration('rvizconfig')],
    )
    
    
    cube_move_node = Node(
        package="pd_lab6",
        executable="cube_move"
    )
    
    reverse_kin_node = Node(
        package="pd_lab6",
        executable="reverse_kin"
    )
    
    forward_kin_node = Node(
        package="pd_lab6",
        executable="forward_kin"
    )

    return LaunchDescription([
        gui_arg,
        model_arg,
        rviz_arg,
        # joint_state_publisher_node,
        # joint_state_publisher_gui_node,
        robot_state_publisher_node,
        rviz_node,
        param_arg,
        cube_move_node,
        reverse_kin_node,
        forward_kin_node
        # marker_publisher_node
    ])