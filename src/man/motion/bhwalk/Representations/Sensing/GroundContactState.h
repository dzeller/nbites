/**
* @file GroundContactState.h
* Declaration of class GroundContactStateBH.
* @author Colin Graf
*/

#pragma once

#include "Tools/Streams/AutoStreamable.h"

/**
* @class GroundContactStateBH
* Describes whether we got contact with ground or not.
*/
//STREAMABLE(GroundContactStateBH,
class GroundContactStateBH
{
    GroundContactStateBH() { contact = true; }
    bool contact; /**< a foot of the robot touches the ground */
};
