# Run3ScoutingAnalysisTools
### Repository for Run 3 scouting analysis tools for Displaced Vertices search

#### Documentation

Further documentation on the code is available at [CodiMD](https://codimd.web.cern.ch/IVPWTLhKQCWhAhdZ9YfAGg#)

#### Setup
Setup a CMSSW working area and clone the `Run3ScoutingAnalysisTools` repo in the specific branch `Run3_JetHT_Summer24`. For Summer24, we are using the CMSSW 14.0.18_patch1 release:
```
cmsrel CMSSW_14_0_18_patch1
cd CMSSW_14_0_18_patch1/src
cmsenv
git cms-init
git clone https://github.com/brunolopesbr2/Run3ScoutingAnalysisTools.git -b Run3_JetHT_Summer24
scram b -j 8
```

#### The framework and how to run it

The framework is divided in three main components, in order to have the offline displaced vertices and to extract meaningful distributions from them:

- `HLTScoutingUnpackProducer`: produces `reco::Track`s from the scouting tracks. 

- `Vertexer`: takes the tracks from the Unpacker to fit displaced vertices, and saves in the format `reco::Vertex`.

- `ScoutingTreeMakerRun3`: Extract the distributions relevant for the analysis and saves in a ROOT tree.


#### Run the HLTScoutingUnpackProducer

IMPORTANT: there are some changes to the Scouting DataFormats needed to run this EDProducer. Move the `DataFormats` folder of this repo to the `src` folder and compile before running. This was taken from [this branch from Patin](https://github.com/patinkaew/cmssw/blob/scouting_nano_dev_14_0_13_patch1/PhysicsTools/Scouting/plugins/HLTScoutingUnpackProducer.cc)

For now, this version of the UnpackProducer takes the Scouting collections and build the standard vertices and tracks collections using fake hit patterns. It only saves trigger results and scouting collections on the output. Simply change the input file and run with `cmsRun HLTScoutingUnpackProducer.py` for a test, this will produce a ROOT file locally. Then, if everything is alright, input the desired dataset to `HLTScoutingUnpackProducer_CRAB.py`, change the tag for the output dataset accordingly and run with:

```
source /cvmfs/cms.cern.ch/common/crab-setup.sh
crab submit -c HLTScoutingUnpackProducer_CRAB.py
```

#### Run the Vertexer

The Vertexer takes as input the output of the HLTScoutingUnpackProducer. Particularly, it needs the scouting tracks in the `reco::Track` data format. It then fits displaced vertices from those tracks. In order to run it, first run a local test with `cmsRun Vertexer.py` using the output of the HLTScoutingUnpackProducer test, then submit CRAB jobs with the corresponding dataset and tag using `Vertexer_CRAB.py` and run it:

```
source /cvmfs/cms.cern.ch/common/crab-setup.sh
crab submit -c Vertexer_CRAB.py
```

Note that, for the vertexer, there are several input parameters that can be tuned by changing `Vertexer.py`. For further details, please rever to the documentation at the top of this README.

#### Run the ScoutingTreeMakerRun3

The tree maker uses all the scouting information and the vertices obtained from the Vertexer step to extract meaningful distributions for our analysis. It is under constant development, and everything is done in `ScoutingTreeMakerRun3/plugins/ScoutingTreeMakerRun3.cc`. It takes the Vertexer output as input. A test can be done with `cmsRun tree.py` and jobs are submitted with:

```
source /cvmfs/cms.cern.ch/common/crab-setup.sh
crab submit -c tree_CRAB.py
```

As a caveat, there are some adjustable parameters in `tree.py`. In particular, it can pick offline reconstructed or scouting tracks, and tune the selection criteria. Adjust accordingly before sending jobs.

#### Run the whole framework at once

Once every part of the fremework is settled, the whole framework can be run at once with `VertexNTupleMaker.py` in a similar fashion as the individual modules.
