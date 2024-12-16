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
#include "TH2.h"
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
  //const edm::EDGetTokenT<std::vector<Run3ScoutingTrack> >  tracksToken;

  const edm::EDGetTokenT<std::vector<reco::Track> >  tracksToken;
  const edm::EDGetTokenT<std::vector<reco::Vertex> >  verticesToken;

  const edm::EDGetTokenT<reco::BeamSpot> beamspot_token;
  const edm::EDGetTokenT<std::vector<reco::GenParticle>> GenParticleToken_;
  const edm::EDGetTokenT<std::vector<TrackingParticle>> TrackingParticleToken_;
  
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
  TTree* objectTree;

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
  int genScout_nMatches = -999;
  int genScoutVert_nMatches = -999;

  std::vector<float>* match_ptRatio;
  std::vector<float>* match_deltaR;
  std::vector<float>* match_diffPt;
  std::vector<float>* match_diffEta;
  std::vector<float>* match_diffPhi;

  TH1F* h_match_gen_dxy = new TH1F("match_gen_dxy",";Gen particle d_{xy} [cm]; Gen particles / 0.01 cm", 100, 0, 1);
  TH1F* h_gen_dxy = new TH1F("gen_dxy",";Gen particle d_{xy} [cm]; Gen particles / 0.01 cm", 100, 0, 1);
  TH2F* h_match_vert_x_y = new TH2F("match_vert_x_y","Vertex Position; X Position [cm] / 0.02 cm; Y Position [cm] / 0.02 cm", 100, -1, 1, 100, -1, 1);
  TH2F* h_match_genVert_x_y = new TH2F("match_genVert_x_y","Gen Vertex Position; X Position [cm] / 0.02 cm; Y Position [cm] / 0.02 cm", 100, -1, 1, 100, -1, 1);
  TH1F* h_match_genVert_dBV = new TH1F("match_genVert_dBV",";Gen Vertex d_{BV} [cm]; Gen Vertices / 0.01 cm", 100, 0, 1);
  TH1F* h_genVert_dBV = new TH1F("genVert_dBV",";Gen Vertex d_{BV} [cm]; Gen Vertices / 0.01 cm", 100, 0, 1);
  TH1F* h_genVert_phi = new TH1F("genVert_phi",";Gen Vertex #phi; Gen Vertices / 0.06284", 100, -3.142, 3.142);
  TH1F* h_genVert_z = new TH1F("genVert_z",";Gen Vertex z [cm]; Gen Vertices / 0.4 cm", 100, -20, 20);
  TH2F* h_genVert_x_y = new TH2F("genVert_x_y","Gen Vertex Position; X Position [cm] / 0.02 cm; Y Position [cm] / 0.02 cm", 100, -1, 1, 100, -1, 1);
  TH1F* h_genVert_nParticles = new TH1F("genVert_nParticles",";Gen Vertex Number of Particles; Gen Vertices / 1", 100, 0, 100);
  TH1F* h_genVert_dPhi = new TH1F("genVert_dPhi",";Gen Vertex d#phi; Occurrences / 0.03142", 100, 0, 3.142);
  TH1F* h_genVert_dVV = new TH1F("genVert_dVV",";Gen Vertex d_{VV} [cm]; Occurrences / 0.015 cm", 1000, 0, 15);
  TH1F* h_genVert_nVertices = new TH1F("genVert_nVertices",";Number of Gen Vertices; Occurrences / 1", 5, 0, 5);
  TH1F* h_resVert_x = new TH1F("resVert_x",";Gen X Position - Scout X Position [cm]; Gen Vertices / 0.02 cm", 100, -1, 1);
  TH1F* h_resVert_y = new TH1F("resVert_y",";Gen Y Position - Scout Y Position [cm]; Gen Vertices / 0.02 cm", 100, -1, 1);
  TH1F* h_resVert_z = new TH1F("resVert_z",";Gen Z Position - Scout Z Position [cm]; Gen Vertices / 0.02 cm", 100, -1, 1);
  TH1F* h_scoutVert_dBV = new TH1F("scoutVert_dBV",";Scout Vertex d_{BV} [cm]; Scout Vertices / 0.01 cm", 100, 0, 1);
  TH1F* h_scoutVert_phi = new TH1F("scoutVert_phi",";Scout Vertex #phi; Scout Vertices / 0.06284", 100, -3.142, 3.142);
  TH1F* h_scoutVert_z = new TH1F("scoutVert_z",";Scout Vertex z [cm]; Scout Vertices / 0.4 cm", 100, -20, 20);
  TH2F* h_scoutVert_x_y = new TH2F("scoutVert_x_y","Scout Vertex Position; X Position [cm] / 0.02 cm; Y Position [cm] / 0.02 cm", 100, -1, 1, 100, -1, 1);
  TH1F* h_scoutVert_nTracks = new TH1F("scoutVert_nTracks",";Scout Vertex Number of Particles; Scout Vertices / 1", 100, 0, 100);
  TH1F* h_scoutVert_dPhi = new TH1F("scoutVert_dPhi",";Scout Vertex d#phi; Occurrences / 0.03142", 100, 0, 3.142);
  TH1F* h_scoutVert_dVV = new TH1F("scoutVert_dVV",";Scout Vertex d_{VV} [cm]; Occurrences / 0.015 cm", 1000, 0, 15);
  TH1F* h_scoutVert_nVertices = new TH1F("scoutVert_nVertices",";Number of Scout Vertices; Occurrences / 1", 10, 0, 10);
  
  typedef std::set<reco::TrackRef> track_set;
  typedef std::vector<reco::TrackRef> track_vec;

  static bool CompareDeltaR(std::vector<float> a, std::vector<float> b) { return a[2] < b[2]; }
  
  std::pair<bool,GlobalPoint> isStopDecay(const reco::Candidate* part){
    if(fabs(part->pdgId())==1000006 && part->numberOfDaughters()==2){
      int daughterId = part->daughter(0)->pdgId();
      bool sameDaughters = (daughterId==part->daughter(1)->pdgId());
      if(sameDaughters && fabs(daughterId)==1){
	return std::make_pair(true, GlobalPoint(part->daughter(0)->vx(),part->daughter(0)->vy(),part->daughter(0)->vz()));
      }
    }
    return std::make_pair(false, GlobalPoint(-999,-999,-999));
  }

  std::pair<bool,GlobalPoint> isStopDecay(std::vector<reco::GenParticle>::const_iterator part){
    if(fabs(part->pdgId())==1000006 && part->numberOfDaughters()==2){
      int daughterId = part->daughter(0)->pdgId();
      bool sameDaughters = (daughterId==part->daughter(1)->pdgId());
      if(sameDaughters && fabs(daughterId)==1){
	return std::make_pair(true, GlobalPoint(part->daughter(0)->vx(),part->daughter(0)->vy(),part->daughter(0)->vz()));
      }
    }
    return std::make_pair(false, GlobalPoint(-999,-999,-999));
  }

  std::pair<bool,GlobalPoint> isChargedStopDecayProductStatusOne(std::vector<reco::GenParticle>::const_iterator part){
    if(part->status()!=1 || part->charge()==0 || part->numberOfMothers()==0) return std::make_pair(false,GlobalPoint(-999,-999,-999));
    const reco::Candidate* candidate = part->mother(0);
    std::pair<bool,GlobalPoint> isStopProduct = std::make_pair(false,GlobalPoint(-999,-999,-999));
    while(!isStopProduct.first){
      isStopProduct = isStopDecay(candidate);
      if(candidate->numberOfMothers()==0) break;
      candidate = candidate->mother(0);
    }
    return isStopProduct;
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

  void removeFlows(TH1F* h) {
    int nbins = h->GetNbinsX();
    double underflow = h->GetBinContent(0);
    double overflow = h->GetBinContent(nbins+1);
    h->AddBinContent(1,underflow);
    h->AddBinContent(nbins,overflow);
    h->SetBinContent(0,0);
    h->SetBinContent(nbins+1,0);
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
  //tracksToken              (consumes<std::vector<Run3ScoutingTrack> >            (iConfig.getParameter<edm::InputTag>("tracks"))),
  tracksToken              (consumes<std::vector<reco::Track> >            (iConfig.getParameter<edm::InputTag>("tracks"))),    
  verticesToken            (consumes<std::vector<reco::Vertex> >           (iConfig.getParameter<edm::InputTag>("displacedVertices"))),
  
  beamspot_token(consumes<reco::BeamSpot>(iConfig.getParameter<edm::InputTag>("beamspot_src"))),
  GenParticleToken_(consumes(iConfig.getParameter<edm::InputTag>("genParticle_src"))),
  TrackingParticleToken_(consumes(iConfig.getParameter<edm::InputTag>("trackingParticle_src"))),
  
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

  match_deltaR->clear();
  match_ptRatio->clear();
  match_diffPt->clear();
  match_diffEta->clear();
  match_diffPhi->clear();
  
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

  h_scoutVert_nVertices->Fill(vertices_ntk.size());
  
  //Get gen particles for truth-level vertices
  edm::Handle<std::vector<reco::GenParticle>> genParticle_handle;
  iEvent.getByToken(GenParticleToken_,genParticle_handle);
  std::vector<GlobalPoint> genVertices;
  
  std::vector<reco::GenParticle>::const_iterator genParticleIter;
  float maxDist = 0;
  for(genParticleIter = genParticle_handle->begin(); genParticleIter != genParticle_handle->end(); ++genParticleIter){
    if(genParticleIter->numberOfMothers()<1) continue;
    if(isStopDecay(genParticleIter->mother(0)).first){
      //std::cout<<"isStop block"<<std::endl;
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
      if(!isDupe){
	genVertices.push_back(genVertex);
	h_genVert_dBV->Fill(dist);
	h_genVert_phi->Fill(atan2(genVertex.y()-beamspot->y0(),genVertex.x()-beamspot->x0()));
	h_genVert_z->Fill(genVertex.z()-beamspot->z0());
	h_genVert_x_y->Fill(genVertex.x()-beamspot->x0(),genVertex.y()-beamspot->y0());
      }
      //std::cout<<"first mother id: "<<mother->pdgId()<<" status: "<<mother->status()<<" vertex: "<<mother->vx()<<" "<<mother->vy()<<" "<<mother->vz()<<" parent id: "<<mother->mother(0)->pdgId()<<" parent status: "<<mother->mother(0)->status()<<" mother vertex: "<<mother->mother(0)->vx()<<" "<<mother->mother(0)->vy()<<" "<<mother->mother(0)->vz()<<std::endl;
      if(dist>maxDist){
	maxDist = dist;
	genVert_x = genParticleIter->vx()-beamspot->x0();
	genVert_y = genParticleIter->vy()-beamspot->y0();
	genVert_z = genParticleIter->vz()-beamspot->z0();
	genVert_dBV = dist;
	genVert_3d = TMath::Sqrt(pow(genParticleIter->vx()-beamspot->x0(),2)+pow(genParticleIter->vy()-beamspot->y0(),2)+pow(genParticleIter->vz()-beamspot->z0(),2));
	genVert_motherEta = mother->eta();
	genVert_motherPhi = mother->phi();
	genVert_motherPt = mother->pt();
	genVert_motherDistTraveled = TMath::Sqrt(pow(genParticleIter->vx()-mother->mother(0)->vx(),2)+pow(genParticleIter->vy()-mother->mother(0)->vy(),2)+pow(genParticleIter->vz()-mother->mother(0)->vz(),2));
      }
    } //isStopDecay
    std::pair<bool,GlobalPoint> isChargedStopDecayProduct = isChargedStopDecayProductStatusOne(genParticleIter);
    if(isChargedStopDecayProduct.first){
      //std::cout<<"pdgid: "<<genParticleIter->pdgId()<<" vertex xyz: "<<genParticleIter->vx()<<" "<<genParticleIter->vy()<<" "<<genParticleIter->vz()<<" status: "<<genParticleIter->status()<<" parent id: "<<genParticleIter->mother(0)->pdgId()<<" parent vertex: "<<genParticleIter->mother(0)->vx()<<" "<<genParticleIter->mother(0)->vy()<<" "<<genParticleIter->mother(0)->vz()<<" parent status: "<<genParticleIter->mother(0)->status()<<std::endl;

      float dxy = TMath::Sqrt(pow(genParticleIter->vx()-beamspot->x0(),2)+pow(genParticleIter->vy()-beamspot->y0(),2));
      h_gen_dxy->Fill(dxy); 
    }
  } //loop over gen particles

  h_genVert_nVertices->Fill(genVertices.size());
  //count number of gen particles from same vertex
  std::vector<int> nMatches(genVertices.size(),0);
  for(genParticleIter = genParticle_handle->begin(); genParticleIter != genParticle_handle->end(); ++genParticleIter){
    std::pair<bool,GlobalPoint> isChargedStopDecayProduct = isChargedStopDecayProductStatusOne(genParticleIter);
    if(!isChargedStopDecayProduct.first) continue;
    uint i = -1;
    for(auto vertex: genVertices){
      i++;
      if((vertex.x()==isChargedStopDecayProduct.second.x()) && (vertex.y()==isChargedStopDecayProduct.second.y()) && (vertex.z()==isChargedStopDecayProduct.second.z())) nMatches[i]++;
    }
  }
  for(uint i=0; i<nMatches.size(); i++){
    h_genVert_nParticles->Fill(nMatches[i]);
  }

  if(genVertices.size()==2){
    genVert_dVV = TMath::Sqrt(pow(genVertices[0].x()-genVertices[1].x(),2)+pow(genVertices[0].y()-genVertices[1].y(),2)+pow(genVertices[0].z()-genVertices[1].z(),2));
    float dPhi = fabs(atan2(genVertices[0].y()-beamspot->y0(),genVertices[0].x()-beamspot->x0())-atan2(genVertices[1].y()-beamspot->y0(),genVertices[1].x()-beamspot->x0()));
    if(dPhi>TMath::Pi()) dPhi = 2*TMath::Pi() - dPhi;
    genVert_dPhi = dPhi;
  }

  if(genVertices.size()>1){
    for(uint i=0; i<(genVertices.size()-1); i++){
      for(uint j=i+1; j<genVertices.size(); j++){
	float dPhi = fabs(atan2(genVertices[i].y()-beamspot->y0(),genVertices[i].x()-beamspot->x0())-atan2(genVertices[j].y()-beamspot->y0(),genVertices[j].x()-beamspot->x0()));
	if(dPhi>TMath::Pi()) dPhi = 2*TMath::Pi() - dPhi;
	h_genVert_dPhi->Fill(dPhi);
	h_genVert_dVV->Fill(TMath::Sqrt(pow(genVertices[i].x()-genVertices[j].x(),2)+pow(genVertices[i].y()-genVertices[j].y(),2)+pow(genVertices[i].z()-genVertices[j].z(),2)));
      }
    }
  }
  
  //Get tracking particles
  edm::Handle<std::vector<TrackingParticle>> TrackingParticleHandle;
  iEvent.getByToken(TrackingParticleToken_, TrackingParticleHandle);

  //Get scouting tracks
  //edm::Handle<std::vector<Run3ScoutingTrack>> ScoutingTrackHandle;
  edm::Handle<std::vector<reco::Track>> ScoutingTrackHandle;
  iEvent.getByToken(tracksToken, ScoutingTrackHandle);
  std::vector<reco::Track>::const_iterator scoutingTrackIter;
  int i_track = -1;
  std::vector<std::vector<float>> deltaRVec;
  for(scoutingTrackIter = ScoutingTrackHandle->begin(); scoutingTrackIter != ScoutingTrackHandle->end(); ++scoutingTrackIter){
    i_track++;
    //std::cout<<"scouting track pt: "<<scoutingTrackIter->tk_pt()<<" eta: "<<scoutingTrackIter->tk_eta()<<" phi: "<<scoutingTrackIter->tk_phi()<<" vertex: "<<scoutingTrackIter->tk_vx()<<" "<<scoutingTrackIter->tk_vy()<<" "<<scoutingTrackIter->tk_vz()<<std::endl;
    int i_gen = -1;
    for(genParticleIter = genParticle_handle->begin(); genParticleIter != genParticle_handle->end(); ++genParticleIter){
      i_gen++;
      if(!isChargedStopDecayProductStatusOne(genParticleIter).first) continue;
      float dPhi = fabs(scoutingTrackIter->phi()-genParticleIter->phi());
      if(dPhi>TMath::Pi()) dPhi = 2*TMath::Pi() - dPhi;
      float dEta = fabs(scoutingTrackIter->eta()-genParticleIter->eta());
      float deltaR = TMath::Sqrt(pow(dPhi,2)+pow(dEta,2));
      float ptRatio = fabs(scoutingTrackIter->pt()-genParticleIter->pt())/(scoutingTrackIter->pt()+genParticleIter->pt());
      if(deltaR<0.05 && ptRatio<0.1){
	deltaRVec.push_back({float(i_gen),float(i_track),deltaR});
      }
    }
  }
  
  sort(deltaRVec.begin(), deltaRVec.end(), CompareDeltaR); //sort matches by deltaR smallest to largest
  std::vector<std::vector<float>> finalMatches;
  while(deltaRVec.size()>0){
    finalMatches.push_back(deltaRVec[0]);
    int i_genMatch = deltaRVec[0][0];
    int i_trackMatch = deltaRVec[0][1];
    deltaRVec.erase(std::remove_if( deltaRVec.begin(), deltaRVec.end(), [i_genMatch](std::vector<float> x){return x[0]==i_genMatch;}), deltaRVec.end());
    deltaRVec.erase(std::remove_if( deltaRVec.begin(), deltaRVec.end(), [i_trackMatch](std::vector<float> x){return x[1]==i_trackMatch;}), deltaRVec.end());
  }
  for(auto match: finalMatches){
    float pt_gen = (genParticle_handle->begin()+match[0])->pt();
    float pt_track = (ScoutingTrackHandle->begin()+match[1])->pt();
    float eta_gen = (genParticle_handle->begin()+match[0])->eta();
    float eta_track = (ScoutingTrackHandle->begin()+match[1])->eta();
    float phi_gen = (genParticle_handle->begin()+match[0])->phi();
    float phi_track = (ScoutingTrackHandle->begin()+match[1])->phi();
    match_ptRatio->push_back(fabs(pt_track-pt_gen)/(pt_track+pt_gen));
    match_deltaR->push_back(match[2]);
    match_diffPt->push_back(fabs(pt_gen-pt_track));
    match_diffEta->push_back(fabs(eta_gen-eta_track));
    float diffPhi = fabs(phi_gen-phi_track);
    if(diffPhi>TMath::Pi()) diffPhi = 2*TMath::Pi() - diffPhi;
    match_diffPhi->push_back(diffPhi);
    
    float dxy = TMath::Sqrt(pow((genParticle_handle->begin()+match[0])->vx()-beamspot->x0(),2)+pow((genParticle_handle->begin()+match[0])->vy()-beamspot->y0(),2));
    h_match_gen_dxy->Fill(dxy);
  }
  genScout_nMatches = finalMatches.size();

  //Two leading vertices
  vector<double> dBVs;
  VertexDistanceXY vertex_dist_2d;
  t=0;
  float dBV_t;

  std::vector<std::vector<float>> deltaRVecVertices;
  for (auto vertex_iter = vertices_ntk.begin(); vertex_iter != vertices_ntk.end(); ++vertex_iter) {
    v = vertices_ntk.at(t);
    dBV_t = vertex_dist_2d.distance(v, fake_bs_vtx).value();
    dBVs.push_back(dBV_t);

    std::vector<TrackRef> trks = vertex_track_vec(v);
    //std::cout<<"vertex number of tracks: "<<trks.size()<<std::endl;

    h_scoutVert_dBV->Fill(dBV_t);
    h_scoutVert_phi->Fill(atan2(v.y()-beamspot->y0(),v.x()-beamspot->x0()));
    h_scoutVert_z->Fill(v.z()-beamspot->z0());
    h_scoutVert_x_y->Fill(v.x()-beamspot->x0(),v.y()-beamspot->y0());
    h_scoutVert_nTracks->Fill(trks.size());
    
    int i_trk = -1;
    for(auto trk: trks){
      i_trk++;
      int i_gen = -1;
      for(genParticleIter = genParticle_handle->begin(); genParticleIter != genParticle_handle->end(); ++genParticleIter){
	i_gen++;
	std::pair<bool,GlobalPoint> isDecayProduct = isChargedStopDecayProductStatusOne(genParticleIter);
	if(!isDecayProduct.first) continue;
	float dPhi = fabs(trk->phi()-genParticleIter->phi());
	if(dPhi>TMath::Pi()) dPhi = 2*TMath::Pi() - dPhi;
	float dEta = fabs(trk->eta()-genParticleIter->eta());
	float deltaR = TMath::Sqrt(pow(dPhi,2)+pow(dEta,2));
	float ptRatio = fabs(trk->pt()-genParticleIter->pt())/(trk->pt()+genParticleIter->pt());
	if(deltaR<0.05 && ptRatio<0.1){
	  deltaRVecVertices.push_back({float(i_gen),float(i_trk),deltaR,float(t),isDecayProduct.second.x(),isDecayProduct.second.y(),isDecayProduct.second.z()});
	}
      } // loop over gen particles
    } // loop over vertex tracks
    t++;
  } //loop over vertices
  
  sort(deltaRVecVertices.begin(), deltaRVecVertices.end(), CompareDeltaR); //sort matches by deltaR smallest to largest
  std::vector<std::vector<float>> finalMatchesVertices;
  while(deltaRVecVertices.size()>0){
    finalMatchesVertices.push_back(deltaRVecVertices[0]);
    int i_genMatch = deltaRVecVertices[0][0];
    int i_trackMatch = deltaRVecVertices[0][1];
    int i_vtx = deltaRVecVertices[0][3];
    deltaRVecVertices.erase(std::remove_if( deltaRVecVertices.begin(), deltaRVecVertices.end(), [i_genMatch](std::vector<float> x){return x[0]==i_genMatch;}), deltaRVecVertices.end());
    deltaRVecVertices.erase(std::remove_if( deltaRVecVertices.begin(), deltaRVecVertices.end(), [i_trackMatch,i_vtx](std::vector<float> x){return ((x[1]==i_trackMatch)&&(x[3]==i_vtx));}), deltaRVecVertices.end());
  }
  std::vector<int> vectorNumMatches(t,0);
  for(auto match: finalMatchesVertices){
    vectorNumMatches[match[3]]++;
    if(vectorNumMatches[match[3]]==2){
      Vertex vert = vertices_ntk.at(match[3]);
      h_match_vert_x_y->Fill(vert.x()-beamspot->x0(),vert.y()-beamspot->y0());
    }
  }
  int nVertMatches = 0;
  for(auto numMatches: vectorNumMatches){
    if(numMatches>1) nVertMatches++;
  }
  genScoutVert_nMatches = nVertMatches;

  //gen vertex matching
  for(auto vertex: genVertices){
    std::vector<int> numMatches(t,0);
    for(auto match: finalMatchesVertices){
      if((vertex.x()==match[4]) && (vertex.y()==match[5]) && (vertex.z()==match[6])){
	numMatches[match[3]]++;
      }
    }
    //std::cout<<"gen vertex num matches: ";
    for(uint i=0; i<numMatches.size(); i++){
      //std::cout<<numMatches[i]<<" ";
    }
    //std::cout<<std::endl;
    bool isMatched = false;
    uint i_match=0;
    int maxMatches = 1;
    for(uint i=0; i<numMatches.size(); i++){
      if(numMatches[i]>maxMatches){
	isMatched = true;
	maxMatches = numMatches[i];
	i_match = i;
      }
    }
    if(isMatched){
      h_match_genVert_x_y->Fill(vertex.x()-beamspot->x0(),vertex.y()-beamspot->y0());
      h_match_genVert_dBV->Fill(TMath::Sqrt(pow(vertex.x()-beamspot->x0(),2)+pow(vertex.y()-beamspot->y0(),2)));
      Vertex scoutVert = vertices_ntk.at(i_match);
      h_resVert_x->Fill(vertex.x()-scoutVert.x());
      h_resVert_y->Fill(vertex.y()-scoutVert.y());
      h_resVert_z->Fill(vertex.z()-scoutVert.z());
    }
  }

  if(vertices_ntk.size()>1){
    for (uint i=0; i<(vertices_ntk.size()-1); i++){
      for(uint j=i+1; j<vertices_ntk.size(); j++){
	float dPhi = fabs(atan2(vertices_ntk[i].y()-beamspot->y0(),vertices_ntk[i].x()-beamspot->x0())-atan2(vertices_ntk[j].y()-beamspot->y0(),vertices_ntk[j].x()-beamspot->x0()));
	if(dPhi>TMath::Pi()) dPhi = 2*TMath::Pi() - dPhi;
	h_scoutVert_dPhi->Fill(dPhi);
	h_scoutVert_dVV->Fill(TMath::Sqrt(pow(vertices_ntk[i].x()-vertices_ntk[j].x(),2)+pow(vertices_ntk[i].y()-vertices_ntk[j].y(),2)+pow(vertices_ntk[i].z()-vertices_ntk[j].z(),2)));
      }
    }
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
  objectTree->Fill();
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
    tree->Branch("genVert_dVV"              , &genVert_dVV                      , "genVert_dVV/F"      );
    tree->Branch("genVert_dPhi"              , &genVert_dPhi                      , "genVert_dPhi/F"      );
    tree->Branch("genScout_nMatches"              , &genScout_nMatches                      , "genScout_nMatches/I"      );
    tree->Branch("genScoutVert_nMatches"              , &genScoutVert_nMatches                      , "genScoutVert_nMatches/I"      );

    match_ptRatio = new std::vector<float>;
    match_deltaR = new std::vector<float>;
    match_diffPt = new std::vector<float>;
    match_diffEta = new std::vector<float>;
    match_diffPhi = new std::vector<float>;
    
    objectTree = fs->make<TTree>("objectTree","objectTree");
    objectTree->Branch("match_ptRatio",&match_ptRatio);
    objectTree->Branch("match_deltaR",&match_deltaR);
    objectTree->Branch("match_diffPt",&match_diffPt);
    objectTree->Branch("match_diffEta",&match_diffEta);
    objectTree->Branch("match_diffPhi",&match_diffPhi);
}

// ------------ method called once each job just after ending the event loop  ------------
void ScoutingTreeMakerRun3::endJob() {
  // please remove this method if not needed
  removeFlows(h_match_gen_dxy);
  removeFlows(h_gen_dxy);
  h_match_gen_dxy->Sumw2();
  h_gen_dxy->Sumw2();
  TH1F* h_matchEff_dxy = (TH1F*)h_match_gen_dxy->Clone();
  h_matchEff_dxy->SetName("matchEff_dxy");
  h_matchEff_dxy->GetYaxis()->SetTitle("Match Efficiency");
  h_matchEff_dxy->Divide(h_match_gen_dxy, h_gen_dxy, 1.0, 1.0, "B");
  h_matchEff_dxy->SetAxisRange(0, 1.1, "Y");
  h_matchEff_dxy->Draw();
  h_matchEff_dxy->Write();

  removeFlows(h_match_genVert_dBV);
  removeFlows(h_genVert_dBV);
  h_match_genVert_dBV->Sumw2();
  h_genVert_dBV->Sumw2();
  TH1F* h_vertMatchEff_dBV = (TH1F*)h_match_genVert_dBV->Clone();
  h_vertMatchEff_dBV->SetName("vertMatchEff_dBV");
  h_vertMatchEff_dBV->GetYaxis()->SetTitle("Match Efficiency");
  h_vertMatchEff_dBV->Divide(h_match_genVert_dBV, h_genVert_dBV, 1.0, 1.0, "B");
  h_vertMatchEff_dBV->SetAxisRange(0, 1.1, "Y");
  h_vertMatchEff_dBV->Draw();
  h_vertMatchEff_dBV->Write();
  
  h_match_vert_x_y->Draw();
  h_match_vert_x_y->Write();

  h_match_genVert_x_y->Draw();
  h_match_genVert_x_y->Write();

  h_match_genVert_dBV->Draw();
  h_match_genVert_dBV->Write();

  removeFlows(h_genVert_phi);
  h_genVert_phi->Draw();
  h_genVert_phi->Write();

  removeFlows(h_genVert_z);
  h_genVert_z->Draw();
  h_genVert_z->Write();

  removeFlows(h_genVert_nParticles);
  h_genVert_nParticles->Draw();
  h_genVert_nParticles->Write();

  removeFlows(h_genVert_dPhi);
  h_genVert_dPhi->Draw();
  h_genVert_dPhi->Write();

  removeFlows(h_genVert_dVV);
  h_genVert_dVV->Draw();
  h_genVert_dVV->Write();

  removeFlows(h_genVert_nVertices);
  h_genVert_nVertices->Draw();
  h_genVert_nVertices->Write();
  
  h_genVert_x_y->Draw();
  h_genVert_x_y->Write();
  
  removeFlows(h_resVert_x);
  h_resVert_x->Draw();
  h_resVert_x->Write();

  removeFlows(h_resVert_y);
  h_resVert_y->Draw();
  h_resVert_y->Write();

  removeFlows(h_resVert_z);
  h_resVert_z->Draw();
  h_resVert_z->Write();

  removeFlows(h_scoutVert_dBV);
  h_scoutVert_dBV->Draw();
  h_scoutVert_dBV->Write();

  removeFlows(h_scoutVert_phi);
  h_scoutVert_phi->Draw();
  h_scoutVert_phi->Write();

  removeFlows(h_scoutVert_z);
  h_scoutVert_z->Draw();
  h_scoutVert_z->Write();

  h_scoutVert_x_y->Draw();
  h_scoutVert_x_y->Write();

  removeFlows(h_scoutVert_nTracks);
  h_scoutVert_nTracks->Draw();
  h_scoutVert_nTracks->Write();

  removeFlows(h_scoutVert_dPhi);
  h_scoutVert_dPhi->Draw();
  h_scoutVert_dPhi->Write();

  removeFlows(h_scoutVert_dVV);
  h_scoutVert_dVV->Draw();
  h_scoutVert_dVV->Write();

  removeFlows(h_scoutVert_nVertices);
  h_scoutVert_nVertices->Draw();
  h_scoutVert_nVertices->Write();
  
  delete match_ptRatio;
  delete match_deltaR;
  delete match_diffPt;
  delete match_diffEta;
  delete match_diffPhi;
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
