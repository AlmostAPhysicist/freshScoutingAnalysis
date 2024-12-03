// -*- C++ -*-
//
// Package:    Run3ScoutingAnalysisTools/ScoutingTreeMakerRun3
// Class:      ScoutingTreeMakerRun3
//
/**\class ScoutingTreeMakerRun3 ScoutingTreeMakerRun3.cc Run3ScoutingAnalysisTools/ScoutingTreeMakerRun3/plugins/ScoutingTreeMakerRun3.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  David Sperka
//         Created:  Sat, 11 Feb 2023 14:15:08 GMT
//
//

// system include files
#include <memory>
#include <TTree.h>
#include <TLorentzVector.h>
#include <algorithm>
#include "TMath.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingVertex.h"

#include "DataFormats/Scouting/interface/Run3ScoutingElectron.h"
#include "DataFormats/Scouting/interface/Run3ScoutingPhoton.h"
#include "DataFormats/Scouting/interface/Run3ScoutingPFJet.h"
#include "DataFormats/Scouting/interface/Run3ScoutingVertex.h"
#include "DataFormats/Scouting/interface/Run3ScoutingTrack.h"
#include "DataFormats/Scouting/interface/Run3ScoutingMuon.h"
#include "DataFormats/Scouting/interface/Run3ScoutingParticle.h"

#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "TrackingTools/IPTools/interface/IPTools.h"

#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"
#include "DataFormats/PatCandidates/interface/PackedTriggerPrescales.h"
#include "L1Trigger/L1TGlobal/interface/L1TGlobalUtil.h"
#include "DataFormats/L1TGlobal/interface/GlobalAlgBlk.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionData.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionEvaluator.h"
#include "HLTrigger/HLTcore/interface/TriggerExpressionParser.h"

#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h" 

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

class ScoutingTreeMakerRun3 : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:

  explicit ScoutingTreeMakerRun3(const edm::ParameterSet&);
  ~ScoutingTreeMakerRun3() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;
  //void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //void endRun(edm::Run const&, edm::EventSetup const&) override;
  //void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;   

  const int required_ntk;
  
  const edm::InputTag triggerResultsTag;
  const edm::EDGetTokenT<edm::TriggerResults>             triggerResultsToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingMuon> >      muonsToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingElectron> >  electronsToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingVertex> >    primaryVerticesToken;
  //const edm::EDGetTokenT<std::vector<Run3ScoutingVertex> >    verticesToken;
  const edm::EDGetTokenT<double>                          rhoToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingPhoton> >  photonsToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingParticle> >  pfcandsToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingPFJet> >  pfjetsToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingTrack> >  tracksToken;
  const edm::EDGetTokenT<std::vector<reco::Vertex> >  verticesToken;

  const edm::EDGetTokenT<reco::BeamSpot> beamspot_token;
  const edm::EDGetTokenT<std::vector<reco::GenParticle>> GenParticleToken_;
  
  
  std::vector<std::string> triggerPathsVector;
  std::map<std::string, int> triggerPathsMap;

  bool doL1;
  triggerExpression::Data triggerCache_;

  bool l1_HTT280;
  
  edm::InputTag                algInputTag_;
  edm::InputTag                extInputTag_;
  edm::EDGetToken              algToken_;
  std::unique_ptr<l1t::L1TGlobalUtil> l1GtUtils_;
  std::vector<std::string>     l1Seeds_;
  std::vector<bool>            l1Result_;

  TTree* tree;

  int nPFJets;

  float HT;
  
  float ptjet1;
  float ptjet2;
  float ptjet3;
  float ptjet4;

  float etajet1;
  float etajet2;
  float etajet3;
  float etajet4;

  float phijet1;
  float phijet2;
  float phijet3;
  float phijet4; 
  
  int nVertices;
  
  int ntk_1;
  int ntk_2;
  
  float bs2derr_1;
  float bs2derr_2;

  float dBV_1;
  float dBV_2;

  float genVert_x = -999;
  float genVert_y = -999;
  float genVert_z = -999;
  float genVert_dBV = -999;
  float genVert_3d = -999;
  float genVert_motherEta = -999;
  float genVert_motherPhi = -999;
  float genVert_motherPt = -999;
  float genVert_motherDistTraveled = -999;
  float genVert_dVV = -999;
  float genVert_dPhi = -999;
  
  typedef std::set<reco::TrackRef> track_set;
  typedef std::vector<reco::TrackRef> track_vec;

  bool isStopDecay(const reco::Candidate* part){
    if(fabs(part->pdgId())==1000006 && part->numberOfDaughters()==2){
      int daughterId = part->daughter(0)->pdgId();
      bool sameDaughters = (daughterId==part->daughter(1)->pdgId());
      if(sameDaughters && fabs(daughterId)==1){
	return true;
      }
    }
    return false;
  }

  bool isStopDecay(std::vector<reco::GenParticle>::const_iterator part){
    if(fabs(part->pdgId())==1000006 && part->numberOfDaughters()==2){
      int daughterId = part->daughter(0)->pdgId();
      bool sameDaughters = (daughterId==part->daughter(1)->pdgId());
      if(sameDaughters && fabs(daughterId)==1){
	return true;
      }
    }
    return false;
  }

  const reco::Candidate* getFinalCopy(std::vector<reco::GenParticle>::const_iterator part){
    const reco::Candidate* candidate = part->clone();
    if(part->numberOfDaughters()==0) return candidate;
    bool foundLast = false;
    while(!foundLast){
      int selfIndex = -1;
      for(uint i=0; i<candidate->numberOfDaughters(); i++){
	if(candidate->daughter(i)->pdgId()==candidate->pdgId()) selfIndex = i;
      }
      if(selfIndex==-1){
	foundLast=true;
      }
      else{
	candidate = candidate->daughter(selfIndex);
      }
    }
    return candidate;
    
  }
  
  track_set vertex_track_set(const reco::Vertex & v, const double min_weight = 0.5) const {
    track_set result;
    
    for (auto it = v.tracks_begin(), ite = v.tracks_end(); it != ite; ++it) {
      const double w = v.trackWeight(*it);
      const bool use = w >= min_weight;
      assert(use);
      if (use)
	result.insert(it->castTo<reco::TrackRef>());
    }
    
    return result;
  }
  
  track_vec vertex_track_vec(const reco::Vertex & v, const double min_weight = 0.5) const {
    track_set s = vertex_track_set(v, min_weight);
    return track_vec(s.begin(), s.end());
  }


  std::pair<size_t, size_t> findTwoLargestIndices(const std::vector<double>& vec) {
    int firstIndex = -1, secondIndex = -1;
    float firstMax = -std::numeric_limits<float>::infinity();
    float secondMax = -std::numeric_limits<float>::infinity();

    for (size_t i = 0; i < vec.size(); ++i) {
      if (vec[i] > firstMax) {
	secondMax = firstMax;
	secondIndex = firstIndex;
	firstMax = vec[i];
	firstIndex = i;
      } else if (vec[i] > secondMax) {
	secondMax = vec[i];
	secondIndex = i;
      }
    }
    
    return {firstIndex, secondIndex};
  }
  
};

ScoutingTreeMakerRun3::ScoutingTreeMakerRun3(const edm::ParameterSet& iConfig):
  required_ntk             (iConfig.getParameter<int>("required_ntk")),
  triggerResultsTag        (iConfig.getParameter<edm::InputTag>("triggerresults")),
  triggerResultsToken      (consumes<edm::TriggerResults>                    (triggerResultsTag)),
  muonsToken               (consumes<std::vector<Run3ScoutingMuon> >             (iConfig.getParameter<edm::InputTag>("muons"))),
  electronsToken           (consumes<std::vector<Run3ScoutingElectron> >         (iConfig.getParameter<edm::InputTag>("electrons"))),
  primaryVerticesToken     (consumes<std::vector<Run3ScoutingVertex> >           (iConfig.getParameter<edm::InputTag>("primaryVertices"))),
  //verticesToken            (consumes<std::vector<Run3ScoutingVertex> >           (iConfig.getParameter<edm::InputTag>("displacedVertices"))),
  rhoToken                 (consumes<double>                                 (iConfig.getParameter<edm::InputTag>("rho"))), 
  photonsToken             (consumes<std::vector<Run3ScoutingPhoton> >         (iConfig.getParameter<edm::InputTag>("photons"))),
  pfcandsToken             (consumes<std::vector<Run3ScoutingParticle> >         (iConfig.getParameter<edm::InputTag>("pfcands"))),
  pfjetsToken              (consumes<std::vector<Run3ScoutingPFJet> >            (iConfig.getParameter<edm::InputTag>("pfjets"))),
  tracksToken              (consumes<std::vector<Run3ScoutingTrack> >            (iConfig.getParameter<edm::InputTag>("tracks"))),
  verticesToken            (consumes<std::vector<reco::Vertex> >           (iConfig.getParameter<edm::InputTag>("displacedVertices"))),
  
  beamspot_token(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamspot_src"))),
  GenParticleToken_(consumes(iConfig.getParameter<edm::InputTag>("genParticle_src"))),
  
    doL1                     (iConfig.existsAs<bool>("doL1")               ?    iConfig.getParameter<bool>  ("doL1")            : false)
{
    usesResource("TFileService");
    if (doL1) {
        algInputTag_ = iConfig.getParameter<edm::InputTag>("AlgInputTag");
        extInputTag_ = iConfig.getParameter<edm::InputTag>("l1tExtBlkInputTag");
        algToken_ = consumes<BXVector<GlobalAlgBlk>>(algInputTag_);
        l1Seeds_ = iConfig.getParameter<std::vector<std::string> >("l1Seeds");
        l1GtUtils_ = std::make_unique<l1t::L1TGlobalUtil>(iConfig, consumesCollector(), *this, algInputTag_, extInputTag_, l1t::UseEventSetupIn::Event);
    }
    else {
        l1Seeds_ = std::vector<std::string>();
        l1GtUtils_ = 0;
    }
}

ScoutingTreeMakerRun3::~ScoutingTreeMakerRun3() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//


// ------------ method called for each event  ------------
void ScoutingTreeMakerRun3::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;
  using namespace reco;
  
  //Get the jets
  Handle<vector<Run3ScoutingPFJet> > pfjetsH;
  iEvent.getByToken(pfjetsToken, pfjetsH);

  //Require 4 PF Jets 
  nPFJets = pfjetsH->size();
  if (nPFJets<4) return;

  HT = 0;
  int t = 0;
  for (auto jets_iter = pfjetsH->begin(); jets_iter != pfjetsH->end(); ++jets_iter) {
    HT = HT + pfjetsH->at(t).pt();
    t++;
  }
  
  //Get the beamspot
  edm::Handle<reco::BeamSpot> beamspot;
  iEvent.getByToken(beamspot_token, beamspot);
  const reco::Vertex fake_bs_vtx(beamspot->position(), beamspot->covariance3D());

  //Get gen particles for truth-level vertices
  edm::Handle<std::vector<reco::GenParticle>> genParticle_handle;
  iEvent.getByToken(GenParticleToken_,genParticle_handle);
  std::vector<GlobalPoint> genVertices;
  
  std::vector<reco::GenParticle>::const_iterator genParticleIter;
  float maxDist = 0;
  for(genParticleIter = genParticle_handle->begin(); genParticleIter != genParticle_handle->end(); ++genParticleIter){
    if(genParticleIter->numberOfMothers()<1) continue;
    if(isStopDecay(genParticleIter->mother(0))){
      //std::cout<<"pdgid: "<<genParticleIter->pdgId()<<" vertex xyz: "<<genParticleIter->vx()<<" "<<genParticleIter->vy()<<" "<<genParticleIter->vz()<<" status: "<<genParticleIter->status()<<" parent id: "<<genParticleIter->mother(0)->pdgId()<<" parent vertex: "<<genParticleIter->mother(0)->vx()<<" "<<genParticleIter->mother(0)->vy()<<" "<<genParticleIter->mother(0)->vz()<<" parent status: "<<genParticleIter->mother(0)->status()<<std::endl;
      float dist = TMath::Sqrt(pow(genParticleIter->vx()-beamspot->x0(),2)+pow(genParticleIter->vy()-beamspot->y0(),2));
      const reco::Candidate* mother = genParticleIter->mother(0);
      while(mother->mother(0)->pdgId()==mother->pdgId()){
	mother = mother->mother(0);
      }

      bool isDupe = false;
      GlobalPoint genVertex = GlobalPoint(genParticleIter->vx(),genParticleIter->vy(),genParticleIter->vz());
      for(auto vertex: genVertices){
	if((vertex.x()==genVertex.x()) && (vertex.y()==genVertex.y()) && (vertex.z()==genVertex.z())) isDupe = true;
      }
      if(!isDupe) genVertices.push_back(genVertex);
      //std::cout<<"first mother id: "<<mother->pdgId()<<" status: "<<mother->status()<<" vertex: "<<mother->vx()<<" "<<mother->vy()<<" "<<mother->vz()<<" parent id: "<<mother->mother(0)->pdgId()<<" parent status: "<<mother->mother(0)->status()<<" mother vertex: "<<mother->mother(0)->vx()<<" "<<mother->mother(0)->vy()<<" "<<mother->mother(0)->vz()<<std::endl;
      if(dist>maxDist){
	maxDist = dist;
	genVert_x = genParticleIter->vx();
	genVert_y = genParticleIter->vy();
	genVert_z = genParticleIter->vz();
	genVert_dBV = dist;
	genVert_3d = TMath::Sqrt(pow(genParticleIter->vx()-beamspot->x0(),2)+pow(genParticleIter->vy()-beamspot->y0(),2)+pow(genParticleIter->vz()-beamspot->z0(),2));
	genVert_motherEta = mother->eta();
	genVert_motherPhi = mother->phi();
	genVert_motherPt = mother->pt();
	genVert_motherDistTraveled = TMath::Sqrt(pow(genParticleIter->vx()-mother->mother(0)->vx(),2)+pow(genParticleIter->vy()-mother->mother(0)->vy(),2)+pow(genParticleIter->vz()-mother->mother(0)->vz(),2));
      }
    } //isStopDecay
  } //loop over gen particles

  if(genVertices.size()==2){
    genVert_dVV = TMath::Sqrt(pow(genVertices[0].x()-genVertices[1].x(),2)+pow(genVertices[0].y()-genVertices[1].y(),2)+pow(genVertices[0].z()-genVertices[1].z(),2));
    float dPhi = fabs(atan2(genVertices[0].y(),genVertices[0].x())-atan2(genVertices[1].y(),genVertices[1].x()));
    if(dPhi>TMath::Pi()) dPhi = 2*TMath::Pi() - dPhi;
    genVert_dPhi = dPhi;
  }
  
  //Get the vertices
  Handle<vector<Vertex> > verticesH;
  iEvent.getByToken(verticesToken, verticesH);

  t=0;
  Vertex v;
  int ntk_t=0;
  vector<TrackRef> tks_t;

  vector<Vertex> vertices_ntk;
  
  for (auto vertex_iter = verticesH->begin(); vertex_iter != verticesH->end(); ++vertex_iter) {
    v = verticesH->at(t);
    tks_t = vertex_track_vec(v);
    ntk_t = tks_t.size();

    if (ntk_t > required_ntk - 1) vertices_ntk.push_back(v);
    t++;
  }
  
  nVertices = vertices_ntk.size();
  
  if (nVertices<2) return;
  
  //Two leading vertices
  vector<double> dBVs;
  VertexDistanceXY vertex_dist_2d;
  t=0;
  float dBV_t;
  
  for (auto vertex_iter = vertices_ntk.begin(); vertex_iter != vertices_ntk.end(); ++vertex_iter) {
    v = vertices_ntk.at(t);
    dBV_t = vertex_dist_2d.distance(v, fake_bs_vtx).value();

    dBVs.push_back(dBV_t);
    
    t++;
  }

  

  auto largest_dBV = findTwoLargestIndices(dBVs);
  
  Vertex v1 = vertices_ntk.at(largest_dBV.first);
  Vertex v2 = vertices_ntk.at(largest_dBV.second);

  //Get the jets distributions
  ptjet1 = pfjetsH->at(0).pt();
  ptjet2 = pfjetsH->at(1).pt();
  ptjet3 = pfjetsH->at(2).pt();
  ptjet4 = pfjetsH->at(3).pt();
  
  etajet1 = pfjetsH->at(0).eta();
  etajet2 = pfjetsH->at(1).eta();
  etajet3 = pfjetsH->at(2).eta();
  etajet4 = pfjetsH->at(3).eta();
  
  phijet1 = pfjetsH->at(0).phi();
  phijet2 = pfjetsH->at(1).phi();
  phijet3 = pfjetsH->at(2).phi();
  phijet4 = pfjetsH->at(3).phi();
  
  //Calculate the vertex distributions
  
  Measurement1D dBV_Meas1D_1 = vertex_dist_2d.distance(v1, fake_bs_vtx);
  Measurement1D dBV_Meas1D_2 = vertex_dist_2d.distance(v2, fake_bs_vtx);
  
  dBV_1 = dBV_Meas1D_1.value();
  bs2derr_1 = dBV_Meas1D_1.error();

  dBV_2 = dBV_Meas1D_2.value();
  bs2derr_2 = dBV_Meas1D_2.error();

  //printf("First dBV = %f, second dBV = %f\n", dBV_1, dBV_2);

  vector<TrackRef> tks_1 = vertex_track_vec(v1);
  ntk_1 = tks_1.size();

  vector<TrackRef> tks_2 = vertex_track_vec(v2);
  ntk_2 = tks_2.size();

  
  //L1 results
  l1Result_.clear();
  if (doL1) {
    l1GtUtils_->retrieveL1(iEvent,iSetup,algToken_);
    for( unsigned int iseed = 0; iseed < l1Seeds_.size(); iseed++ ) {
      bool l1htbit = 0;
      l1GtUtils_->getFinalDecisionByName(string(l1Seeds_[iseed]), l1htbit);
      l1Result_.push_back( l1htbit );
    }
    l1GtUtils_->getFinalDecisionByName(string("L1_HTT280er"), l1_HTT280);
  }
  
  tree->Fill();
    
}

//Fill the tree with the variables retrieved above
void ScoutingTreeMakerRun3::beginJob() {
    edm::Service<TFileService> fs;
    tree = fs->make<TTree>("tree"      , "tree");

    //tree->Branch("nPFJets"             , &nPFJets                     , "nPFJets/F"     );

    tree->Branch("ptjet1"              , &ptjet1                      , "ptjet1/F"      );
    tree->Branch("ptjet2"              , &ptjet2                      , "ptjet2/F"      ); 
    tree->Branch("ptjet3"              , &ptjet3                      , "ptjet3/F"      );
    tree->Branch("ptjet4"              , &ptjet4                      , "ptjet4/F"      );    

    tree->Branch("etajet1"             , &etajet1                     , "etajet1/F"     );
    tree->Branch("etajet2"             , &etajet2                     , "etajet2/F"     );
    tree->Branch("etajet3"             , &etajet3                     , "etajet3/F"     );
    tree->Branch("etajet4"             , &etajet4                     , "etajet4/F"     );

    tree->Branch("phijet1"             , &phijet1                     , "phijet1/F"     );
    tree->Branch("phijet2"             , &phijet2                     , "phijet2/F"     );
    tree->Branch("phijet3"             , &phijet3                     , "phijet3/F"     );
    tree->Branch("phijet4"             , &phijet4                     , "phijet4/F"     );

    //Add the missing distributions
    tree->Branch("dBV_1"               , &dBV_1                       , "dBV_1/F"       );
    tree->Branch("dBV_2"               , &dBV_2                       , "dBV_2/F"       );

    tree->Branch("bs2derr_1"           , &bs2derr_1                   , "bs2derr_1/F"   );
    tree->Branch("bs2derr_2"           , &bs2derr_2                   , "bs2derr_2/F"   );
    
    tree->Branch("ntk_1"               , &ntk_1                       , "ntk_1/I"       );
    tree->Branch("ntk_2"               , &ntk_2                       , "ntk_2/I"       );

    tree->Branch("nVertices"               , &nVertices                       , "nVertices/I"       );
    
    tree->Branch("HT"                  , &HT                          , "HT/F"          );
    
    tree->Branch("l1Result", "std::vector<bool>"             ,&l1Result_, 32000, 0  );
    tree->Branch("l1_HTT280"           , &l1_HTT280                   , "l1_HTT280/O"  );
    tree->Branch("genVert_x_1"              , &genVert_x                      , "genVert_x_1/F"      );
    tree->Branch("genVert_y_1"              , &genVert_y                      , "genVert_y_1/F"      );
    tree->Branch("genVert_z_1"              , &genVert_z                      , "genVert_z_1/F"      );
    tree->Branch("genVert_dBV_1"              , &genVert_dBV                      , "genVert_dBV_1/F"      );
    tree->Branch("genVert_3d_1"              , &genVert_3d                      , "genVert_3d_1/F"      );
    tree->Branch("genVert_motherEta_1"       , &genVert_motherEta               , "genVert_motherEta_1/F"      );
    tree->Branch("genVert_motherPhi_1"       , &genVert_motherPhi               , "genVert_motherPhi_1/F"      );
    tree->Branch("genVert_motherPt_1"       , &genVert_motherPt               , "genVert_motherPt_1/F"      );
    tree->Branch("genVert_motherDistTraveled_1"       , &genVert_motherDistTraveled               , "genVert_motherDistTraveled_1/F"      );
    tree->Branch("genVert_dVV_1"              , &genVert_dVV                      , "genVert_dVV_1/F"      );
    tree->Branch("genVert_dPhi_1"              , &genVert_dPhi                      , "genVert_dPhi_1/F"      );
}

// ------------ method called once each job just after ending the event loop  ------------
void ScoutingTreeMakerRun3::endJob() {
  // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void ScoutingTreeMakerRun3::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ScoutingTreeMakerRun3);
