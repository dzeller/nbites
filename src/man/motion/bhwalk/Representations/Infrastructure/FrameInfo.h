/**
* @file FrameInfo.h
* The file declares a class that contains information on the current frame.
* @author <a href="mailto:Thomas.Roefer@dfki.de">Thomas Röfer</a>
*/

#pragma once

#include "Tools/Streams/AutoStreamable.h"

/**
* @class FrameInfoBH
* A class that contains information on the current frame.
*/
class FrameInfoBH
{
public:
    FrameInfoBH() {time = 0; cycleTime = 1;}
  /**
  * The method returns the time difference between a given time stamp and the
  * current frame time.
  * @param timeStamp A time stamp, usually in the past.
  * @return The number of ms passed since the given time stamp.
  */
  int getTimeSince(unsigned timeStamp) const {return int(time - timeStamp);}

  unsigned time; /**< The time stamp of the data processed in the current frame in ms. */
  float cycleTime; /**< Length of one cycle in seconds. */
};
