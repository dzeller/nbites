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

        player.wasPenalized = False

    return player.stay()

@superState('gameControllerResponder')
def gameReady(player):
    if player.firstFrame():
        player.gainsOn()
        player.brain.nav.stand()
        player.runfallController = False
        player.wasPenalized = False

        if len(player.kick) > 1:
            player.kick = player.kick[:-1]

    return player.stay()
@superState('gameControllerResponder')
def gameSet(player):
    if player.firstFrame():
#        player.kick = SweetMoves.INITIAL_POS
        player.kick = SweetMoves.STAND_FOR_KICK_LEFT
        player.wasPenalized = False
    return player.stay()

@superState('gameControllerResponder')
def gamePlaying(player):
    if player.firstFrame():
        player.gainsOn()
        if player.wasPenalized:
            joints = constructTuple(player)
            player.kick = player.kick + (joints,)
        player.stand()
        gamePlaying.hasExecuted = False
        player.wasPenalized = False

    if player.brain.nav.isStopped() and not gamePlaying.hasExecuted:
        player.executeMove(player.kick)
        gamePlaying.hasExecuted = True

    return player.stay()

@superState('gameControllerResponder')
def gamePenalized(player):
    if player.firstFrame():
        player.gainsOff()
        player.wasPenalized = True

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

    fifthLine = (.4, 1, stiff.STANDUP_STIFFNESSES)

    # jointTuple = (firstLine, secondLine, thirdLine, fourthLine) + fifthLine
    jointTuple = (player.kick[-1][0], secondLine, player.kick[-1][2], player.kick[-1][3]) + fifthLine

    for something in jointTuple:
            print something

    return jointTuple
