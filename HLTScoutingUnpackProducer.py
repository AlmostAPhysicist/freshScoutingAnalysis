import FWCore.ParameterSet.Config as cms

process = cms.Process("TrackProducer")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.MessageLogger.cerr.FwkSummary.reportEvery = 100
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10000) )

process.source = cms.Source("PoolSource",
    # Test file generated on CMSSW 13.3.0
    fileNames = cms.untracked.vstring( #add test file here
    )
)

#Choosing the GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '133X_mcRun3_2024_realistic_v9', '')  

# Input tags to the EDProducer
process.hltScoutingUnpackProducer = cms.EDProducer('HLTScoutingUnpackProducer',
  scoutingTrack = cms.InputTag('hltScoutingTrackPacker'),
  scoutingPrimaryVertex = cms.InputTag("hltScoutingPrimaryVertexPacker","primaryVtx"),
  producePFCHSCandidate = cms.bool(False),
  mightGet = cms.optional.untracked.vstring
)

# Save only the scouting collections on the output file
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('scout.root'),
    outputCommands = cms.untracked.vstring('drop *', 'keep *_hltGtStage2ObjectMap_*_*', 'keep *_TriggerResults_*_*', 'keep *_hltFEDSelectorL1_*_*', 'keep *_hltScouting*_*_*', 'keep *_genParticles_*_*', 'keep *_prunedTrackingParticles_*_*','keep GenEventInfoProduct_*_*_*')
)

# Usually it is better to put producers on a task instead of a path
# but paths also work.
process.p = cms.Path(process.hltScoutingUnpackProducer)
process.e = cms.EndPath(process.out)
