# LPC CMSDAS 2019 - Tau Short Exercise

All code needed for the tau short exercise at the 2019 CMSDAS is contained in this repository. Basic templates for the analyzers are included and inteneded to be used by students to develope their own analyzers. A fully-implemented version of each analyzer is also included to be used as a reference.

## Setting up an area in LPC
Before grabbing the code, we need to setup our environment and get a CMSSW release. First, setup your environment

```
source /cvmfs/cms.cern.ch/cmsset_default.csh  # .sh for bash
setenv SCRAM_ARCH slc6_amd64_gcc491
```

Then, make a new directory for this exercise and get a CMSSW release
```
mkdir TauExercise
cd TauExercise
cmsrel CMSSW_7_4_14
cd CMSSW_7_4_14/src
cmsenv
```

## Getting the code
Clone this GitHub repository to get a copy of all the code you need
```
git clone git@github.com:tmitchel/DAS2019-Tau-Short.git
```
or if you don't have SSH keys setup
```
git clone https://github.com/tmitchel/DAS2019-Tau-Short.git
```
