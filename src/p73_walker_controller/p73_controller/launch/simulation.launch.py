#!/usr/bin/env python3

from launch import LaunchDescription
from launch.actions import (
    DeclareLaunchArgument,
    OpaqueFunction,
    RegisterEventHandler,
    TimerAction,
)
from launch.event_handlers import OnProcessStart
from launch.substitutions import LaunchConfiguration
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory

import os


# Mapping from the `model` launch argument → MuJoCo XML filename.
# Add new variants here only; do not hard-code the path elsewhere.
MODEL_VARIANTS = {
    'default': 'p73_walker.xml',          # class-default motor params (pre-PACE)
    'pace':    'p73_walker_pace.xml',     # PACE-identified per-joint params
    'fixed':   'p73_walker_fixed.xml',    # fixed-base variant
}


def launch_setup(context, *args, **kwargs):
    # Resolve launch arg to an actual filename
    model = LaunchConfiguration('model').perform(context)
    if model not in MODEL_VARIANTS:
        raise ValueError(
            f"[simulation.launch.py] Unknown 'model' argument: '{model}'. "
            f"Expected one of {list(MODEL_VARIANTS.keys())}. "
            f"Example: ros2 launch p73_controller simulation.launch.py model:=pace"
        )
    xml_filename = MODEL_VARIANTS[model]

    # Resolve package share paths
    p73_description_share = get_package_share_directory('p73_walker_description')
    model_file = os.path.join(p73_description_share, 'mujoco', xml_filename)
    urdf_path  = os.path.join(p73_description_share, 'urdf', 'p73_walker.urdf')
    setting_sim_PDgain_path = os.path.join(
        p73_description_share, 'setting', 'setting_sim_PDgain.yaml'
    )

    # Sanity checks
    if not os.path.exists(model_file):
        raise FileNotFoundError(f"Model file not found: {model_file}")
    if not os.path.exists(urdf_path):
        raise FileNotFoundError(f"URDF file not found: {urdf_path}")

    print(f"[simulation.launch.py] model='{model}' → {model_file}")

    # MuJoCo ROS2 node
    mujoco_node = Node(
        package='mjc_ros2',
        executable='mujoco_ros2',
        name='mujoco_ros2',
        output='screen',
        parameters=[{
            'model_file': model_file,
            'joint_names': [
                'L_HipRoll_Joint', 'L_HipPitch_Joint', 'L_HipYaw_Joint',
                'L_Knee_Joint',    'L_AnklePitch_Joint', 'L_AnkleRoll_Joint',
                'R_HipRoll_Joint', 'R_HipPitch_Joint', 'R_HipYaw_Joint',
                'R_Knee_Joint',    'R_AnklePitch_Joint', 'R_AnkleRoll_Joint',
                'WaistYaw_Joint',
            ],
        }]
    )

    # P73 Controller node
    p73_controller_node = Node(
        package='p73_controller',
        executable='p73_controller',
        name='p73_controller',
        output='screen',
        parameters=[
            {
                'urdf_path': urdf_path,
                'xml_path':  model_file,
                'sim_mode':  True,
            },
            setting_sim_PDgain_path,
        ]
    )

    # P73 GUI node (RQt plugin)
    # RQt saves/restores window position and size automatically
    p73_gui_node = Node(
        package='rqt_gui',
        executable='rqt_gui',
        name='p73_gui',
        output='screen',
        arguments=['--force-discover', '--standalone', 'p73_gui/P73Gui']
    )

    delayed_p73_controller_node = RegisterEventHandler(
        OnProcessStart(
            target_action=mujoco_node,
            on_start=[TimerAction(period=0.2, actions=[p73_controller_node])]
        )
    )

    return [
        mujoco_node,
        delayed_p73_controller_node,
        p73_gui_node,
    ]


def generate_launch_description():
    return LaunchDescription([
        DeclareLaunchArgument(
            'model',
            default_value='default',
            description=(
                "MuJoCo model variant. Options: "
                "'default' (pre-PACE class-default motor params), "
                "'pace' (PACE-identified per-joint damping/frictionloss), "
                "'fixed' (fixed-base variant). "
                "Usage: ros2 launch p73_controller simulation.launch.py model:=pace"
            ),
        ),
        OpaqueFunction(function=launch_setup),
    ])
