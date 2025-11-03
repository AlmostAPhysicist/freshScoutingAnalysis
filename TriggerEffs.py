import FWCore.ParameterSet.Config as cms
import os
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("TriggerEffs")
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

options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.MessageLogger.cerr.FwkSummary.reportEvery = 100
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(1000) )

process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
        #MC test file
        #'/store/mc/RunIII2024Summer24MiniAOD/QCD-4Jets_Bin-HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/MINIAODSIM/140X_mcRun3_2024_realistic_v26-v2/100000/00f7403b-49bf-4efd-9b8f-0398bd61d910.root'
        #Data test file
        '/store/data/Run2024D/ScoutingPFRun3/HLTSCOUT/v1/000/380/945/00000/cdf45723-07c4-4b41-9595-f368f2929369.root'
        #PF monitor file
        #'/store/data/Run2024D/ScoutingPFMonitor/MINIAOD/PromptReco-v1/000/380/306/00000/70ec6086-72c5-4562-82a8-1f043e645d59.root'
    )
)


#Choosing the GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag

if(options.isMC):
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_mcRun3_2024_realistic_v26', '')  
    truePileupTag = cms.InputTag("slimmedAddPileupInfo")
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_Prompt_v4', '')
    truePileupTag = cms.InputTag("")

process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree.root")
                                   )


pfjetsTag = cms.InputTag("hltScoutingPFPacker")

#The L1 seeds used for JetHT
L1Info = ["L1_HTT200er", "L1_HTT255er", "L1_HTT280er", "L1_HTT320er", "L1_HTT360er", "L1_HTT400er", "L1_HTT450er", "L1_ETT2000", "L1_SingleJet180", "L1_SingleJet200", "L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min300_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min330_dEta_Max1p5"]

process.triggerEffs = cms.EDAnalyzer('TriggerEffs',
                                     isMC = cms.bool(options.isMC),
                                     triggerresults   = cms.InputTag("TriggerResults", "", "HLT"),
                                     ReadPrescalesFromFile = cms.bool( False ),
                                     AlgInputTag       = cms.InputTag("gtStage2Digis"),
                                     l1tAlgBlkInputTag = cms.InputTag("gtStage2Digis"),
                                     l1tExtBlkInputTag = cms.InputTag("gtStage2Digis"),
                                     luminosity = cms.double(options.lumi), #2024 luminosity (fb-1)
                                     crossSection = cms.double(options.crossSection), # cross section in fb
                                     l1Seeds           = cms.vstring(L1Info),
                                     pfjets            = pfjetsTag,
                                     generatorName = cms.InputTag('generator'),
                                     scoutingMuon = cms.InputTag('hltScoutingMuonPackerNoVtx'), #remove the NoVtx if 2023 data
                                     muon_pt = cms.double(27),
                                     muon_eta = cms.double(2.4),
                                     muon_chi2 = cms.double(10),
                                     muon_trackLayers = cms.int32(5),
                                     muon_pixelHits = cms.int32(0),
                                     muon_muonHits = cms.int32(0),
                                     muon_matchedStation = cms.int32(1),
                                     )


# Usually it is better to put producers on a task instead of a path
# but paths also work.
process.p = cms.Path(process.gtStage2Digis+process.triggerEffs)
