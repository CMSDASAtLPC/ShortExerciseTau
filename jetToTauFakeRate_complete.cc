#include "TreeReader.h"
#include <string>
#include <ostream>

//////////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh jetToTauFakeRate.cc                    //
//   Running the code:     ./jetToTauFakeRate.exe OutPut.root   Input.root  //
//////////////////////////////////////////////////////////////////////////////

void registerBranches(TTree *);
float TMass_F(float, float, float, float, float);

int main(int argc, char **argv) {
  using std::cout;
  using std::endl;
  using std::ios;

  std::string out = *(argv + 1);

  cout << "\nOUTPUT NAME IS:    " << out << endl;  // PRINTING THE OUTPUT FILE NAME
  TFile *fout = TFile::Open(out.c_str(), "RECREATE");

  std::string input = *(argv + 2);
  cout << "\nINPUT NAME IS:    " << input << endl;  // PRINTING THE INPUT FILE NAME
  TFile *myFile = TFile::Open(input.c_str());

  // add the histrograms of All gen-matched taus, and all gen-matched taus
  // passing numerator

  TH1F *histoDenominator = new TH1F("histoDenominator", "histoDenominator", 300, 0, 300);
  TH1F *histoLooseNumerator = new TH1F("histoLooseNumerator", "histoLooseNumerator", 300, 0, 300);
  TH1F *histoMediumNumerator = new TH1F("histoMediumNumerator", "histoMediumNumerator", 300, 0, 300);
  TH1F *histoTightNumerator = new TH1F("histoTightNumerator", "histoTightNumerator", 300, 0, 300);

  TTree *Run_Tree = reinterpret_cast<TTree*>(myFile->Get("EventTree"));
  cout.setf(ios::fixed, ios::floatfield);

  // hardcode lepton masses
  float MuMass = 0.10565837;
  float eleMass = 0.000511;

  // SetBranchAddress-es to read data from root file
  registerBranches(Run_Tree);

  // Loop over all events in the TTree.
  auto nentries_wtn = Run_Tree->GetEntries();
  for (auto ievt = 0; ievt < nentries_wtn; ievt++) {
    Run_Tree->GetEntry(ievt);  // read this event
    if (ievt % 1000 == 0) {
      fprintf(stdout, "\r  Processed events: %8d of %8d ", ievt, nentries_wtn);
    }
    fflush(stdout);

    TLorentzVector Mu4Momentum, Tau4Momentum;

    // pick the good muon
    for (auto imu = 0; imu < nMu; imu++) {
      // Muon kinematic selection.
      if (muPt->at(imu) < 30 || fabs(muEta->at(imu)) > 2.1) {
        continue;
      }

      // Apply muon ID.
      if (muIsMediumID->at(imu) < 0.5) {
        continue;
      }

      // Apply muon isolation.
      float IsoMu(muPFChIso->at(imu) / muPt->at(imu));
      IsoMu += std::max(0., (muPFNeuIso->at(imu) + muPFPhoIso->at(imu) - 0.5 * muPFPUIso->at(imu)) / muPt->at(imu));

      if (IsoMu > 0.1) {
        continue;
      }

      // Transverse mass cut to assure this is a W boson decaying.
      float MuMetTranverseMass = TMass_F(muPt->at(imu), muPt->at(imu) * cos(muPhi->at(imu)),
                                         muPt->at(imu) * sin(muPhi->at(imu)), pfMET, pfMETPhi);

      if (MuMetTranverseMass < 50) {
        continue;
      }

      // Set the muon 4-momentum to be used later.
      Mu4Momentum.SetPtEtaPhiM(muPt->at(imu), muEta->at(imu), muPhi->at(imu), MuMass);
      break;  // Only 1 good muon so no need to continue the loop.
    }  // End Muon loop.

    // pick a good tau
    for (auto itau = 0; itau < nTau; itau++) {
      // Tau kinematic selection.
      if (tauPt->at(itau) < 30 || fabs(tauEta->at(itau)) > 2.3) {
        continue;
      }

      // Anti-muon/electron discriminators.
      if (tauByTightMuonRejection3->at(itau) < 0.5 || tauByMVA6LooseElectronRejection->at(itau) < 0.5) {
        continue;
      }

      // Set tau 4-momentum and check dR between mu and tau
      Tau4Momentum.SetPtEtaPhiM(tauPt->at(itau), tauEta->at(itau), tauPhi->at(itau), tauMass->at(itau));
      if (Mu4Momentum.DeltaR(Tau4Momentum) < 0.5) {
        continue;
      }

      // Fill Denominator
      histoDenominator->Fill(tauPt->at(itau));
      // Fill Numerator
      if (tauByTightCombinedIsolationDeltaBetaCorr3Hits->at(itau) > 0.5) {
        histoTightNumerator->Fill(tauPt->at(itau));
      }
      if (tauByMediumCombinedIsolationDeltaBetaCorr3Hits->at(itau) > 0.5) {
        histoMediumNumerator->Fill(tauPt->at(itau));
      }
      if (tauByLooseCombinedIsolationDeltaBetaCorr3Hits->at(itau) > 0.5) {
        histoLooseNumerator->Fill(tauPt->at(itau));
      }
    }  // End Tau loop.
  }  // End Event loop.

  // close the file
  fout->cd();
  histoLooseNumerator->Write();
  histoMediumNumerator->Write();
  histoTightNumerator->Write();
  histoDenominator->Write();
  fout->Close();
}

void registerBranches(TTree *tree) {
  tree->SetBranchAddress("tauPt", &tauPt);
  tree->SetBranchAddress("tauPt", &tauPt);
  tree->SetBranchAddress("tauPt", &tauPt);
  tree->SetBranchAddress("tauPt", &tauPt);
  tree->SetBranchAddress("nTau", &nTau);
  tree->SetBranchAddress("tauPt", &tauPt);
  tree->SetBranchAddress("tauEta", &tauEta);
  tree->SetBranchAddress("tauPhi", &tauPhi);
  tree->SetBranchAddress("tauMass", &tauMass);
  tree->SetBranchAddress("tauDxy", &tauDxy);
  tree->SetBranchAddress("tauByTightMuonRejection3", &tauByTightMuonRejection3);
  tree->SetBranchAddress("tauByMVA6LooseElectronRejection", &tauByMVA6LooseElectronRejection);
  tree->SetBranchAddress("tauByLooseCombinedIsolationDeltaBetaCorr3Hits",
                         &tauByLooseCombinedIsolationDeltaBetaCorr3Hits);
  tree->SetBranchAddress("tauByMediumCombinedIsolationDeltaBetaCorr3Hits",
                         &tauByMediumCombinedIsolationDeltaBetaCorr3Hits);
  tree->SetBranchAddress("tauByTightCombinedIsolationDeltaBetaCorr3Hits",
                         &tauByTightCombinedIsolationDeltaBetaCorr3Hits);

  tree->SetBranchAddress("nMu", &nMu);
  tree->SetBranchAddress("muPt", &muPt);
  tree->SetBranchAddress("muEta", &muEta);
  tree->SetBranchAddress("muPhi", &muPhi);
  tree->SetBranchAddress("muIsMediumID", &muIsMediumID);
  tree->SetBranchAddress("muPFChIso", &muPFChIso);
  tree->SetBranchAddress("muPFPhoIso", &muPFPhoIso);
  tree->SetBranchAddress("muPFNeuIso", &muPFNeuIso);
  tree->SetBranchAddress("muPFPUIso", &muPFPUIso);
  tree->SetBranchAddress("muD0", &muD0);
  tree->SetBranchAddress("muDz", &muDz);

  tree->SetBranchAddress("pfMET", &pfMET);
  tree->SetBranchAddress("pfMETPhi", &pfMETPhi);
  tree->SetBranchAddress("HLTEleMuX", &HLTEleMuX);
}

float TMass_F(float pt3lep, float px3lep, float py3lep, float met, float metPhi) {
  return sqrt(pow(pt3lep + met, 2) - pow(px3lep + met * cos(metPhi), 2) - pow(py3lep + met * sin(metPhi), 2));
}
