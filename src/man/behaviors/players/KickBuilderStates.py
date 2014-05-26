from ..headTracker import HeadMoves
from .. import SweetMoves
from ..util import *
from .. import StiffnessModes as stiff

import math

@superState('gameControllerResponder')
def gameInitial(player):
    if player.firstFrame():
        player.gainsOn()
        player.brain.nav.stand()
        player.runfallController = False

        if len(player.kick) > 1:
            player.kick = player.kick[:-1]

    return player.stay()

@superState('gameControllerResponder')
def gameReady(player):
    if player.firstFrame():
        pass
#        player.executeMove(player.kick)

    return player.stay()

@superState('gameControllerResponder')
def gameSet(player):
    if player.firstFrame():
        pass
#player.gainsOff()
    return player.stay()

@superState('gameControllerResponder')
def gamePlaying(player):
    if player.firstFrame():
        player.gainsOn()
        joints = constructTuple(player)
        player.kick = player.kick + (joints,)
        player.stand()
        gamePlaying.hasExecuted = False

    if player.brain.nav.isStopped() and not gamePlaying.hasExecuted:
        player.executeMove(player.kick)
        gamePlaying.hasExecuted = True

    return player.stay()

@superState('gameControllerResponder')
def gamePenalized(player):
    if player.firstFrame():
        player.gainsOff()

    return player.stay()

def constructTuple(player):
    joints = player.brain.interface.joints

    deg = 180 / math.pi

    firstLine = (deg*joints.l_shoulder_pitch, deg*joints.l_shoulder_roll, deg*joints.l_elbow_yaw, deg*joints.l_elbow_roll)
    secondLine = (deg*joints.l_hip_yaw_pitch, deg*joints.l_hip_roll, deg*joints.l_hip_pitch, deg*joints.l_knee_pitch, \
                  deg*joints.l_ankle_pitch, deg*joints.l_ankle_roll)
    thirdLine = (deg*joints.r_hip_yaw_pitch, deg*joints.r_hip_roll, deg*joints.r_hip_pitch, deg*joints.r_knee_pitch, \
                  deg*joints.r_ankle_pitch, deg*joints.r_ankle_roll)
    fourthLine = (deg*joints.r_shoulder_pitch, deg*joints.r_shoulder_roll, deg*joints.r_elbow_yaw, deg*joints.r_elbow_roll)

    fifthLine = (.5, 0, stiff.STANDUP_STIFFNESSES)

    jointTuple = (firstLine, secondLine, thirdLine, fourthLine) + fifthLine
    # for position in jointTuple:
    #     for tup in position:
    #         print tup
    #         print "done with that position"

    return jointTuple
