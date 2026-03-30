#ifndef TOCABI_H
#define TOCABI_H

#include <iostream>
#include "math_type_define.h"

#define _MAXTORQUE 1000 // 1000 is the max number

namespace P73
{
    // =====================================================================
    // Joint order: matches MuJoCo XML body tree / actuator order / IsaacLab
    //   L_HipRoll(0), L_HipPitch(1), L_HipYaw(2), L_Knee(3), L_AnklePitch(4), L_AnkleRoll(5),
    //   R_HipRoll(6), R_HipPitch(7), R_HipYaw(8), R_Knee(9), R_AnklePitch(10), R_AnkleRoll(11),
    //   WaistYaw(12)
    // =====================================================================
    const std::string JOINT_NAME[MODEL_DOF] = {
        "L_HipRoll_Joint", "L_HipPitch_Joint", "L_HipYaw_Joint", "L_Knee_Joint", "L_AnklePitch_Joint", "L_AnkleRoll_Joint",
        "R_HipRoll_Joint", "R_HipPitch_Joint", "R_HipYaw_Joint", "R_Knee_Joint", "R_AnklePitch_Joint", "R_AnkleRoll_Joint",
        "WaistYaw_Joint"};

    static constexpr const char *LINK_NAME[LINK_NUMBER] = {
        "base_link",

        "L_HipRoll_Link", "L_HipPitch_Link", "L_Thigh_Link", "L_Knee_Link", "L_AnkleRoll_Link", "L_Foot_Link",
        "R_HipRoll_Link", "R_HipPitch_Link", "R_Thigh_Link", "R_Knee_Link", "R_AnkleRoll_Link", "R_Foot_Link",

        "WaistYaw_Link"};

    const int Pelvis     = 0;
    const int Left_Foot  = 6;
    const int Right_Foot = 12;
    const int Waist = 13;
    const int COM_id = 14;

    const int JOINT_2_ELMO[MODEL_DOF] = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12};

    const int ELMO_2_JOINT[MODEL_DOF] = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12};

    // Pinocchio (URDF BFS) joint order differs from MuJoCo/code order.
    // PINOCCHIO_IDX_FOR_CODE[code_idx] = pinocchio joint index
    // Used to map q_/q_dot_ (code order) → q_virtual_local_ (Pinocchio order)
    const int PINOCCHIO_IDX_FOR_CODE[MODEL_DOF] = {
        2, 4, 6, 8, 10, 12,    // L leg: Roll→2, Pitch→4, Yaw→6, Knee→8, AnkleP→10, AnkleR→12
        1, 3, 5, 7,  9, 11,    // R leg: Roll→1, Pitch→3, Yaw→5, Knee→7, AnkleP→9,  AnkleR→11
        0                       // WaistYaw→0
    };

} // namespace P73

#endif