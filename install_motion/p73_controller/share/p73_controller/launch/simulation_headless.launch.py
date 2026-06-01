#!/usr/bin/env python3
"""
simulation_headless.launch.py — GUI 없이 자동화용 MuJoCo 시뮬레이션 실행.

변경점 (simulation.launch.py 대비):
  - p73_gui, motion_gui 미실행
  - auto_start=true (Space 없이 즉시 시작)
  - auto_shutdown_duration 설정 가능 (시뮬 시간 초과 시 자동 종료)

사용법:
  xvfb-run ros2 launch p73_controller simulation_headless.launch.py
  xvfb-run ros2 launch p73_controller simulation_headless.launch.py auto_shutdown_duration:=60.0
"""

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument, TimerAction, RegisterEventHandler, OpaqueFunction
from launch.event_handlers import OnProcessStart
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


def _launch_setup(context, *args, **kwargs):
    """OpaqueFunction to resolve LaunchConfiguration to proper types."""
    p73_description_share = get_package_share_directory('p73_walker_description')

    model_file = os.path.join(p73_description_share, 'mujoco', 'p73_walker.xml')
    urdf_path = os.path.join(p73_description_share, 'urdf', 'p73_walker.urdf')
    setting_sim_PDgain_path = os.path.join(p73_description_share, 'setting', 'setting_sim_PDgain.yaml')

    if not os.path.exists(model_file):
        raise FileNotFoundError(f"Model file not found: {model_file}")

    # Resolve to proper float (LaunchConfiguration returns string)
    shutdown_dur = float(LaunchConfiguration('auto_shutdown_duration').perform(context))

    mujoco_node = Node(
        package='mjc_ros2',
        executable='mujoco_ros2_headless',
        name='mujoco_ros2',
        output='screen',
        parameters=[{
            'model_file': model_file,
            'joint_names': [
                'L_HipRoll_Joint', 'L_HipPitch_Joint', 'L_HipYaw_Joint',
                'L_Knee_Joint', 'L_AnklePitch_Joint', 'L_AnkleRoll_Joint',
                'R_HipRoll_Joint', 'R_HipPitch_Joint', 'R_HipYaw_Joint',
                'R_Knee_Joint', 'R_AnklePitch_Joint', 'R_AnkleRoll_Joint',
                'WaistYaw_Joint',
            ],
            'auto_shutdown_duration': shutdown_dur,
        }]
    )

    p73_controller_node = Node(
        package='p73_controller',
        executable='p73_controller',
        name='p73_controller',
        output='screen',
        parameters=[{
            'urdf_path': urdf_path,
            'xml_path': model_file,
            'sim_mode': True,
        }, setting_sim_PDgain_path]
    )

    delayed_p73_controller_node = RegisterEventHandler(
        OnProcessStart(
            target_action=mujoco_node,
            on_start=[TimerAction(period=0.2, actions=[p73_controller_node])]
        )
    )

    return [mujoco_node, delayed_p73_controller_node]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument('auto_shutdown_duration', default_value='0.0',
                              description='Auto-shutdown after N sim seconds (0=disabled)'),
        OpaqueFunction(function=_launch_setup),
    ])
