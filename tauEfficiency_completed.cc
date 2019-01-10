#include <ostream>
#include <string>
#include "TreeReader.h"

//////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh tauEfficiency.cc                   //
//   Running the code:     ./tauEfficiency.exe OutPut.root   Input.root //
//////////////////////////////////////////////////////////////////////////

void registerBranches(TTree *);

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

  // add the histrograms of All gen-matched taus, and all gen-matched taus passing numerator
  TH1F *histoDenominator = new TH1F("histoDenominator", "histoDenominator", 300, 0, 300);
  TH1F *histoLooseNumerator = new TH1F("histoLooseNumerator", "histoLooseNumerator", 300, 0, 300);
  TH1F *histoMediumNumerator = new TH1F("histoMediumNumerator", "histoMediumNumerator", 300, 0, 300);
  TH1F *histoTightNumerator = new TH1F("histoTightNumerator", "histoTightNumerator", 300, 0, 300);

  // TTree *Run_Tree = (TTree *)myFile->Get("EventTree");
  TTree *Run_Tree = reinterpret_cast<TTree *>(myFile->Get("EventTree"));
  cout.setf(ios::fixed, ios::floatfield);

  registerBranches(Run_Tree);
  TLorentzVector Tau4Momentum, MC4Momentum;

  auto nentries_wtn = Run_Tree->GetEntries();
  for (auto ievt = 0; ievt < nentries_wtn; ievt++) {
    Run_Tree->GetEntry(ievt);
    if (ievt % 1000 == 0) {
      fprintf(stdout, "\r  Processed events: %8d of %8d ", ievt, nentries_wtn);
    }
    fflush(stdout);

    for (auto itau = 0; itau < nTau; itau++) {
      if (tauPt->at(itau) < 30 || fabs(tauEta->at(itau)) > 2.3 || fabs(tauDxy->at(itau)) < 0.05) {
        continue;
      }

      Tau4Momentum.SetPtEtaPhiM(tauPt->at(itau), tauEta->at(itau), tauPhi->at(itau), tauMass->at(itau));

      for (auto igen = 0; igen < nMC; igen++) {
        MC4Momentum.SetPtEtaPhiM(mcPt->at(igen), mcEta->at(igen), mcPhi->at(igen), mcMass->at(igen));
        if (mcPID->at(igen) != 15 || MC4Momentum.DeltaR(Tau4Momentum) > 0.2) {
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
        break;  // already found the match.
      }
    }
  }

  // close the file at the end
  fout->cd();
  histoLooseNumerator->Write();
  histoMediumNumerator->Write();
  histoTightNumerator->Write();
  histoDenominator->Write();
  fout->Close();
}

void registerBranches(TTree *tree) {
  tree->SetBranchAddress("nMC", &nMC);
  tree->SetBranchAddress("mcPID", &mcPID);
  tree->SetBranchAddress("mcPt", &mcPt);
  tree->SetBranchAddress("mcMass", &mcMass);
  tree->SetBranchAddress("mcEta", &mcEta);
  tree->SetBranchAddress("mcPhi", &mcPhi);

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
}
