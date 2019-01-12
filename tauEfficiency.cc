#include <ostream>
#include <string>
#include "TreeReader.h"

//////////////////////////////////////////////////////////////////////////
//   Compiling the code:   ./Make.sh tauEfficiency.cc                   //
//   Running the code:     ./tauEfficiency.exe OutPut.root   Input.root //
//////////////////////////////////////////////////////////////////////////

void registerBranches(TTree *tree);

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

  // SetBranchAddress-es to read data from root file
  // ...

  // close the file at the end
  fout->cd();
  histoLooseNumerator->Write();
  histoMediumNumerator->Write();
  histoTightNumerator->Write();
  histoDenominator->Write();
  fout->Close();
}

void registerBranches(TTree *tree) {
}
