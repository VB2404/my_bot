import os

from ament_index_python.packages import get_package_share_directory

from launch import LaunchDescription
from launch.substitutions import LaunchConfiguration
from launch.actions import DeclareLaunchArgument

from launch_ros.actions import Node

import xacro


def generate_launch_description():

    # Launch argument
    use_sim_time = LaunchConfiguration('use_sim_time')

    # Get path to package
    pkg_path = get_package_share_directory('my_bot')

    # Path to xacro file
    xacro_file = os.path.join(pkg_path, 'description', 'robot.urdf.xacro')

    # Process xacro
    robot_description_config = xacro.process_file(xacro_file).toxml()

    # Robot description parameter
    params = {
        'robot_description': robot_description_config,
        'use_sim_time': use_sim_time
    }

    # Robot State Publisher
    node_robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        output='screen',
        parameters=[params]
    )

    # Joint State Publisher GUI
    node_joint_state_publisher_gui = Node(
        package='joint_state_publisher_gui',
        executable='joint_state_publisher_gui',
        name='joint_state_publisher_gui'
    )

    # RViz
    node_rviz = Node(
        package='rviz2',
        executable='rviz2',
        output='screen'
    )

    return LaunchDescription([

        DeclareLaunchArgument(
            'use_sim_time',
            default_value='false',
            description='Use simulation time if true'
        ),

        node_robot_state_publisher,
        node_joint_state_publisher_gui,
        node_rviz

    ])