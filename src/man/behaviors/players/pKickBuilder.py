from . import SoccerFSA
from . import FallControllerStates
from . import RoleSwitchingStates
from . import GameControllerStates
from . import KickBuilderStates

from .. import SweetMoves
from .. import StiffnessModes as stiff

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

        self.kick = SweetMoves.STAND_FOR_KICK_LEFT
        self.kickSetup = kickInitial

kickInitial = (
     ((20.,30.,0.,0.),
     (0.,17.,-15,43.5,-30,-20.),
     (0.,10.,-27,45,-22.5,-17),
     (80.,-30.,0.,0),
     .8,0, stiff.NORMAL_STIFFNESSES),

    # Lift/cock leg
    ((20.,30.,0.,0.),
     (0., 17, -40., 100.,-50.,-25.),
     (0., 10,-27,45.,-22.5,-18),
     (100.,-30,0.,0),
     .4,0, stiff.NORMAL_STIFFNESSES))
