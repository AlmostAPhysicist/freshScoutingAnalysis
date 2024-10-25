import FWCore.ParameterSet.Config as cms

process = cms.Process("TESTVERTEX")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

process.MessageLogger.cerr.FwkSummary.reportEvery = 1
process.MessageLogger.cerr.FwkReport.reportEvery = 1

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(10) ) #10 events to debug

process.source = cms.Source("PoolSource",
    # Test file generated on CMSSW 13.3.0
    fileNames = cms.untracked.vstring( 
        'file:scout_test.root'
    )
)

#Choosing the GlobalTag
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load('Configuration.StandardSequences.GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.GlobalTag import GlobalTag
process.GlobalTag = GlobalTag(process.GlobalTag, '133X_mcRun3_2024_realistic_v9', '')  
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")

# Input tags to the EDProducer
process.Vertexer = cms.EDProducer('Vertexer',
                                  seed_tracks_src = cms.InputTag('hltScoutingUnpackProducer', 'Track'),
                                  #kvr_params = kvr_params,
                                  #do_track_refinement = cms.bool(False), # remove tracks + trim out tracks with IP significance larger than trackrefine_sigmacut and trackrefine_trimmax, respectively   
                                  #resolve_split_vertices_loose = cms.bool(False), # an alternative merging routine with `loose` criteria, to merge any nearby vertices within a given dist or significance
                                  #resolve_split_vertices_tight = cms.bool(True), # merging routine, based on vtx dphi and dVV 
                                  #investigate_merged_vertices = cms.bool(False), # investigate quality cuts on merged vertices from tight merging 
                                  #resolve_shared_jets = cms.bool(True),       # shared-jet mitigation
                                  #resolve_shared_jets_src = cms.InputTag('selectedPatJets'), 
                                  #beamspot_src = cms.InputTag('offlineBeamSpot'),
                                  n_tracks_per_seed_vertex = cms.int32(2),
                                  max_seed_vertex_chi2 = cms.double(5),
                                  use_2d_vertex_dist = cms.bool(False),
                                  use_2d_track_dist = cms.bool(False),
                                  #merge_anyway_dist = cms.double(-1),
                                  #merge_anyway_sig = cms.double(4), # merging criteria for loose merging (*only* if resolve_split_vertices_loose is True)
                                  merge_shared_dist = cms.double(-1),
                                  merge_shared_sig = cms.double(4), # default merging shared-track vertices  
                                  max_track_vertex_dist = cms.double(-1),
                                  max_track_vertex_sig = cms.double(5), # default track arbitration
                                  min_track_vertex_sig_to_remove = cms.double(1.5), # default track arbitration
                                  remove_one_track_at_a_time = cms.bool(True),
                                  #max_nm1_refit_dist3 = cms.double(-1),
                                  #max_nm1_refit_distz = cms.double(0.005),
                                  #max_nm1_refit_count = cms.int32(-1),
                                  #trackrefine_sigmacut = cms.double(5), # track refinement criteria (*only* if do_track_refinement = True)
                                  #trackrefine_trimmax = cms.double(5), # track refinement criteria (*only* if do_track_refinement = True)
                                  )

# Save only the scouting collections on the output file
process.out = cms.OutputModule("PoolOutputModule",
    fileName = cms.untracked.string('vertexer_test.root'),
    #outputCommands = cms.untracked.vstring('drop *', 'keep *_hltGtStage2ObjectMap_*_*', 'keep *_TriggerResults_*_*', 'keep *_hltScouting*_*_*')
)

# Usually it is better to put producers on a task instead of a path
# but paths also work.
process.p = cms.Path(process.Vertexer)
process.e = cms.EndPath(process.out)
