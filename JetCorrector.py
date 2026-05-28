import FWCore.ParameterSet.Config as cms
import os
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("JetCorrector")
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
        'file:scoutToRecoJet.root'
    )
)

#Choosing the GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag

if(options.isMC):
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_mcRun3_2024_realistic_v26', '')  
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_HLT_v3', '')


# update JEC
process.GlobalTag.toGet = cms.VPSet(
  cms.PSet( # hlt AK4PFHLT latest
    record = cms.string("JetCorrectionsRecord"),
    tag = cms.string("JetCorrectorParametersCollection_AK4PFHLT_hlt_v1"),
    label = cms.untracked.string("AK4PFHLT"),
    connect = cms.string("frontier://FrontierProd/CMS_CONDITIONS")
  )
)

# Scouting
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
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('scoutJetCorrected.root'),
)

# Usually it is better to put producers on a task instead of a path
# but paths also work.
process.p = cms.Path(
    process.hltAK4PFFastJetCorrector *
    process.hltAK4PFRelativeCorrector *
    process.hltAK4PFAbsoluteCorrector *
    process.hltAK4PFResidualCorrector *
    process.hltAK4PFCorrector *
    process.scoutingPFJetCorrected
)
process.e = cms.EndPath(process.out)