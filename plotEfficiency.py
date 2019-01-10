import math
from ROOT import TFile, TCanvas, TGraph, TGraphAsymmErrors
import array

Binning_PT = array.array(
    'd', [0, 20, 25, 30, 40, 55, 75, 95, 120, 150, 200, 300])
OutFile = TFile('outputEfficiency.root')

HistoNum = OutFile.Get('histoNumerator')
HistoNum = HistoNum.Rebin(len(Binning_PT)-1, '', Binning_PT)

HistoDeNum = OutFile.Get('histoDenominator')
HistoDeNum = HistoDeNum.Rebin(len(Binning_PT)-1, '', Binning_PT)

tauEffi = TGraphAsymmErrors(HistoNum, HistoDeNum, '')

canv = TCanvas('canv', 'histograms', 0, 0, 600, 600)

tauEffi.SetMinimum(0.5)
tauEffi.GetXaxis().SetRangeUser(0, 300)
tauEffi.GetXaxis().SetTitle('Reconstructed #tau p_{T} [GeV]')
tauEffi.GetYaxis().SetRangeUser(0, 1)
tauEffi.SetTitle('Tau Efficiency')
tauEffi.SetMarkerStyle(20)

tauEffi.Draw()

canv.SaveAs('tauEfficiency.pdf')
