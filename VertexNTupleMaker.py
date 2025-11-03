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
options.register('hasReco',
                 True,
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "If there are reco tracks to compare with the scouting"
    )
#Will use the reco derived from the scouting if hasReco is turned to false, so deviation plots entries should all be 0
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
                 True,
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
        '/store/mc/RunIII2024Summer24MiniAOD/QCD-4Jets_Bin-HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/MINIAODSIM/140X_mcRun3_2024_realistic_v26-v2/100000/00f7403b-49bf-4efd-9b8f-0398bd61d910.root'
        #'/store/user/brlopesd/StopStopbarTo2Dbar2D_M-200_CTau-1mm_Summer24_100k_v2/StopStopbarTo2Dbar2D_M-200_CTau-1mm_Summer24_100k_miniAOD_v2/250214_150834/0000/stop_dbar_miniAOD_1.root'
        #Data test file
        #'/store/data/Run2024D/ScoutingPFRun3/HLTSCOUT/v1/000/380/945/00000/cdf45723-07c4-4b41-9595-f368f2929369.root'
        #PF monitor file
        #'/store/data/Run2024D/ScoutingPFMonitor/MINIAOD/PromptReco-v1/000/380/306/00000/70ec6086-72c5-4562-82a8-1f043e645d59.root'
    )
)

process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

#Choosing the GlobalTag
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag

if(options.isMC):
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_mcRun3_2024_realistic_v26', '')  
    truePileupTag = cms.InputTag("slimmedAddPileupInfo")
else:
    process.GlobalTag = GlobalTag(process.GlobalTag, '140X_dataRun3_Prompt_v4', '')
    truePileupTag = cms.InputTag("")
    
process.load("RecoVertex.BeamSpotProducer.BeamSpot_cfi")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

process.load("EventFilter.L1TRawToDigi.gtStage2Digis_cfi")
process.gtStage2Digis.InputLabel = cms.InputTag( "hltFEDSelectorL1" )

process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree.root")
                                   )

if(options.isScouting):
    #unpacker
    scoutingTrackTag = cms.InputTag('hltScoutingTrackPacker')
    scoutingPVTag = cms.InputTag("hltScoutingPrimaryVertexPacker","primaryVtx")
    scoutingPFTag = cms.InputTag("hltScoutingPFPacker")
    pfCandTag = cms.InputTag("")
    lostTrackTag = cms.InputTag("")

    #skim
    pfjetsTag = cms.InputTag("hltScoutingPFPacker")
    patjetsTag = cms.InputTag("")

    #tree maker
    skimPFJetsTag = cms.InputTag("triggerFilter","pfjets")
    skimPatJetsTag = cms.InputTag("")
    pvTag = cms.InputTag("hltScoutingUnpackProducer","PrimaryVertex")
else:
    #unpacker
    scoutingTrackTag = cms.InputTag("")
    scoutingPVTag = cms.InputTag("")
    scoutingPFTag = cms.InputTag("")
    pfCandTag = cms.InputTag("packedPFCandidates")
    lostTrackTag = cms.InputTag("lostTracks")

    #skim
    pfjetsTag = cms.InputTag("")
    patjetsTag = cms.InputTag("slimmedJets")

    #tree maker
    skimPFJetsTag = cms.InputTag("")
    skimPatJetsTag = cms.InputTag("triggerFilter","patjets")
    pvTag = cms.InputTag("offlineSlimmedPrimaryVertices")

if(options.hasReco):
    recoTrackTag = cms.InputTag("displacedTracks")
else:
    recoTrackTag = cms.InputTag("hltScoutingUnpackProducer","Track")
    
# Input tags to the EDProducer
process.hltScoutingUnpackProducer = cms.EDProducer('HLTScoutingUnpackProducer',
  scoutingTrack = scoutingTrackTag,
  scoutingPrimaryVertex = scoutingPVTag,
  scoutingParticle = scoutingPFTag,
  pfCand = pfCandTag,
  lostTrack = lostTrackTag,
  isScouting = cms.bool(options.isScouting),
  producePFCHSCandidate = cms.bool(False),
  mightGet = cms.optional.untracked.vstring
)

#The L1 seeds used for JetHT
L1Info = ["L1_HTT200er", "L1_HTT255er", "L1_HTT280er", "L1_HTT320er", "L1_HTT360er", "L1_HTT400er", "L1_HTT450er", "L1_ETT2000", "L1_SingleJet180", "L1_SingleJet200", "L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min300_dEta_Max1p5", "L1_DoubleJet30er2p5_Mass_Min330_dEta_Max1p5"]

process.triggerFilter = cms.EDFilter('TriggerFilter',
                                     isMC = cms.bool(options.isMC),
                                     triggerresults   = cms.InputTag("TriggerResults", "", "HLT"),
                                     AlgInputTag       = cms.InputTag("gtStage2Digis"),
                                     l1tExtBlkInputTag = cms.InputTag("gtStage2Digis"),
                                     isScouting = cms.bool(options.isScouting),
                                     luminosity = cms.double(options.lumi), #2024 luminosity (fb-1)
                                     crossSection = cms.double(options.crossSection), # cross section in fb
                                     l1Seeds           = cms.vstring(L1Info),
                                     pfjets            = pfjetsTag,
                                     patjets           = patjetsTag,
                                     generatorName = cms.InputTag('generator'),
                                     genJet_src = cms.InputTag('slimmedGenJets',''),
                                     storeGenJets = cms.bool(False)
                                     )

process.Vertexer = cms.EDProducer('Vertexer',
                                  seed_tracks_src = cms.InputTag('hltScoutingUnpackProducer', 'Track'),
                                  pt_min_cut = cms.double(1.0),
                                  dxySig_min_cut = cms.double(2.5),
                                  dxySig_max_cut = cms.double(5.0), #dxySig between 0.5 and 2.5 for a control region
                                  npixelHits_min_cut = cms.int32(1),
                                  nstripHits_min_cut = cms.int32(0),
                                  ntrackerLayers_min_cut = cms.int32(5),
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
                                  max_nm1_refit_dist3 = cms.double(0.002),
                                  max_nm1_refit_distz = cms.double(-1),
                                  max_nm1_refit_count = cms.int32(-1),
                                  #trackrefine_sigmacut = cms.double(5), # track refinement criteria (*only* if do_track_refinement = True)
                                  #trackrefine_trimmax = cms.double(5), # track refinement criteria (*only* if do_track_refinement = True)
                                  verbose = cms.bool(False),
                                  )

process.scoutingTree = cms.EDAnalyzer('ScoutingTreeMakerRun3',
                                      isMC = cms.bool(options.isMC),
                                      required_ntk     = cms.int32(4),
                                      triggerresults   = cms.InputTag("TriggerResults", "", "HLT"),
                                      ReadPrescalesFromFile = cms.bool( False ),
                                      AlgInputTag       = cms.InputTag("gtStage2Digis"),
                                      l1tAlgBlkInputTag = cms.InputTag("gtStage2Digis"),
                                      l1tExtBlkInputTag = cms.InputTag("gtStage2Digis"),
                                      doTrigger = cms.bool( True ),
                                      isScouting = cms.bool(options.isScouting),
                                      doPhiCorrection = cms.bool( False ),
                                      doGenMatching = cms.bool( False ),
                                      fillScoutTrack = cms.bool( True ),
                                      luminosity = cms.double(options.lumi), #2024 luminosity (fb-1)
                                      crossSection = cms.double(options.crossSection), # cross section in fb
                                      truePileup        = truePileupTag,
                                      l1Seeds           = cms.vstring(L1Info),
                                      muons             = cms.InputTag("hltScoutingMuonPacker"),
                                      electrons         = cms.InputTag("hltScoutingEgammaPacker"),
                                      photons           = cms.InputTag("hltScoutingEgammaPacker"),
                                      pfjets            = skimPFJetsTag,
                                      patjets           = skimPatJetsTag,
                                      tracks            = cms.InputTag("hltScoutingUnpackProducer","Track"),
                                      trackRefs         = cms.InputTag("hltScoutingUnpackProducer", "Track-RefToOriginal"),
                                      recoTrack         = recoTrackTag,
                                      vertexShiftZMap   = cms.InputTag("Vertexer","vtxZShift"),
                                      vertexShift3DMap  = cms.InputTag("Vertexer","vtx3DShift"),
                                      primaryVertices   = pvTag,
                                      displacedVertices = cms.InputTag("Vertexer"),
                                      pfMet             = cms.InputTag("hltScoutingPFPacker","pfMetPt"),
                                      pfMetPhi          = cms.InputTag("hltScoutingPFPacker","pfMetPhi"),
                                      rho               = cms.InputTag("hltScoutingPFPacker","rho"),
                                      beamspot_src = cms.InputTag('offlineBeamSpot'),
                                      genParticle_src = cms.InputTag('genParticles',''),
                                      genJet_src = cms.InputTag('slimmedGenJets',''),
                                      generatorName = cms.InputTag('generator'),
                                      scoutingParticle = scoutingPFTag
                                      )
# Usually it is better to put producers on a task instead of a path
# but paths also work.
process.p = cms.Path(process.hltScoutingUnpackProducer+process.gtStage2Digis+process.triggerFilter+process.offlineBeamSpot+process.Vertexer+process.scoutingTree)
