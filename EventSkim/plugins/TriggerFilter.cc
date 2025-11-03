// -*- C++ -*-
//
// Package:    Run3ScoutingAnalysisTools/EventSkim
// Class:      TriggerFilter
//
/**\class TriggerFilter  Run3ScoutingAnalysisTools/EventSkim/plugins/TriggerFilter.cc

   Description: [one line class summary]
                                                                
   Implementation:
   [Notes on implementation]
*/

// system include files
#include <memory>
#include <iostream>
#include <TTree.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

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
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/Scouting/interface/Run3ScoutingPFJet.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"
#include "TH1.h"
#include "TH2.h"
#include "TMath.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
//
// class declaration
//

class TriggerFilter : public edm::one::EDFilter<edm::one::SharedResources> {
   public:
      explicit TriggerFilter(const edm::ParameterSet&);
      ~TriggerFilter();
  
      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() ;
      
      virtual bool beginRun(edm::Run&, edm::EventSetup const&);
      virtual bool endRun(edm::Run&, edm::EventSetup const&);
      virtual bool beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
      virtual bool endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&);
  
      // ----------member data ---------------------------
      const edm::EDGetTokenT<std::vector<Run3ScoutingPFJet> >  pfjetsToken;
      const edm::EDGetTokenT<std::vector<pat::Jet> >  patjetsToken;
      const edm::EDGetTokenT<GenEventInfoProduct> GeneratorToken_;
      const edm::EDGetTokenT<std::vector<reco::GenJet>> GenJetToken_;
      const edm::EDGetTokenT<edm::TriggerResults> triggerResultsToken;

      double luminosity;
      double crossSection;
      edm::InputTag algInputTag_;
      edm::InputTag extInputTag_;
      edm::EDGetToken algToken_;
      std::unique_ptr<l1t::L1TGlobalUtil> l1GtUtils_;
      std::vector<std::string>     l1Seeds_;
      bool isScouting;
      bool isMC;
      bool storeGenJets;
      TH2F* jetVetoMap_;
      TH1D* h_genWeights;
      TH1D* h_weights;
      TH1D* h_weightsSquared;

      TTree* tree;
      std::vector<float>* genJet_pt;
      std::vector<float>* genJet_eta;
      std::vector<float>* genJet_phi;
      std::vector<float>* genJet_mass;
      std::vector<float>* genJet_energy;
      std::vector<int>* genJet_nConstituents;
      float genHT;
      int nGenJets;
      
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
TriggerFilter::TriggerFilter(const edm::ParameterSet& iConfig):
  pfjetsToken(consumes<std::vector<Run3ScoutingPFJet> >(iConfig.getParameter<edm::InputTag>("pfjets"))),
  patjetsToken(consumes<std::vector<pat::Jet> >(iConfig.getParameter<edm::InputTag>("patjets"))),
  GeneratorToken_(consumes(iConfig.getParameter<edm::InputTag>("generatorName"))),
  GenJetToken_(consumes(iConfig.getParameter<edm::InputTag>("genJet_src"))),
  triggerResultsToken(consumes<edm::TriggerResults>(iConfig.getParameter<edm::InputTag>("triggerresults"))),
  luminosity(iConfig.existsAs<double>("luminosity") ? iConfig.getParameter<double>  ("luminosity") : 1.0),
  crossSection(iConfig.existsAs<double>("crossSection") ? iConfig.getParameter<double>  ("crossSection") : 1.0),  
  isScouting(iConfig.existsAs<bool>("isScouting") ? iConfig.getParameter<bool>  ("isScouting") : false),
  isMC(iConfig.existsAs<bool>("isMC") ?  iConfig.getParameter<bool>  ("isMC") : false),
  storeGenJets(iConfig.existsAs<bool>("storeGenJets") ? iConfig.getParameter<bool>  ("storeGenJets") : false)
{
  usesResource("TFileService");
  algInputTag_ = iConfig.getParameter<edm::InputTag>("AlgInputTag");
  extInputTag_ = iConfig.getParameter<edm::InputTag>("l1tExtBlkInputTag");
  algToken_ = consumes<BXVector<GlobalAlgBlk>>(algInputTag_);
  l1Seeds_ = iConfig.getParameter<std::vector<std::string> >("l1Seeds");
  l1GtUtils_ = std::make_unique<l1t::L1TGlobalUtil>(iConfig, consumesCollector(), *this, algInputTag_, extInputTag_, l1t::UseEventSetupIn::Event);
  if(isScouting){
    produces<std::vector<Run3ScoutingPFJet>>("pfjets");
  }
  else{
    produces<std::vector<pat::Jet>>("patjets");
  }
}


TriggerFilter::~TriggerFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
TriggerFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  using namespace edm;
  using namespace std;

  bool passFilter = true;
  
  double genWeight;
  double theWeight;
  
  genJet_pt->clear();
  genJet_eta->clear();
  genJet_phi->clear();
  genJet_mass->clear();
  genJet_energy->clear();
  genJet_nConstituents->clear();
  
  if(isMC){
    edm::Handle<GenEventInfoProduct> generatorHandle;
    iEvent.getByToken(GeneratorToken_, generatorHandle);
    genWeight = generatorHandle->weight();
    h_genWeights->Fill("None",genWeight);
    theWeight = genWeight*luminosity*crossSection;
    h_weights->Fill("None",theWeight);
    h_weightsSquared->Fill("None",pow(theWeight,2));
  }
  else{
    genWeight = 1;
    theWeight = 1;
    
    //Just to get the number of events
    h_genWeights->Fill("None",genWeight);
    h_weights->Fill("None",theWeight);
    h_weightsSquared->Fill("None",pow(theWeight,2));
  }
  
  bool passTrigger;
  if(isScouting){
    l1GtUtils_->retrieveL1(iEvent,iSetup,algToken_);
    bool L1_HTT280er;
    bool L1_ETT2000;
    bool L1_SingleJet180;
    bool L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5;
    l1GtUtils_->getFinalDecisionByName(string("L1_HTT280er"), L1_HTT280er);
    l1GtUtils_->getFinalDecisionByName(string("L1_ETT2000"), L1_ETT2000);
    l1GtUtils_->getFinalDecisionByName(string("L1_SingleJet180"), L1_SingleJet180);
    l1GtUtils_->getFinalDecisionByName(string("L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5"), L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5);
    passTrigger = L1_HTT280er || L1_ETT2000 || L1_SingleJet180 || L1_DoubleJet30er2p5_Mass_Min250_dEta_Max1p5;
  }
  else{
    edm::Handle<edm::TriggerResults> triggerBits;
    iEvent.getByToken(triggerResultsToken, triggerBits);
    const edm::TriggerNames &names = iEvent.triggerNames(*triggerBits);
    bool HLT_FinalResult = false;
    for (unsigned int i = 0, n = triggerBits->size(); i < n; ++i) {
      std::string name = names.triggerName(i);
      if((name.find("HLT_PFHT330PT30_QuadPFJet_75_60_45_40_PNet3BTag_4p3_v")!=std::string::npos) || (name.find("HLT_PFHT1050_v")!=std::string::npos) ){
	if(triggerBits->accept(i)) HLT_FinalResult = true;
      }
    }
    passTrigger = HLT_FinalResult; 
  }

  passFilter = passFilter && passTrigger;
  if(passFilter){
    h_genWeights->Fill("Trigger",genWeight);
    h_weights->Fill("Trigger",theWeight);
    h_weightsSquared->Fill("Trigger",pow(theWeight,2));
  }

  //Get gen jets
  std::vector<reco::GenJet>::const_iterator genJetIter;
  edm::Handle<std::vector<reco::GenJet>> genJet_handle;
  if(isMC && storeGenJets){
    nGenJets = 0;
    genHT = 0;
    iEvent.getByToken(GenJetToken_,genJet_handle);
    for(genJetIter = genJet_handle->begin(); genJetIter != genJet_handle->end(); ++genJetIter){
      genJet_pt->push_back(genJetIter->pt());
      genJet_eta->push_back(genJetIter->eta());
      genJet_phi->push_back(genJetIter->phi());
      genJet_mass->push_back(genJetIter->mass());
      float energy = TMath::Sqrt(pow(TMath::CosH(genJetIter->eta())*genJetIter->pt(),2)+pow(genJetIter->mass(),2));
      genJet_energy->push_back(energy);
      genJet_nConstituents->push_back(genJetIter->nConstituents());
      nGenJets++;
      genHT += genJetIter->pt();
    }
  }
  
  int nPFJets = -1;
  //Get the jets
  Handle<vector<Run3ScoutingPFJet> > pfjetsH;
  iEvent.getByToken(pfjetsToken, pfjetsH);
  std::unique_ptr<std::vector<Run3ScoutingPFJet>> pfJetVector(new std::vector<Run3ScoutingPFJet>());

  //Require 4 PF Jets
  if(pfjetsH.isValid() && isScouting){
    for (auto jets_iter = pfjetsH->begin(); jets_iter != pfjetsH->end(); ++jets_iter) {
      float Jet_eta = jets_iter->eta();
      float Jet_pt = jets_iter->pt();
      if((Jet_pt > 30) && (abs(Jet_eta) < 2.5)){
	int binX = jetVetoMap_->GetXaxis()->FindBin(Jet_eta);
        int binY = jetVetoMap_->GetYaxis()->FindBin(jets_iter->phi());
        float maskBit = jetVetoMap_->GetBinContent(binX, binY);

	float energy = TMath::Sqrt(pow(TMath::CosH(Jet_eta)*jets_iter->pt(),2)+pow(jets_iter->m(),2));
	float Jet_chHEF = jets_iter->chargedHadronEnergy()/energy;
	float Jet_neHEF = jets_iter->neutralHadronEnergy()/energy;
	float Jet_muEF = jets_iter->muonEnergy()/energy;
	float Jet_chEmEF = jets_iter->electronEnergy()/energy;
	float Jet_neEmEF = (jets_iter->photonEnergy()+jets_iter->HFEMEnergy())/energy;
	int Jet_chMultiplicity = jets_iter->chargedHadronMultiplicity()+jets_iter->electronMultiplicity()+jets_iter->muonMultiplicity();
	int Jet_neMultiplicity = jets_iter->neutralHadronMultiplicity()+jets_iter->photonMultiplicity()+jets_iter->HFHadronMultiplicity()+jets_iter->HFEMMultiplicity();
	bool Jet_passJetIdTight = false;
	if (abs(Jet_eta) <= 2.6)
	  Jet_passJetIdTight = (Jet_neHEF < 0.99) && (Jet_neEmEF < 0.9) && (Jet_chMultiplicity+Jet_neMultiplicity > 1) && (Jet_chHEF > 0.01) && (Jet_chMultiplicity > 0);
	else if (abs(Jet_eta) > 2.6 && abs(Jet_eta) <= 2.7)
	  Jet_passJetIdTight = (Jet_neHEF < 0.90) && (Jet_neEmEF < 0.99);
	else if (abs(Jet_eta) > 2.7 && abs(Jet_eta) <= 3.0)
	  Jet_passJetIdTight = (Jet_neHEF < 0.99);
	else if (abs(Jet_eta) > 3.0)
	  Jet_passJetIdTight = (Jet_neMultiplicity >= 2) && (Jet_neEmEF < 0.4);

	bool Jet_passJetIdTightLepVeto = false;
	if (abs(Jet_eta) <= 2.7)
	  Jet_passJetIdTightLepVeto = Jet_passJetIdTight && (Jet_muEF < 0.8) && (Jet_chEmEF < 0.8);
	else
	  Jet_passJetIdTightLepVeto = Jet_passJetIdTight;
	
	if((maskBit==0) && Jet_passJetIdTightLepVeto && ((Jet_chEmEF+Jet_neEmEF)<0.9) ){
	  pfJetVector->emplace_back(*jets_iter);
	}
      }
    }
    nPFJets = pfJetVector->size();
  }

  //Get the jets
  Handle<vector<pat::Jet> > patjetsH;
  iEvent.getByToken(patjetsToken, patjetsH);
  std::unique_ptr<std::vector<pat::Jet>> patJetVector(new std::vector<pat::Jet>());

  //Require 4 Pat Jets, only for MC
  if(isMC && patjetsH.isValid() && !isScouting){
    for (auto jets_iter = patjetsH->begin(); jets_iter != patjetsH->end(); ++jets_iter) {
      float Jet_eta = jets_iter->eta();
      float Jet_pt = jets_iter->pt();
      if((Jet_pt > 30) && (abs(Jet_eta) < 2.5)){
	int binX = jetVetoMap_->GetXaxis()->FindBin(Jet_eta);
        int binY = jetVetoMap_->GetYaxis()->FindBin(jets_iter->phi());
        float maskBit = jetVetoMap_->GetBinContent(binX, binY);

	if(maskBit==0){
	  patJetVector->emplace_back(*jets_iter);
	}
      }
    }
    nPFJets = patJetVector->size();  
  }
  passFilter = passFilter && (nPFJets>2);
  if(passFilter){
    h_genWeights->Fill("nJets",genWeight);
    h_weights->Fill("nJets",theWeight);
    h_weightsSquared->Fill("nJets",pow(theWeight,2));
  }
  if(isScouting){
    iEvent.put(std::move(pfJetVector),"pfjets");
  }
  else{
    iEvent.put(std::move(patJetVector),"patjets");
  }
  tree->Fill();
  return passFilter;
}

// ------------ method called once each job just before starting event loop  ------------
void 
TriggerFilter::beginJob()
{
  edm::Service<TFileService> fs;
  h_genWeights = fs->make<TH1D>("genWeightsSkim",";Cut Applied; Sum of Gen Weights",3,0,3);
  h_genWeights->GetXaxis()->SetBinLabel(1,"None");
  h_genWeights->GetXaxis()->SetBinLabel(2,"Trigger");
  h_genWeights->GetXaxis()->SetBinLabel(3,"nJets");
  
  h_weights = fs->make<TH1D>("weightsSkim",";Cut Applied; Sum of Weights",3,0,3);
  h_weights->GetXaxis()->SetBinLabel(1,"None");
  h_weights->GetXaxis()->SetBinLabel(2,"Trigger");
  h_weights->GetXaxis()->SetBinLabel(3,"nJets");
  
  h_weightsSquared = fs->make<TH1D>("weightsSquaredSkim",";Cut Applied; Sum of Squared Weights",3,0,3);
  h_weightsSquared->GetXaxis()->SetBinLabel(1,"None");
  h_weightsSquared->GetXaxis()->SetBinLabel(2,"Trigger");
  h_weightsSquared->GetXaxis()->SetBinLabel(3,"nJets");

  TFile* vetoFile = TFile::Open("Summer24Prompt24_RunBCDEFGHI.root", "READ");
  jetVetoMap_ = (TH2F*)vetoFile->Get("jetvetomap");

  tree = fs->make<TTree>("filterTree","filterTree");
  
  genJet_pt = new std::vector<float>;
  genJet_eta = new std::vector<float>;
  genJet_phi = new std::vector<float>;
  genJet_mass = new std::vector<float>;
  genJet_energy = new std::vector<float>;
  genJet_nConstituents = new std::vector<int>;

  if(storeGenJets){
    tree->Branch("genJet_pt",&genJet_pt);
    tree->Branch("genJet_eta",&genJet_eta);
    tree->Branch("genJet_phi",&genJet_phi);
    tree->Branch("genJet_mass",&genJet_mass);
    tree->Branch("genJet_energy",&genJet_energy);
    tree->Branch("genJet_nConstituents",&genJet_nConstituents);
    tree->Branch("nGenJets", &nGenJets, "nGenJets/I");
    tree->Branch("genHT", &genHT, "genHT/F");
  }
}

// ------------ method called once each job just after ending the event loop  ------------
void 
TriggerFilter::endJob() {
  h_genWeights->GetDirectory()->cd();
  h_genWeights->Draw();
  h_genWeights->Write();

  h_weights->Draw();
  h_weights->Write();

  h_weightsSquared->Draw();
  h_weightsSquared->Write();

  tree->GetDirectory()->cd();
  
  delete genJet_pt;
  delete genJet_eta;
  delete genJet_phi;
  delete genJet_mass;
  delete genJet_energy;
  delete genJet_nConstituents;
}

// ------------ method called when starting to processes a run  ------------
bool 
TriggerFilter::beginRun(edm::Run&, edm::EventSetup const&)
{ 
  return true;
}

// ------------ method called when ending the processing of a run  ------------
bool 
TriggerFilter::endRun(edm::Run&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when starting to processes a luminosity block  ------------
bool 
TriggerFilter::beginLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method called when ending the processing of a luminosity block  ------------
bool 
TriggerFilter::endLuminosityBlock(edm::LuminosityBlock&, edm::EventSetup const&)
{
  return true;
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
TriggerFilter::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}
//define this as a plug-in
DEFINE_FWK_MODULE(TriggerFilter);
