'''
Usage:python plotRocCurve.py 
Script to plot a Roc curve based on 3 IDs from 2 root files with historgrams.
2016 CMSDAS Tau Exercise
'''

from subprocess import Popen
from sys import argv, exit, stdout, stderr

import ROOT
import numpy
from array import array

import plotRocCurve_def

# So things don't look like crap.
ROOT.gROOT.SetStyle("Plain")
ROOT.gROOT.SetBatch(True)  # run in batch mode
ROOT.gStyle.SetOptStat(0)  # don't plot statistics

######## Load Files #########
ntuple_fileeff = ROOT.TFile("outputEfficiency.root")  # file efficiency
ntuple_filefr = ROOT.TFile("outputFR.root")  # file fake rate

#####################################
plotRocCurve_def.produce_roc_curve(
    ntuple_fileeff, ntuple_filefr,  # ntuple files with histograms
    # 'histogramName No. 1', ' legendTitle No. 1'
    'histoLooseNumerator', 'byCombinedLoose',
			 # 'histogramName No. 2', ' legendTitle No. 2'
			 'histoMediumNumerator', 'byCombinedMedium',
			 # 'histogramName No. 3', ' legendTitle No. 3'
			 'histoTightNumerator', 'byCombinedTight',
			 # 'Top Title of Plot; X-Axis Title; Y-Axis Title'
			 'ROC; tau Efficiency;tau Fake Rate',
			 'roc'  # name of saved plot
)
