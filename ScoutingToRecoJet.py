import FWCore.ParameterSet.Config as cms
import os
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("ScoutingToRecoJet")
options = VarParsing.VarParsing ('analysis')

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
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_HLT_v3', '')

process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )


process.scoutingToRecoJets = cms.EDProducer('Run3ScoutingPFJetToRecoPFJetProducer',
                                     scoutingPFJet = cms.InputTag("hltScoutingPFPacker")
                                     )

process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('scoutToRecoJet.root'),
)
# Usually it is better to put producers on a task instead of a path
# but paths also work.
process.p = cms.Path(process.scoutingToRecoJets)
process.e = cms.EndPath(process.out)