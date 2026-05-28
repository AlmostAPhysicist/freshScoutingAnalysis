M = ChangeMass
CTAU = ChangeCtau

WIDTH = 0.0197e-11 / CTAU

SLHA_TABLE = '''
BLOCK SPINFO      # Spectrum calculator information
    1    Minimal  # spectrum calculator
    2    1.0.0    # version number

BLOCK MODSEL      # Model selection
    1    1        #

BLOCK MASS        # Mass Spectrum
#   PDG code   mass  particle
    1000006    %E    # ~t_1

DECAY    1000006    %E               # ~t_1 decays (~t_1bar is automatically handled)
#   BR          NDA  ID1   ID2
    1.00E+00    2    -1    -1        # ~t_1 -> dbar dbar
''' % (M, WIDTH)


import FWCore.ParameterSet.Config as cms

from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.MCTunesRun3ECM13p6TeV.PythiaCP5Settings_cfi import *


generator = cms.EDFilter("Pythia8ConcurrentGeneratorFilter",
                         comEnergy = cms.double(13600.0),
                         maxEventsToPrint = cms.untracked.int32(1),
                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                         pythiaPylistVerbosity = cms.untracked.int32(1),
                         SLHATableForPythia8 = cms.string(SLHA_TABLE),
                         PythiaParameters = cms.PSet(
        pythia8CommonSettingsBlock,
        pythia8CP5SettingsBlock,
        processParameters = cms.vstring(
            'SUSY:all = off',
            'SUSY:gg2squarkantisquark = on',
            'SUSY:qqbar2squarkantisquark = on',
            'SUSY:idA = 1000006',
            'SUSY:idB = 1000006',
            'RHadrons:allow = on',
            '1000006:tau0 = %f' % CTAU,
            ),
        parameterSets = cms.vstring(
            'pythia8CommonSettings',
            'pythia8CP5Settings',
            'processParameters',
            ),
        ),
    )

ProductionFilterSequence = cms.Sequence(generator)