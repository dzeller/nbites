/**
* @file CameraCalibration.h
* Declaration of a class for representing the calibration values of the camera.
* @author <a href="mailto:Thomas.Roefer@dfki.de">Thomas Röfer</a>
*/

#pragma once

#include "Tools/Math/Vector3.h"
#include "Tools/Streams/AutoStreamable.h"
#include "Tools/Module/Next.h"

STREAMABLE(CameraCalibrationBH,
{,
  (float)(0) lowerCameraTiltCorrection, /**< The correction of the camera tilt angle in radians. */
  (float)(0) lowerCameraRollCorrection, /**< The correction of the camera roll angle in radians. */
  (float)(0) lowerCameraPanCorrection, /**< The correction of the camera pan angle in radians. */
  (float)(0) upperCameraTiltCorrection, /**< The correction of the camera tilt angle in radians. */
  (float)(0) upperCameraRollCorrection, /**< The correction of the camera roll angle in radians. */
  (float)(0) upperCameraPanCorrection, /**< The correction of the camera pan angle in radians. */
  (float)(0) bodyTiltCorrection, /**< The correction of the body tilt angle in radians. */
  (float)(0) bodyRollCorrection, /**< The correction of the body roll angle in radians. */
  (Vector3BH<>) bodyTranslationCorrection, /**< The correction of the body translation in mm. */
});

using CameraCalibrationNext = Next<CameraCalibrationBH>;
