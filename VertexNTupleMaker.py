import FWCore.ParameterSet.Config as cms
import os
import FWCore.ParameterSet.VarParsing as VarParsing

process = cms.Process("DisplacedVertexNtupleMaker")
options = VarParsing.VarParsing ('analysis')
options.register('isScouting',
                 True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "If scouting or reco tracks should be used for vertexing"
    )
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

options.parseArguments()
process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.MessageLogger.cerr.FwkSummary.reportEvery = 100
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.source = cms.Source("PoolSource",
    # Test file generated on CMSSW 13.3.0
    fileNames = cms.untracked.vstring(
        #'file:/afs/cern.ch/work/r/rmccarth/private/MiniAODSIM_QCD-HT_1500to2000.root'
    )
)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

#Choosing the GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '133X_mcRun3_2024_realistic_v9', '')  

process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree.root")
                                   )

if(options.isScouting):
    scoutingTrackTag = cms.InputTag('hltScoutingTrackPacker')
    scoutingPVTag = cms.InputTag("hltScoutingPrimaryVertexPacker","primaryVtx")
    pfCandTag = cms.InputTag("")
    lostTrackTag = cms.InputTag("")
else:
    scoutingTrackTag = cms.InputTag("")
    scoutingPVTag = cms.InputTag("")
    pfCandTag = cms.InputTag("packedPFCandidates")
    lostTrackTag = cms.InputTag("lostTracks")
    
# Input tags to the EDProducer
process.hltScoutingUnpackProducer = cms.EDProducer('HLTScoutingUnpackProducer',
  scoutingTrack = scoutingTrackTag,
  scoutingPrimaryVertex = scoutingPVTag,
  pfCand = pfCandTag,
  lostTrack = lostTrackTag,
  isScouting = cms.bool(options.isScouting),
  producePFCHSCandidate = cms.bool(False),
  mightGet = cms.optional.untracked.vstring
)

process.Vertexer = cms.EDProducer('Vertexer',
                                  seed_tracks_src = cms.InputTag('hltScoutingUnpackProducer', 'Track'),
                                  #kvr_params = kvr_params,
                                  #do_track_refinement = cms.bool(False), # remove tracks + trim out tracks with IP significance larger than trackrefine_sigmacut and trackrefine_trimmax, respectively
                                  resolve_split_vertices_loose = cms.bool(False), # an alternative merging routine with `loose` criteria, to merge any nearby vertices within a given dist or significance
                                  resolve_split_vertices_tight = cms.bool(True), # merging routine, based on vtx dphi and dVV
                                  investigate_merged_vertices = cms.bool(False), # investigate quality cuts on merged vertices from tight merging
                                  #resolve_shared_jets = cms.bool(True),       # shared-jet mitigation
                                  #resolve_shared_jets_src = cms.InputTag('selectedPatJets'),
                                  beamspot_src = cms.InputTag('offlineBeamSpot'),
                                  n_tracks_per_seed_vertex = cms.int32(2),
                                  max_seed_vertex_chi2 = cms.double(5),
                                  use_2d_vertex_dist = cms.bool(False),
                                  use_2d_track_dist = cms.bool(False),
                                  merge_anyway_dist = cms.double(-1),
                                  merge_anyway_sig = cms.double(4), # merging criteria for loose merging (*only* if resolve_split_vertices_loose is True)
                                  merge_shared_dist = cms.double(-1),
                                  merge_shared_sig = cms.double(4), # default merging shared-track vertices
                                  max_track_vertex_dist = cms.double(-1),
                                  max_track_vertex_sig = cms.double(5), # default track arbitration
                                  min_track_vertex_sig_to_remove = cms.double(1.5), # default track arbitration
                                  remove_one_track_at_a_time = cms.bool(True),
                                  max_nm1_refit_dist3 = cms.double(-1),
                                  max_nm1_refit_distz = cms.double(0.005),
                                  max_nm1_refit_count = cms.int32(-1),
                                  #trackrefine_sigmacut = cms.double(5), # track refinement criteria (*only* if do_track_refinement = True)
                                  #trackrefine_trimmax = cms.double(5), # track refinement criteria (*only* if do_track_refinement = True)
                                  verbose = cms.bool(False),
                                  )

#The L1 seeds used for JetHT
L1Info = ["L1_HTT200er", "L1_HTT255er", "L1_HTT280er", "L1_HTT320er", "L1_HTT360er", "L1_HTT400er", "L1_HTT450er", "L1_ETT2000", "L1_SingleJet180", "L1_SingleJet200", "L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min300_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min330_dEta_Max1p5"]

if(options.isScouting):
    pfjetsTag = cms.InputTag("hltScoutingPFPacker")
    patjetsTag = cms.InputTag("")
    pvTag = cms.InputTag("hltScoutingUnpackProducer","PrimaryVertex")
else:
    pfjetsTag = cms.InputTag("")
    patjetsTag = cms.InputTag("slimmedJets")
    pvTag = cms.InputTag("offlineSlimmedPrimaryVertices")
process.scoutingTree = cms.EDAnalyzer('ScoutingTreeMakerRun3',
                                      required_ntk     = cms.int32(2),
                                      triggerresults   = cms.InputTag("TriggerResults", "", "HLT"),
                                      ReadPrescalesFromFile = cms.bool( False ),
                                      AlgInputTag       = cms.InputTag("gtStage2Digis"),
                                      l1tAlgBlkInputTag = cms.InputTag("gtStage2Digis"),
                                      l1tExtBlkInputTag = cms.InputTag("gtStage2Digis"),
                                      doL1 = cms.bool( True ),
                                      doPhiCorrection = cms.bool( False ),
                                      doGenMatching = cms.bool( False ),
                                      luminosity = cms.double(options.lumi), #2024 luminosity (fb-1)
                                      crossSection = cms.double(options.crossSection), # cross section in fb
                                      l1Seeds           = cms.vstring(L1Info),
                                      muons             = cms.InputTag("hltScoutingMuonPacker"),
                                      electrons         = cms.InputTag("hltScoutingEgammaPacker"),
                                      photons           = cms.InputTag("hltScoutingEgammaPacker"),
                                      pfjets            = pfjetsTag,
                                      patjets           = patjetsTag,
                                      tracks            = cms.InputTag("hltScoutingUnpackProducer","Track"),
                                      primaryVertices   = pvTag,
                                      displacedVertices = cms.InputTag("Vertexer"),
                                      pfMet             = cms.InputTag("hltScoutingPFPacker","pfMetPt"),
                                      pfMetPhi          = cms.InputTag("hltScoutingPFPacker","pfMetPhi"),
                                      rho               = cms.InputTag("hltScoutingPFPacker","rho"),
                                      beamspot_src = cms.InputTag('offlineBeamSpot'),
                                      genParticle_src = cms.InputTag('genParticles',''),
                                      generatorName = cms.InputTag('generator')
                                      )

# Usually it is better to put producers on a task instead of a path
# but paths also work.
process.p = cms.Path(process.hltScoutingUnpackProducer+process.offlineBeamSpot+process.Vertexer+process.gtStage2Digis+process.scoutingTree)

