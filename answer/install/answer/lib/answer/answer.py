from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='homework',
            executable='homework_node',
            name='homework_node',
        ),
        Node(
            package='answer',
            executable='answer',
            name='answer',
        ),
    ])