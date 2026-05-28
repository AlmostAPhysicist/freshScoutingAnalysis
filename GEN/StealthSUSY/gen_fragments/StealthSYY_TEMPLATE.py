import FWCore.ParameterSet.Config as cms
from Configuration.Generator.Pythia8CommonSettings_cfi import *
from Configuration.Generator.MCTunes2017.PythiaCP5Settings_cfi import *
from Configuration.Generator.PSweightsPythia.PythiaPSweightsSettings_cfi import *

mstop = ChangeMstop
mso = ChangeMso
ctau = ChangeCtau

baseSLHATable="""
BLOCK QNUMBERS 5000001 # singlino singlinobar
      1     0  # 3 times electric charge
      2     2  # number of spin states (2S+1)
      3     1  # colour rep (1: singlet, 3: triplet, 6: sextet, 8: octet)
      4     1  # Particle/Antiparticle distinction (0=own anti)
BLOCK QNUMBERS 5000002 # singlet singletbar
      1     0  # 3 times electric charge
      2     1  # number of spin states (2S+1)
      3     1  # colour rep (1: singlet, 3: triplet, 6: sextet, 8: octet)
      4     1  # Particle/Antiparticle distinction (0=own anti)
BLOCK MASS  # Mass Spectrum
# PDG code           mass       particle
   1000001     1.00000000E+05   # ~d_L
   2000001     1.00000000E+05   # ~d_R
   1000002     1.00000000E+05   # ~u_L
   2000002     1.00000000E+05   # ~u_R
   1000003     1.00000000E+05   # ~s_L
   2000003     1.00000000E+05   # ~s_R
   1000004     1.00000000E+05   # ~c_L
   2000004     1.00000000E+05   # ~c_R
   1000005     1.00000000E+05   # ~b_1
   2000005     1.00000000E+05   # ~b_2
   1000006     %MSTOP%          # ~t_1
   2000006     1.00000000E+05   # ~t_2
   1000011     1.00000000E+05   # ~e_L
   2000011     1.00000000E+05   # ~e_R
   1000012     1.00000000E+05   # ~nu_eL
   1000013     1.00000000E+05   # ~mu_L
   2000013     1.00000000E+05   # ~mu_R
   1000014     1.00000000E+05   # ~nu_muL
   1000015     1.00000000E+05   # ~tau_1
   2000015     1.00000000E+05   # ~tau_2
   1000016     1.00000000E+05   # ~nu_tauL
   1000021     1.00000000E+05   # ~g
   1000022     1.00000000E+05   # ~chi_10
   1000023     1.00000000E+05   # ~chi_20
   1000025     1.00000000E+05   # ~chi_30
   1000035     1.00000000E+05   # ~chi_40
   1000024     1.00000000E+05   # ~chi_1+
   1000037     1.00000000E+05   # ~chi_2+
   5000001     %MSO%  		# m(singlino)
   5000002     90.0 		# m(singlet)
   1000039     1.0  		# m(Gravitino)
# DECAY TABLE
#         PDG            Width
DECAY   1000001     0.00000000E+00   # sdown_L decays
DECAY   2000001     0.00000000E+00   # sdown_R decays
DECAY   1000002     0.00000000E+00   # sup_L decays
DECAY   2000002     0.00000000E+00   # sup_R decays
DECAY   1000003     0.00000000E+00   # sstrange_L decays
DECAY   2000003     0.00000000E+00   # sstrange_R decays
DECAY   1000004     0.00000000E+00   # scharm_L decays
DECAY   2000004     0.00000000E+00   # scharm_R decays
DECAY   1000005     0.00000000E+00   # sbottom1 decays
DECAY   2000005     0.00000000E+00   # sbottom2 decays
DECAY   1000006     1.00000000E+00   # stop1 decays
    1.00000000E-00    3           6    5000001      21       # BR(stop -> top singlino g)
DECAY   2000006     0.00000000E+00   # stop2 decays
DECAY   1000011     0.00000000E+00   # selectron_L decays
DECAY   2000011     0.00000000E+00   # selectron_R decays
DECAY   1000012     0.00000000E+00   # snu_elL decays
DECAY   1000013     0.00000000E+00   # smuon_L decays
DECAY   2000013     0.00000000E+00   # smuon_R decays
DECAY   1000014     0.00000000E+00   # snu_muL decays
DECAY   1000015     0.00000000E+00   # stau_1 decays
DECAY   2000015     0.00000000E+00   # stau_2 decays
DECAY   1000016     0.00000000E+00   # snu_tauL decays
DECAY   1000021     0.00000000E+00   # gluino decays
DECAY   1000022     0.00000000E+00   # neutralino1 decays
DECAY   1000023     0.00000000E+00   # neutralino2 decays
DECAY   1000024     0.00000000E+00   # chargino1+ decays
DECAY   1000025     0.00000000E+00   # neutralino3 decays
DECAY   1000035     0.00000000E+00   # neutralino4 decays
DECAY   1000037     0.00000000E+00   # chargino2+ decays
DECAY   5000001     %WIDTHSO%        # singlino decays
     1.00000000E-00    2     5000002   1000039        # BR(singlino -> singlet gravitino)
DECAY   5000002     1.00000000E-03   # singlet decays
     1.00000000E-00    2          21        21        # BR(singlet -> g g)
"""

chbar = 1.97326980E-13 # in GeV*mm
widthso = chbar / ctau
ctauLabel = str(ctau).replace('.', 'p')

slhatable = (baseSLHATable
             .replace('%MSTOP%', '%e' % mstop)
             .replace('%MSO%', '%e' % mso)
             .replace('%WIDTHSO%', '%e' % widthso))

def matchParams(mass):
  if mass>99 and mass<199: return 62.
  elif mass<299:           return 62.
  elif mass<399:           return 62.
  elif mass<499:           return 64.
  elif mass<599:           return 64. 
  elif mass<1299:          return 68.
  elif mass<1451:          return 70.
  elif mass<1801:          return 74.
  elif mass<2000:          return 76.
  elif mass<2051:          return 78.

qcut = matchParams(mstop)

externalLHEProducer = cms.EDProducer("ExternalLHEProducer",
    args = cms.vstring('root://eosuser.cern.ch//eos/user/b/brlopesd/StealthSUSY_gridpacks/SMS-StopStop_mStop-%i_el8_amd64_gcc10_CMSSW_12_4_8_tarball.tar.xz' % int(mstop)),
    nEvents = cms.untracked.uint32(10),
    numberOfParameters = cms.uint32(1),
    outputFile = cms.string('cmsgrid_final.lhe'),
    scriptName = cms.FileInPath('GeneratorInterface/LHEInterface/data/run_generic_tarball_xrootd.sh')
)

basePythiaParameters = cms.PSet(
    pythia8CommonSettingsBlock,
    pythia8CP5SettingsBlock,
    pythia8PSweightsSettingsBlock,
    JetMatchingParameters = cms.vstring(
        'JetMatching:setMad = off',
        'JetMatching:scheme = 1',
        'JetMatching:merge = on',
        'JetMatching:jetAlgorithm = 2',
        'JetMatching:etaJetMax = 5.',
        'JetMatching:coneRadius = 1.',
        'JetMatching:slowJetPower = 1',
        'JetMatching:qCut = %.0f' % qcut, #this is the actual merging scale
        'JetMatching:nQmatch = 5', #4 corresponds to 4-flavour scheme (no matching of b-quarks), 5 for 5-flavour scheme
        'JetMatching:nJetMax = 2', #number of partons in born matrix element for highest multiplicity
        'JetMatching:doShowerKt = off', #off for MLM matching, turn on for shower-kT matching
        '6:m0 = 172.5',
        'Check:abortIfVeto = on',
    ), 
    parameterSets = cms.vstring('pythia8CommonSettings',
                                'pythia8CP5Settings',
                                'JetMatchingParameters',
                                'pythia8PSweightsSettings',
                                )
)

generator = cms.EDFilter("Pythia8HadronizerFilter",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(-1),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    comEnergy = cms.double(13600.),
    maxEventsToPrint = cms.untracked.int32(1),
    SLHATableForPythia8 = cms.string(slhatable),
    PythiaParameters = basePythiaParameters,
    ConfigWeight = cms.double(1),
    ConfigDescription = cms.string(f"StealthSHH_2t4b_mStop-{mstop}_mSo-{mso}_ctau-{ctauLabel}")
)

process = cms.Process("GEN")
process.externalLHEProducer = externalLHEProducer
process.generator = generator
process.ProductionFilterSequence = cms.Sequence(
    process.externalLHEProducer * process.generator
)