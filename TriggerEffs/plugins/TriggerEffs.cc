// -*- C++ -*-
//
// Package:    Run3ScoutingAnalysisTools/TriggerEffs
// Class:      TriggerEffs
//
/**\class TriggerEffs TriggerEffs.cc Run3ScoutingAnalysisTools/TriggerEffs/plugins/TriggerEffs.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Bruno Lopes
//         Created:  Tue, 09 Sep 2025 16:54:35 GMT
//
//

// system include files
#include <memory>
#include <TTree.h>
#include <TLorentzVector.h>
#include <algorithm>
#include "TMath.h"
#include <valarray>

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
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "DataFormats/Scouting/interface/Run3ScoutingElectron.h"
#include "DataFormats/Scouting/interface/Run3ScoutingPhoton.h"
#include "DataFormats/Scouting/interface/Run3ScoutingPFJet.h"
#include "DataFormats/Scouting/interface/Run3ScoutingVertex.h"
#include "DataFormats/Scouting/interface/Run3ScoutingTrack.h"
#include "DataFormats/Scouting/interface/Run3ScoutingMuon.h"
#include "DataFormats/Scouting/interface/Run3ScoutingParticle.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "RecoVertex/VertexTools/interface/VertexDistance3D.h"
#include "RecoVertex/VertexTools/interface/VertexDistanceXY.h"
#include "TrackingTools/IPTools/interface/IPTools.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"

#include "DataFormats/Math/interface/deltaPhi.h"

#include "DataFormats/L1TGlobal/interface/GlobalExtBlk.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
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

class TriggerEffs : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit TriggerEffs(const edm::ParameterSet&);
  ~TriggerEffs() override;

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  // ----------member data ---------------------------
  const edm::EDGetTokenT<std::vector<Run3ScoutingPFJet> >  pfjetsToken;
  const edm::EDGetTokenT<GenEventInfoProduct> GeneratorToken_;
  const edm::EDGetTokenT<edm::TriggerResults> triggerResultsToken;

  std::vector<std::string> triggerPathsVector;
  std::map<std::string, int> triggerPathsMap;

  float genWeight;
  float theWeight;
  float luminosity;
  float crossSection;

  triggerExpression::Data triggerCache_;

  edm::InputTag                algInputTag_;
  edm::InputTag                extInputTag_;
  edm::EDGetToken              algToken_;
  std::unique_ptr<l1t::L1TGlobalUtil> l1GtUtils_;
  std::vector<std::string>     l1Seeds_;

  
  bool isMC;
  TTree* objectTree;

  int nPFJets;

  float ht;
  float ht_raw;

  bool L1_HTT280er;
  bool L1_ETT2000;
  bool L1_SingleJet180;
  bool L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5;

  const edm::EDGetTokenT<std::vector<PileupSummaryInfo>> truePileupToken;

  const edm::EDGetTokenT<std::vector<Run3ScoutingMuon>>      muonsToken;
  const edm::EDGetTokenT<std::vector<Run3ScoutingParticle>> scoutingParticle_collection_token_;
  double muon_pt;
  double muon_eta;
  double muon_chi2;
  int muon_trackLayers;
  int muon_pixelHits;
  int muon_muonHits;
  int muon_matchedStation;

  double matchTolerance;
  double muon_iso;
  double muon_iso_max;

  int observedPU;
  int truePU;

  int nMuons;
  int nSelectedMuons;

  int nPFMuons;

  double mu1s_pt;
  double mu1s_eta;
  double mu1s_chi2;
  int mu1s_trackLayers;
  int mu1s_pixelHits;
  int mu1s_muonHits;
  int mu1s_matchedStation;

  double mu1_pt;
  double mu1_eta;
  double mu1_chi2;
  int mu1_trackLayers;
  int mu1_pixelHits;
  int mu1_muonHits;
  int mu1_matchedStation;

  double mu2_pt;
  double mu2_eta;
  double mu2_chi2;
  int mu2_trackLayers;
  int mu2_pixelHits;
  int mu2_muonHits;
  int mu2_matchedStation;

  bool passHTTrigger;
  bool passMuonTrigger;

  bool muonIso;
  bool isPFMuon;

  std::vector<double> muonsIsoCustom;
  std::vector<double> muonsIsoDefault;
  std::vector<int> isPFMuons;

  bool offlineMuon;
  bool finalMuon;

  double MuonTrackIso(Run3ScoutingMuon const& muonTrack, edm::Handle<std::vector<Run3ScoutingParticle>> const& scoutingParticleH){

    double mu_pt = muonTrack.pt();
    double mu_eta = muonTrack.eta();
    double mu_phi = muonTrack.phi();

    double pf_pt = 0.0;
    double pf_eta;
    double pf_phi;

    double dEta;
    double dPhi;
    double dR;

    for (size_t pf_index = 0; pf_index < scoutingParticleH->size(); pf_index++) {
      auto & scoutingPFCandidate = scoutingParticleH->at(pf_index);

      pf_eta = scoutingPFCandidate.eta();
      pf_phi = scoutingPFCandidate.phi();
      
      dEta = mu_eta - pf_eta;
      dPhi = deltaPhi(mu_phi, pf_phi);
      dR = sqrt(pow(dEta, 2) + pow(dPhi, 2));

      if(abs(scoutingPFCandidate.pdgId()) != 13){
        if(dR < 0.3){
          pf_pt = pf_pt + scoutingPFCandidate.pt();
        }
      }
    }

    double iso = pf_pt / mu_pt;

    return iso;
  }

bool matchesPF(int ID, double tolerance, Run3ScoutingMuon const& muonTrack,  edm::Handle<std::vector<Run3ScoutingParticle>> const& scoutingParticleH){

  bool matches = false;

  float mu_eta = muonTrack.eta();
  float mu_phi = muonTrack.phi();

  double dEta;
  double dPhi;
  double dR;

  for (size_t pf_index = 0; pf_index < scoutingParticleH->size(); pf_index++) {
    auto & scoutingPFCandidate = scoutingParticleH->at(pf_index);

    float pf_eta = scoutingPFCandidate.eta();
    float pf_phi = scoutingPFCandidate.phi();

    dEta = mu_eta - pf_eta;
    dPhi = deltaPhi(mu_phi, pf_phi);
    dR = sqrt(pow(dEta, 2) + pow(dPhi, 2));

    if(abs(scoutingPFCandidate.pdgId()) == 13 && dR < tolerance){
          matches = true;
      }
  }
  return matches;
}

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TriggerEffs::TriggerEffs(const edm::ParameterSet& iConfig):
    pfjetsToken(consumes<std::vector<Run3ScoutingPFJet> >(iConfig.getParameter<edm::InputTag>("pfjets"))),
    GeneratorToken_(consumes(iConfig.getParameter<edm::InputTag>("generatorName"))),
    triggerResultsToken(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerresults"))),
    luminosity(iConfig.existsAs<double>("luminosity") ? iConfig.getParameter<double>  ("luminosity") : 1.0),
    crossSection(iConfig.existsAs<double>("crossSection") ? iConfig.getParameter<double>  ("crossSection") : 1.0),
    algInputTag_(iConfig.getParameter<edm::InputTag>("AlgInputTag")),
    extInputTag_(iConfig.getParameter<edm::InputTag>("l1tExtBlkInputTag")), 
    algToken_(consumes<BXVector<GlobalAlgBlk>>(iConfig.getParameter<edm::InputTag>("AlgInputTag"))),
    l1Seeds_(iConfig.getParameter<std::vector<std::string>>("l1Seeds")),
    isMC(iConfig.existsAs<bool>("isMC") ?  iConfig.getParameter<bool>  ("isMC") : false),
    truePileupToken(consumes<std::vector<PileupSummaryInfo>>(iConfig.getParameter<edm::InputTag>("truePileup"))),
    muonsToken(consumes<std::vector<Run3ScoutingMuon>>(iConfig.getParameter<edm::InputTag>("scoutingMuon"))),
    scoutingParticle_collection_token_(consumes(iConfig.getParameter<edm::InputTag>("scoutingParticle"))),
    muon_pt(iConfig.getParameter<double>("muon_pt")),
    muon_eta(iConfig.getParameter<double>("muon_eta")),
    muon_chi2(iConfig.getParameter<double>("muon_chi2")),
    muon_trackLayers(iConfig.getParameter<int>("muon_trackLayers")),
    muon_pixelHits(iConfig.getParameter<int>("muon_pixelHits")),
    muon_muonHits(iConfig.getParameter<int>("muon_muonHits")),
    muon_matchedStation(iConfig.getParameter<int>("muon_matchedStation")),
    matchTolerance(iConfig.getParameter<double>("matchingTolerance")),
    muon_iso_max(iConfig.getParameter<double>("muon_iso_max"))
    {
  //now do what ever initialization is needed
    usesResource("TFileService");

    algInputTag_ = iConfig.getParameter<edm::InputTag>("AlgInputTag");
    extInputTag_ = iConfig.getParameter<edm::InputTag>("l1tExtBlkInputTag");
    algToken_ = consumes<BXVector<GlobalAlgBlk>>(algInputTag_);
    l1Seeds_ = iConfig.getParameter<std::vector<std::string> >("l1Seeds");
    l1GtUtils_ = std::make_unique<l1t::L1TGlobalUtil>(iConfig, consumesCollector(), *this, algInputTag_, extInputTag_, l1t::UseEventSetupIn::Event);
    }

TriggerEffs::~TriggerEffs() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called for each event  ------------
void TriggerEffs::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;
  using namespace std;

  //Get gen weights if MC


  if(isMC){
    edm::Handle<GenEventInfoProduct> generatorHandle;
    iEvent.getByToken(GeneratorToken_, generatorHandle);
    genWeight = generatorHandle->weight();
    theWeight = genWeight*luminosity*crossSection;
  }
  else{
    genWeight = 1;
    theWeight = 1;
  }

    //Pileup info -- only for MC

  observedPU = -1;
  truePU = -1;

  if(isMC){
    edm::Handle<std::vector<PileupSummaryInfo>> pileup;
    iEvent.getByToken(truePileupToken, pileup);

    std::vector<PileupSummaryInfo>::const_iterator pileupIter;
    for(pileupIter = pileup->begin(); pileupIter != pileup->end(); ++pileupIter){
      if (pileupIter->getBunchCrossing() == 0) {
	observedPU = pileupIter->getPU_NumInteractions();
	truePU = pileupIter->getTrueNumInteractions();
      }
    }
    
  }
    


  //Get jet info and calculate H_T
  nPFJets = -1;                                                                                                                
  Handle<vector<Run3ScoutingPFJet> > pfjetsH;
  iEvent.getByToken(pfjetsToken, pfjetsH);
  std::vector<Run3ScoutingPFJet> pfJetVector;

  //Get HT from the jets
  //ht_raw is with no jet selection, ht requires pt > 20 GeV, abs(eta) < 2.4
  ht_raw = 0;
  ht = 0;

  if(pfjetsH.isValid()){
    for (auto jets_iter = pfjetsH->begin(); jets_iter != pfjetsH->end(); ++jets_iter) {
      ht_raw = ht_raw + jets_iter->pt();
      if(jets_iter->pt() > 30 && abs(jets_iter->eta()) < 2.4){ //same requirements as L1_HTTer
        pfJetVector.push_back(*jets_iter);
        ht = ht + jets_iter->pt();
      }
    }
    nPFJets = pfJetVector.size();
  }

  //Get the trigger bits for both HT L1 seeds and SingleMuon (DST_PFScouting_SingleMuon_v)
  l1GtUtils_->retrieveL1(iEvent,iSetup,algToken_);

  l1GtUtils_->getFinalDecisionByName(string("L1_HTT280er"), L1_HTT280er);
  l1GtUtils_->getFinalDecisionByName(string("L1_ETT2000"), L1_ETT2000);
  l1GtUtils_->getFinalDecisionByName(string("L1_SingleJet180"), L1_SingleJet180);
  l1GtUtils_->getFinalDecisionByName(string("L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5"), L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5);
  passHTTrigger = L1_HTT280er || L1_ETT2000 || L1_SingleJet180 || L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5;


  Handle<edm::TriggerResults> triggerBits;
  iEvent.getByToken(triggerResultsToken, triggerBits);
  const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
  bool HLT_FinalResult = false;
  for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
    std::string name = names.triggerName(i);
    if((name.find("DST_PFScouting_SingleMuon_v")!=std::string::npos) ){
       if(triggerBits->accept(i)) HLT_FinalResult = true;
    }
  }
  passMuonTrigger = HLT_FinalResult;

  //Offline muon selection -- tight id, following Run 2 AN (until there are recommendations for 2024)
  Handle<vector<Run3ScoutingMuon> > muonsH;
  iEvent.getByToken(muonsToken, muonsH);
  // Temporary container for selected muons
  std::vector<const Run3ScoutingMuon*> orderedMuons;
  std::vector<const Run3ScoutingMuon*> selectedMuons;

  //Get scouting particle flow candidates
  edm::Handle<std::vector<Run3ScoutingParticle>> scoutingParticleH;
  iEvent.getByToken(scoutingParticle_collection_token_, scoutingParticleH);

  nPFMuons = 0;
  for (auto pfcands_iter = scoutingParticleH->begin(); pfcands_iter != scoutingParticleH->end(); ++pfcands_iter) {
    if (abs(pfcands_iter->pdgId()) == 13) nPFMuons++;
  }

  muonsIsoCustom.clear();
  muonsIsoDefault.clear();
  isPFMuons.clear();
  nMuons = 0;
  nSelectedMuons = 0;
  offlineMuon = false;

  for (auto muons_iter = muonsH->begin(); muons_iter != muonsH->end(); ++muons_iter) {
    nMuons++;
    orderedMuons.push_back(&(*muons_iter));

    muon_iso = MuonTrackIso(*muons_iter, scoutingParticleH);
    isPFMuon = matchesPF(13, matchTolerance, *muons_iter, scoutingParticleH);

    muonsIsoCustom.push_back(muon_iso);
    muonsIsoDefault.push_back(muons_iter->trackIso());
    isPFMuons.push_back(isPFMuon ? 1 : 0);

    if (muons_iter->pt() > muon_pt &&
        abs(muons_iter->eta()) < muon_eta &&
        muons_iter->normalizedChi2() < muon_chi2 &&
        muons_iter->nTrackerLayersWithMeasurement() > muon_trackLayers &&
        muons_iter->nValidPixelHits() > muon_pixelHits &&
        muons_iter->nValidRecoMuonHits() > muon_muonHits &&
        muons_iter->nRecoMuonMatchedStations() > muon_matchedStation &&
        muons_iter->trackIso() < muon_iso_max &&
        isPFMuon == true) 
    {
      offlineMuon = true;
      selectedMuons.push_back(&(*muons_iter));
      nSelectedMuons++;
    }
  }

  // Sort by pT
  if(orderedMuons.size() > 1){
    std::sort(orderedMuons.begin(), orderedMuons.end(),
              [](const Run3ScoutingMuon* a, const Run3ScoutingMuon* b){ return a->pt() > b->pt(); });
  }

    // Sort by pT
  if(selectedMuons.size() > 1){
    std::sort(selectedMuons.begin(), selectedMuons.end(),
              [](const Run3ScoutingMuon* a, const Run3ScoutingMuon* b){ return a->pt() > b->pt(); });
  }


  mu1s_pt = -100;
  mu1s_eta = -100;
  mu1s_chi2 = -100;
  mu1s_trackLayers = -100;
  mu1s_pixelHits = -100;
  mu1s_muonHits = -100;
  mu1s_matchedStation = -100;

  // Fill leading selected muon info if it exists
  if (selectedMuons.size() > 0) {
    const Run3ScoutingMuon* mu = selectedMuons[0];
    mu1s_pt = mu->pt();
    mu1s_eta = mu->eta();
    mu1s_chi2 = mu->normalizedChi2();
    mu1s_trackLayers = mu->nTrackerLayersWithMeasurement();
    mu1s_pixelHits = mu->nValidPixelHits();
    mu1s_muonHits = mu->nValidRecoMuonHits();
    mu1s_matchedStation = mu->nRecoMuonMatchedStations();
  }


  mu1_pt = -100;
  mu1_eta = -100;
  mu1_chi2 = -100;
  mu1_trackLayers = -100;
  mu1_pixelHits = -100;
  mu1_muonHits = -100;
  mu1_matchedStation = -100;

  // Fill leading muon info if it exists
  if (orderedMuons.size() > 0) {
    const Run3ScoutingMuon* mu = orderedMuons[0];
    mu1_pt = mu->pt();
    mu1_eta = mu->eta();
    mu1_chi2 = mu->normalizedChi2();
    mu1_trackLayers = mu->nTrackerLayersWithMeasurement();
    mu1_pixelHits = mu->nValidPixelHits();
    mu1_muonHits = mu->nValidRecoMuonHits();
    mu1_matchedStation = mu->nRecoMuonMatchedStations();
  }

  mu2_pt = -100;
  mu2_eta = -100;
  mu2_chi2 = -100;
  mu2_trackLayers = -100;
  mu2_pixelHits = -100;
  mu2_muonHits = -100;
  mu2_matchedStation = -100;

  // Fill subleading muon info if it exists
  if (orderedMuons.size() > 1) {
    const Run3ScoutingMuon* mu = orderedMuons[1];
    mu2_pt = mu->pt();
    mu2_eta = mu->eta();
    mu2_chi2 = mu->normalizedChi2();
    mu2_trackLayers = mu->nTrackerLayersWithMeasurement();
    mu2_pixelHits = mu->nValidPixelHits();
    mu2_muonHits = mu->nValidRecoMuonHits();
    mu2_matchedStation = mu->nRecoMuonMatchedStations();
  }
  finalMuon = passMuonTrigger && offlineMuon;

  //std::cout<<"passMuonTrigger: "<< passMuonTrigger <<std::endl;
  //std::cout<<"offlineMuon: "<< offlineMuon <<std::endl;
  //std::cout<<"finalMuon: "<< finalMuon <<std::endl;

  objectTree->Fill();

}
// ------------ method called once each job just before starting event loop  ------------
void TriggerEffs::beginJob() {
  // please remove this method if not needed
  edm::Service<TFileService> fs;
  objectTree = fs->make<TTree>("objectTree","objectTree");
  
  objectTree->Branch("genWeight",&genWeight, "genWeight/F" );
  objectTree->Branch("theWeight",&theWeight, "theWeight/F" );
  
  objectTree->Branch("ht",&ht, "ht/F" );
  objectTree->Branch("ht_raw",&ht_raw, "ht_raw/F" );

  objectTree->Branch("L1_HTT280er",&L1_HTT280er, "L1_HTT280er/O" );
  objectTree->Branch("L1_ETT2000",&L1_ETT2000, "L1_ETT2000/O" );
  objectTree->Branch("L1_SingleJet180",&L1_SingleJet180, "L1_SingleJet180/O" );
  objectTree->Branch("L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5",&L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5, "L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5/O" );

  objectTree->Branch("passHTTrigger",&passHTTrigger, "passHTTrigger/O" );
  objectTree->Branch("passMuonTrigger",&passMuonTrigger, "passMuonTrigger/O" );
  objectTree->Branch("offlineMuon",&offlineMuon, "offlineMuon/O" );
  objectTree->Branch("finalMuon",&finalMuon, "finalMuon/O" );

  objectTree->Branch("observedPU", &observedPU, "observedPU/I");
  objectTree->Branch("truePU", &truePU, "truePU/I");

  objectTree->Branch("nMuons",&nMuons, "nMuons/I" );
  objectTree->Branch("nSelectedMuons",&nSelectedMuons, "nSelectedMuons/I" );

  objectTree->Branch("mu1s_pt",&mu1s_pt, "mu1s_pt/D" );
  objectTree->Branch("mu1s_eta",&mu1s_eta, "mu1s_eta/D" );
  objectTree->Branch("mu1s_chi2",&mu1s_chi2, "mu1s_chi2/D" );
  objectTree->Branch("mu1s_trackLayers",&mu1s_trackLayers, "mu1s_trackLayers/I" );
  objectTree->Branch("mu1s_pixelHits",&mu1s_pixelHits, "mu1s_pixelHits/I" );
  objectTree->Branch("mu1s_muonHits",&mu1s_muonHits, "mu1s_muonHits/I" );
  objectTree->Branch("mu1s_matchedStation",&mu1s_matchedStation, "mu1s_matchedStation/I" );

  objectTree->Branch("mu1_pt",&mu1_pt, "mu1_pt/D" );
  objectTree->Branch("mu1_eta",&mu1_eta, "mu1_eta/D" );
  objectTree->Branch("mu1_chi2",&mu1_chi2, "mu1_chi2/D" );
  objectTree->Branch("mu1_trackLayers",&mu1_trackLayers, "mu1_trackLayers/I" );
  objectTree->Branch("mu1_pixelHits",&mu1_pixelHits, "mu1_pixelHits/I" );
  objectTree->Branch("mu1_muonHits",&mu1_muonHits, "mu1_muonHits/I" );
  objectTree->Branch("mu1_matchedStation",&mu1_matchedStation, "mu1_matchedStation/I" );

  objectTree->Branch("mu2_pt",&mu2_pt, "mu2_pt/D" );
  objectTree->Branch("mu2_eta",&mu2_eta, "mu2_eta/D" );
  objectTree->Branch("mu2_chi2",&mu2_chi2, "mu2_chi2/D" );
  objectTree->Branch("mu2_trackLayers",&mu2_trackLayers, "mu2_trackLayers/I" );
  objectTree->Branch("mu2_pixelHits",&mu2_pixelHits, "mu2_pixelHits/I" );
  objectTree->Branch("mu2_muonHits",&mu2_muonHits, "mu2_muonHits/I" );
  objectTree->Branch("mu2_matchedStation",&mu2_matchedStation, "mu2_matchedStation/I" );

  objectTree->Branch("nPFMuons",&nPFMuons, "nPFMuons/I" );
  objectTree->Branch("muonsIsoCustom", &muonsIsoCustom);
  objectTree->Branch("muonsIsoDefault", &muonsIsoDefault);
  objectTree->Branch("isPFMuons", &isPFMuons);

}

// ------------ method called once each job just after ending the event loop  ------------
void TriggerEffs::endJob() {
  // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void TriggerEffs::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //edm::ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks", edm::InputTag("ctfWithMaterialTracks"));
  //descriptions.addWithDefaultLabel(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(TriggerEffs);
