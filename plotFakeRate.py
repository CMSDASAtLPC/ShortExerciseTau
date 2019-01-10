import math
from ROOT import TFile, TCanvas, TGraph, TGraphAsymmErrors
import array

Binning_PT = array.array("d", [0, 30, 35, 40, 50, 60, 75, 95, 120, 150, 200])
OutFile = TFile("OutPutFR.root")

HistoNum = OutFile.Get("histoLooseNumerator")
HistoNum = HistoNum.Rebin(len(Binning_PT)-1, "", Binning_PT)

HistoDeNum = OutFile.Get("histoDenominator")
HistoDeNum = HistoDeNum.Rebin(len(Binning_PT)-1, "", Binning_PT)

fakeRate = TGraphAsymmErrors(HistoNum, HistoDeNum, "")

canv = TCanvas("canv", "histograms", 0, 0, 600, 600)
canv.SetLogy()
fakeRate.GetXaxis().SetRangeUser(0, 200)
fakeRate.GetXaxis().SetTitle("#tau p_{T} [GeV]")
fakeRate.GetYaxis().SetRangeUser(0.01, 0.5)
fakeRate.SetTitle('Jet to Tau Fake Rate')
fakeRate.SetMarkerStyle(20)

fakeRate.Draw()

canv.SaveAs("jetToTauFR.pdf")
