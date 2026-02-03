import FWCore.ParameterSet.Config as cms
import os
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("ScoutingToRecoJet")
options = VarParsing.VarParsing ('analysis')

options.register('lumi',
                 108.96,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.float,
                 "Integrated luminosity for weighting"
    )
options.register('crossSection',
                 1,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.float,
                 "Cross Section for weighting"
    )
options.register('isMC',
                 False,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "If using MC or data"
    )

options.register('hasJEC',
                 True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "If the dataset has jet corrections. Corrected jets are in the reco::Jet format"
    )

options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.MessageLogger.cerr.FwkSummary.reportEvery = 100
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #MC test file
        #'/store/mc/RunIII2024Summer24MiniAOD/QCD-4Jets_Bin-HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/MINIAODSIM/140X_mcRun3_2024_realistic_v26-v2/100000/00f7403b-49bf-4efd-9b8f-0398bd61d910.root'
        #Data test file
        #'/store/data/Run2024D/ScoutingPFRun3/HLTSCOUT/v1/000/380/945/00000/cdf45723-07c4-4b41-9595-f368f2929369.root'
        #PF monitor file
        #'/store/data/Run2024D/ScoutingPFMonitor/MINIAOD/PromptReco-v1/000/380/306/00000/70ec6086-72c5-4562-82a8-1f043e645d59.root'
        #Higgs test file
        #'/store/mc/RunIII2024Summer24MiniAOD/GluGluH-Hto2Sto4D_Par-ctauS-0p1-MH-125-MS-15_TuneCP5_13p6TeV_powheg-pythia8/MINIAODSIM/140X_mcRun3_2024_realistic_v26-v2/110000/03add799-043d-4b36-ae54-b114138eb7c8.root'
        #Stop test file
        #'/store/user/brlopesd/StopStopbarTo2Dbar2D_M-800_CTau-3mm_Summer24_100k_v2/StopStopbarTo2Dbar2D_M-800_CTau-3mm_Summer24_100k_miniAOD_v2/250214_151828/0000/stop_dbar_miniAOD_1.root'
        #local test file
        'file:testDataFile.root'
    )
)


#Choosing the GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag

if(options.isMC):
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_mcRun3_2024_realistic_v26', '')  
    truePileupTag = cms.InputTag("slimmedAddPileupInfo")
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_HLT_v3', '')
    truePileupTag = cms.InputTag("")

process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )

if(options.hasJEC):
    pfjetsCorrectedTag = cms.InputTag("scoutingPFJetCorrected")
else:
    pfjetsCorrectedTag = cms.InputTag("")

# update JEC
process.GlobalTag.toGet = cms.VPSet(
  cms.PSet( # hlt AK4PFHLT latest
    record = cms.string("JetCorrectionsRecord"),
    tag = cms.string("JetCorrectorParametersCollection_AK4PFHLT_hlt_v1"),
    label = cms.untracked.string("AK4PFHLT"),
    connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
  )
)
#The L1 seeds used for JetHT
L1Info = ["L1_HTT200er", "L1_HTT255er", "L1_HTT280er", "L1_HTT320er", "L1_HTT360er", "L1_HTT400er", "L1_HTT450er", "L1_ETT2000", "L1_SingleJet180", "L1_SingleJet200", "L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min300_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min330_dEta_Max1p5"]

#Part 1: create reco::PFJet
process.scoutingToRecoJets = cms.EDProducer('Run3ScoutingPFJetToRecoPFJetProducer',
                                     scoutingPFJet = cms.InputTag("hltScoutingPFPacker")
                                     )

#Patr 2: Jet corrector
process.hltAK4PFFastJetCorrector = cms.EDProducer("L1FastjetCorrectorProducer",
    algorithm = cms.string("AK4PFHLT"),
    level = cms.string("L1FastJet"),
    srcRho = cms.InputTag("hltScoutingPFPacker", "rho")
)

process.hltAK4PFRelativeCorrector = cms.EDProducer( "LXXXCorrectorProducer",
    algorithm = cms.string( "AK4PFHLT" ),
    level = cms.string( "L2Relative" )
)

process.hltAK4PFAbsoluteCorrector = cms.EDProducer( "LXXXCorrectorProducer",
    algorithm = cms.string( "AK4PFHLT" ),
    level = cms.string( "L3Absolute" )
)

process.hltAK4PFResidualCorrector = cms.EDProducer( "LXXXCorrectorProducer",
    algorithm = cms.string( "AK4PFHLT" ),
    level = cms.string( "L2L3Residual" )
)

process.hltAK4PFCorrector = cms.EDProducer("ChainedJetCorrectorProducer",
    correctors = cms.VInputTag(["hltAK4PFFastJetCorrector", "hltAK4PFRelativeCorrector", "hltAK4PFAbsoluteCorrector", "hltAK4PFResidualCorrector" ])
)
process.scoutingPFJetCorrected = cms.EDProducer("CorrectedPFJetProducer",
    correctors = cms.VInputTag(["hltAK4PFCorrector"]),
    src = cms.InputTag("scoutingToRecoJets"),
)
#Part 3: analyzer
process.triggerEffs = cms.EDAnalyzer('TriggerEffs',
                                     hasJEC = cms.bool(options.hasJEC),
                                     isMC = cms.bool(options.isMC),
                                     triggerresults   = cms.InputTag("TriggerResults", "", "HLT"),
                                     ReadPrescalesFromFile = cms.bool( False ),
                                     AlgInputTag       = cms.InputTag("gtStage2Digis"),
                                     l1tAlgBlkInputTag = cms.InputTag("gtStage2Digis"),
                                     l1tExtBlkInputTag = cms.InputTag("gtStage2Digis"),
                                     luminosity = cms.double(options.lumi), #2024 luminosity (fb-1)
                                     crossSection = cms.double(options.crossSection), # cross section in fb
                                     l1Seeds           = cms.vstring(L1Info),
                                     pfjets            = cms.InputTag("hltScoutingPFPacker"),
                                     pfjetsCorrected   = pfjetsCorrectedTag,
                                     generatorName = cms.InputTag('generator'),
                                     truePileup        = truePileupTag,
                                     scoutingMuon = cms.InputTag('hltScoutingMuonPackerNoVtx'), #remove the NoVtx if 2023 data
                                     scoutingParticle = cms.InputTag("hltScoutingPFPacker"),
                                     muon_pt = cms.double(20),
                                     muon_eta = cms.double(2.4),
                                     muon_chi2 = cms.double(10),
                                     muon_trackLayers = cms.int32(5),
                                     muon_pixelHits = cms.int32(0),
                                     muon_muonHits = cms.int32(0),
                                     muon_matchedStation = cms.int32(1),
                                     matchingTolerance = cms.double(0.1),
                                     muon_iso_max = cms.double(0.10)
)

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree_corrected.root")
                                   )
# Usually it is better to put producers on a task instead of a path
# but paths also work.

process.p = cms.Path(
    process.scoutingToRecoJets *
    process.hltAK4PFFastJetCorrector *
    process.hltAK4PFRelativeCorrector *
    process.hltAK4PFAbsoluteCorrector *
    process.hltAK4PFResidualCorrector *
    process.hltAK4PFCorrector *
    process.scoutingPFJetCorrected *
    process.gtStage2Digis *
    process.triggerEffs
)

#process.e = cms.EndPath(process.out)