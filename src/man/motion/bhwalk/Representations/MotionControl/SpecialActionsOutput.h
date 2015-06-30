/**
* @file Representations/MotionControl/SpecialActionsOutput.h
* This file declares a class that represents the output of the special actions module.
* @author <A href="mailto:Thomas.Roefer@dfki.de">Thomas Röfer</A>
*/

#pragma once

#include "Representations/Infrastructure/JointData.h"
#include "Representations/MotionControl/SpecialActionRequest.h"
#include "Tools/Math/Pose2D.h"

/**
* @class SpecialActionsOutputBH
* A class that represents the output of the special actions module.
*/
//STREAMABLE_WITH_BASE(SpecialActionsOutputBH, JointRequestBH,
class SpecialActionsOutputBH : JointRequestBH
{
    SpecialActionsOutputBH() { isLeavingPossible = true; isMotionStable = false; }
    Pose2DBH odometryOffset; /**< The body motion performed in this step. */
    bool isLeavingPossible; /**< Is leaving the motion module possible now? */
    bool isMotionStable; /**< Is the position of the camera directly related to the kinematic chain of joint angles? */
    SpecialActionRequest executedSpecialAction; /**< The special action currently executed. */
};
