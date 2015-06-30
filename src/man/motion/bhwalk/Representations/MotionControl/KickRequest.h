/**
* @file Representations/MotionControl/kickRequest.h
* @author <a href="mailto:judy@informatik.uni-bremen.de">Judith Müller</a>
*/

#pragma once

#include "Modules/MotionControl/KickEngine/KickEngineParameters.h"
#include "Tools/Streams/AutoStreamable.h"

//STREAMABLE(KickRequest,
class KickRequest {
public:
  ENUM(KickMotionID,
    kickForward,
    newKick,
    none
  );

    KickRequest() {
        kickMotionType = none;
        mirror = false;
        dynamical = false;
    }

    static KickMotionID getKickMotionFromName(const char* name);

    KickMotionID kickMotionType;
    bool mirror;
    bool dynamical;
    std::vector<DynPoint> dynPoints;
};
