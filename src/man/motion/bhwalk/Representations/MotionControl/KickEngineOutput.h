/**
* @file Representations/MotionControl/KickEngineOutput.h
* This file declares a class that represents the output of modules generating motion.
* @author <A href="mailto:judy@tzi.de">Judith Müller</A>
*/

#pragma once

#include "Representations/Infrastructure/JointData.h"
#include "Tools/Math/Pose2D.h"
#include "Representations/MotionControl/KickRequest.h"

/**
* @class KickEngineOutput
* A class that represents the output of the walking engine.
*/
//STREAMABLE_WITH_BASE(KickEngineOutputBH, JointRequest,
class KickEngineOutputBH : JointRequestBH
{
    KickEngineOutputBH() { isLeavingPossible = true; isStable = true; }
    Pose2DBH odometryOffset; /**< The body motion performed in this step. */
    bool isLeavingPossible; /**< Is leaving the motion module possible now? */
    bool isStable; /**< Is motion currently stable? */
    KickRequest executedKickRequest; /**< The kick request that is currently in execution. */
};
