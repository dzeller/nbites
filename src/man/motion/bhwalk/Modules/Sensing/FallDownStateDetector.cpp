/**
 * @file FallDownStateDetector.cpp
 *
 * This file implements a module that provides information about the current state of the robot's body.
 *
 * @author <a href="mailto:maring@informatik.uni-bremen.de">Martin Ring</a>
 */

#include "FallDownStateDetector.h"
#include "Representations/Infrastructure/JointData.h"
#include "Tools/Debugging/DebugDrawings.h"

using namespace std;

PROCESS_WIDE_STORAGE(FallDownStateDetector) FallDownStateDetector::theInstance = 0;

FallDownStateDetector::FallDownStateDetector() : lastFallDetected(0)
{
  fallDownAngleX *= pi_180;
  fallDownAngleY *= pi_180;
  onGroundAngle  *= pi_180;
  staggeringAngleX *= pi_180;
  staggeringAngleY *= pi_180;
}

void FallDownStateDetector::update(FallDownStateBH& fallDownState)
{
  DECLARE_PLOT("module:FallDownStateDetector:accelerationAngleXZ");
  DECLARE_PLOT("module:FallDownStateDetector:accelerationAngleYZ");

  // Buffer data:
  buffers[accX].add(theFilteredSensorDataBH.data[SensorDataBH::accX]);
  buffers[accY].add(theFilteredSensorDataBH.data[SensorDataBH::accY]);
  buffers[accZ].add(theFilteredSensorDataBH.data[SensorDataBH::accZ]);

  // Compute average acceleration values and angles:
  float accXaverage(buffers[accX].getAverage());
  float accYaverage(buffers[accY].getAverage());
  float accZaverage(buffers[accZ].getAverage());
  float accelerationAngleXZ(atan2(accZaverage, accXaverage));
  float accelerationAngleYZ(atan2(accZaverage, accYaverage));
  MODIFY("module:FallDownStateDetector:accX",  accXaverage);
  MODIFY("module:FallDownStateDetector:accY",  accYaverage);
  MODIFY("module:FallDownStateDetector:accZ",  accZaverage);
  MODIFY("module:FallDownStateDetector:accAngleXZ", accelerationAngleXZ);
  MODIFY("module:FallDownStateDetector:accAngleYZ", accelerationAngleYZ);
  PLOT("module:FallDownStateDetector:accelerationAngleXZ", accelerationAngleXZ);
  PLOT("module:FallDownStateDetector:accelerationAngleYZ", accelerationAngleYZ);

  fallDownState.odometryRotationOffset = 0;

  if(isCalibrated() && !specialSpecialAction())
  {
    if(theFrameInfoBH.getTimeSince(lastFallDetected) <= fallTime)
    {
      fallDownState.state = FallDownStateBH::falling;
    }
    else if((abs(theFilteredSensorDataBH.data[SensorDataBH::angleX]) <= staggeringAngleX - pi_180
             && abs(theFilteredSensorDataBH.data[SensorDataBH::angleY]) <= staggeringAngleY - pi_180)
            || (fallDownState.state == FallDownStateBH::upright && !isStaggering()))
    {
      fallDownState.state = FallDownStateBH::upright;
      fallDownState.direction = FallDownStateBH::none;
      fallDownState.sidewards = FallDownStateBH::noot;
    }
    else if(fallDownState.state == FallDownStateBH::staggering && isFalling())
    {
      //SystemCall::playSound("doh.wav");
      lastFallDetected = theFrameInfoBH.time;
      fallDownState.state = FallDownStateBH::falling;
      fallDownState.direction = directionOf(theFilteredSensorDataBH.data[SensorDataBH::angleX], theFilteredSensorDataBH.data[SensorDataBH::angleY]);
      if(fallDownState.sidewards != FallDownStateBH::fallen)
      {
        fallDownState.sidewards = sidewardsOf(fallDownState.direction);
      }
    }
    else if((isUprightOrStaggering(fallDownState)
             && isStaggering())
            || (fallDownState.state == FallDownStateBH::staggering
                && abs(theFilteredSensorDataBH.data[SensorDataBH::angleX]) <= staggeringAngleX - pi_180
                && abs(theFilteredSensorDataBH.data[SensorDataBH::angleY]) <= staggeringAngleY - pi_180))
    {
      fallDownState.state = FallDownStateBH::staggering;
      fallDownState.direction = directionOf(theFilteredSensorDataBH.data[SensorDataBH::angleX], theFilteredSensorDataBH.data[SensorDataBH::angleY]);
      if(fallDownState.sidewards != FallDownStateBH::fallen)
      {
        fallDownState.sidewards = sidewardsOf(fallDownState.direction);
      }
    }
    else
    {
      fallDownState.state = FallDownStateBH::undefined;

      if(abs(accelerationAngleXZ) < 0.5f)
      {
        fallDownState.direction = FallDownStateBH::front;
        fallDownState.state = FallDownStateBH::onGround;
        if(theMotionInfoBH.motion != MotionRequestBH::getUp)
        {
          if(fallDownState.sidewards == FallDownStateBH::leftwards)
          {
            fallDownState.odometryRotationOffset = pi_2;
            fallDownState.sidewards = FallDownStateBH::fallen;
          }
          else if(fallDownState.sidewards == FallDownStateBH::rightwards)
          {
            fallDownState.odometryRotationOffset = -pi_2;
            fallDownState.sidewards = FallDownStateBH::fallen;
          }
        }
      }
      else if(abs(accelerationAngleXZ) > 2.5f)
      {

        fallDownState.direction = FallDownStateBH::back;
        fallDownState.state = FallDownStateBH::onGround;
        if(theMotionInfoBH.motion != MotionRequestBH::getUp)
         {
          if(fallDownState.sidewards == FallDownStateBH::leftwards)
          {
            fallDownState.odometryRotationOffset = -pi_2;
            fallDownState.sidewards = FallDownStateBH::fallen;
          }
          else if(fallDownState.sidewards == FallDownStateBH::rightwards)
          {
            fallDownState.odometryRotationOffset = pi_2;
            fallDownState.sidewards = FallDownStateBH::fallen;
          }
        }
      }
      else if(abs(accelerationAngleYZ) < 0.5f)
      {
        fallDownState.direction = FallDownStateBH::left;
        fallDownState.state = FallDownStateBH::onGround;
        if(theMotionInfoBH.motion != MotionRequestBH::getUp)
          if(fallDownState.sidewards != FallDownStateBH::fallen)
            fallDownState.sidewards = FallDownStateBH::leftwards;
      }
      else if(abs(accelerationAngleYZ) > 2.5f)
      {
        fallDownState.direction = FallDownStateBH::right;
        fallDownState.state = FallDownStateBH::onGround;
        if(theMotionInfoBH.motion != MotionRequestBH::getUp)
          if(fallDownState.sidewards != FallDownStateBH::fallen)
            fallDownState.sidewards = FallDownStateBH::rightwards;
      }
    }
  }
  else
    fallDownState.state = FallDownStateBH::undefined;

  //these odometry changes really need to be proven if they are still correct
  if(keeperJumped != None)
  {
    if(theMotionInfoBH.motion == MotionRequestBH::getUp)
    {
      if(fallDownState.direction == FallDownStateBH::front)
      {
        if(keeperJumped == KeeperJumpedLeft)
        {
          fallDownState.odometryRotationOffset = pi_2;
        }
        else
        {
          fallDownState.odometryRotationOffset = -pi_2;
        }
      }
      else
      {
        // standUpBack
        if(keeperJumped == KeeperJumpedLeft)
        {
          fallDownState.odometryRotationOffset = -pi_2;
        }
        else
        {
          fallDownState.odometryRotationOffset = pi_2;
        }
      }
      keeperJumped = None;
    }
  }
}

bool FallDownStateDetector::isUprightOrStaggering(FallDownStateBH& fallDownState)
{
  return fallDownState.state == FallDownStateBH::upright
         || fallDownState.state == FallDownStateBH::staggering;
}

bool FallDownStateDetector::specialSpecialAction()
{
  return (theMotionInfoBH.motion == MotionRequestBH::specialAction
          && theMotionInfoBH.specialActionRequest.specialAction == SpecialActionRequest::playDead);
}

bool FallDownStateDetector::isStaggering()
{
  return abs(theFilteredSensorDataBH.data[SensorDataBH::angleX]) >= staggeringAngleX  pi_180
         || abs(theFilteredSensorDataBH.data[SensorDataBH::angleY]) >= staggeringAngleY  pi_180;
}

bool FallDownStateDetector::isFalling()
{
  return abs(theFilteredSensorDataBH.data[SensorDataBH::angleX]) >= fallDownAngleX
         || abs(theFilteredSensorDataBH.data[SensorDataBH::angleY]) >= fallDownAngleY;
}

bool FallDownStateDetector::isCalibrated()
{
  return theInertiaSensorDataBH.calibrated;
}

FallDownStateBH::Direction FallDownStateDetector::directionOf(float angleX, float angleY)
{
  if(abs(angleX) > abs(angleY) + 0.2f)
  {
    if(angleX < 0.f)
      return FallDownStateBH::left;
    else
      return FallDownStateBH::right;
  }
  else
  {
    if(angleX > 0.f)
      return FallDownStateBH::front;
    else
      return FallDownStateBH::back;
  }
}

FallDownStateBH::Sidestate FallDownStateDetector::sidewardsOf(FallDownStateBH::Direction dir)
{
  switch(dir)
  {
  case FallDownStateBH::left:
    return FallDownStateBH::leftwards;
  case FallDownStateBH::right:
    return FallDownStateBH::rightwards;
  default:
    return FallDownStateBH::noot;
  }
}

MAKE_MODULE(FallDownStateDetector, Sensing)
