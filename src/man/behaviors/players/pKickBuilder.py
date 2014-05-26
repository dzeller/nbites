from . import SoccerFSA
from . import FallControllerStates
from . import RoleSwitchingStates
from . import GameControllerStates
from . import KickBuilderStates

from .. import SweetMoves

class SoccerPlayer(SoccerFSA.SoccerFSA):
    def __init__(self, brain):
        SoccerFSA.SoccerFSA.__init__(self, brain)
        self.addStates(FallControllerStates)
        self.addStates(RoleSwitchingStates)
        self.addStates(GameControllerStates)
        self.addStates(KickBuilderStates)
        self.setName('pKickBuilder')
        self.currentState = 'fallController' # initial state

        self.brain.fallController.enabled = False
        self.roleSwitching = False
        self.role = brain.playerNumber

        self.kick = SweetMoves.INITIAL_POS
