# LPC CMSDAS 2019 - Tau Short Exercise

All code needed for the tau short exercise at the 2019 CMSDAS is contained in this repository. Basic templates for the analyzers are included and inteneded to be used by students to develope their own analyzers. A fully-implemented version of each analyzer is also included to be used as a reference.

##### Table of Contents
- [Setting up an area in LPC](#lpc) <br/>
- [Getting the code](#code) <br/>
- [Input file location](#location) <br/>
- [Compiling Analyzers](#compile) <br/>
- [Running Analyzers](#run) <br/>
- [Plotting Scripts](#plot) <br/>
  - [Plotting the jet fake rate](#plotFake) <br/>
  - [Plotting the tau ID efficiency](#plotEff) <br/>
  - [Producing ROC curves](#plotROC) <br/>


<a name="lpc"/>

## Setting up an area in LPC
Before grabbing the code, we need to setup our environment and get a CMSSW release. First, setup your environment

```
source /cvmfs/cms.cern.ch/cmsset_default.csh  # .sh for bash
setenv SCRAM_ARCH slc6_amd64_gcc491  # for .csh
# export SCRAM_ARCH=slc6_amd64_gcc491  # for .sh
```

Then, make a new directory for this exercise and get a CMSSW release
```
mkdir TauExercise
cd TauExercise
cmsrel CMSSW_7_4_14
cd CMSSW_7_4_14/src
cmsenv
```

<a name="code"/>

## Getting the code
Clone this GitHub repository to get a copy of all the code you need
```
git clone git@github.com:tmitchel/DAS2019-Tau-Short.git
```
or if you don't have SSH keys setup
```
git clone https://github.com/tmitchel/DAS2019-Tau-Short.git
```

<a name="location"/>

## Input file location
All input ROOT files are located at in the LPC eos area
```
$ xrdfsls ls -u /store/user/tmitchel/DAS2019-Tau-Short
root://131.225.204.161:1094//store/user/tmitchel/DAS2019-Tau-Short/DYJetsToLL_M-50_Inc_ShortEx.root
root://131.225.204.161:1094//store/user/tmitchel/DAS2019-Tau-Short/SingleMuon.root
root://131.225.204.161:1094//store/user/tmitchel/DAS2019-Tau-Short/WJetsToLNu_Inc_ShortEx.root
```

<a name="compile"/>

## Compiling Analyzers
A small bash script `Make.sh` can be used to compile the analyzers into a binary with the same name, but extension `.exe`. An example is shown below:
```
./Make.sh tauEfficiency_completed.cc
```

<a name="run"/>

## Running Analyzers
Analyzers take two extra inputs: (1) the output file name and (2) a path to the input file. An example is shown below:
```
tauEfficiency_completed.exe outputFile.root root://131.225.204.161:1094//store/user/tmitchel/DAS2019-Tau-Short/DYJetsToLL_M-50_Inc_ShortEx.root
```

<a name="plot"/>

## Plotting Scripts
Plotting scripts are included to produce the plots you will need. To see how the plotting is down with PyROOT, open up one of the files and look around. They are quite simple.

<a name="plotFake"/>

#### Plotting the jet fake rate
```
python plotFakeRate.py
```

<a name="plotEff"/>

#### Plotting the tau ID efficiency
```
python plotEfficiency.py
```

<a name="plotROC"/>

#### Producing ROC curves
```
python plotRocCurve.py
```