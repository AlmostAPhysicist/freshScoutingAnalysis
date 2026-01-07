# Stealth SUSY samples

This folder has all the necessary scripts to generate Stealth SUSY samples. It is proven to work on `CMSSW_14_0_21_patch4`.

### The model

Stealth SUSY is a way to preserve R-Parity, but still couple SUSY partners to the SM. It introduces a new chiral superfield which contains a new singlet (and its SUSY partner, a singlino). This superfield can either couple to the MSSM Higgs (SHH model), leading to b-jets in the final state, or to a new chiral supermultiplet Y (SYY model), leading to gluon jets -- see details in the paper: https://arxiv.org/abs/1105.5135

Here we focus on the case in which the lightest visible SM superpartner (LVSP) is charged (stop), which is not necessary for the theory. The decay chain for this case would be stop -> singlino (+ top) -> singlet (+ soft gravitino) -> jets. The singlet mass is fixed at 90 GeV and the gravitino at 1 GeV, as it was done in previous analyses for Run 2. We consider both the SHH and SYY cases, different mass gaps ("boosted" and "resolved" topologies, see below), stop masses starting from 300 GeV and several lifetimes, as the singlino is long-lived.

The "boosted" topology fixes the singlino mass at 100 GeV regardless of the stop mass. It is preferred in SUSY phenomenology, as the gap between the singlet and singlino is kept small. On the other hand, the "resolved" topology keeps the mass gap between the stop and the singlino at 225 GeV, possibly improving the vertex position uncertainty as the final state is less collimated. It constrains the minimum value of the stop mass, as the singlet would be off-shell if it is lighter than 91 GeV. We then set a minimum of 325 GeV for the stop mass, while the "boosted" case starts from 300 GeV.

The process is generated using MadGraph + Pythia. It uses a madgraph gridpack to generate the LHE events and then pythia to hadronise. This folder provides the cmsDriver workflow and CRAB scripts to generate samples based on a GEN fragment, available in `GEN/fragment_TEMPLATE.py`, which has the pythia8 parameters, including the decay table, and points to the madgraph gridpack. 

### How to generate samples

The workflow is divided in 5 basic steps (including the setup as step 0). In each step, we generate a configuration file and send it to CRAB using a simple bash script. 

#### step 0: Setting up

The first step is to prepare the gridpacks which will be used for the LHE generation at step 1. Stealth SUSY was centrally produced for the Run 2, so we can base our new gridpacks on what was done there -- the centre of mass energy is different and it uses an old madgraph version, so it needs to be regenerated. The old gridpacks are available at `/cvmfs/cms.cern.ch/phys_generator/gridpacks/2017/13TeV/madgraph/V5_2.4.2/sus_sms/LO_PDF/SMS-StopStop/v1/`. There's one gridpack for each stop mass, as madgraph is responsible for the stop generation.

In order to regenerate the gridpacks, pick the one corresponding to the desired stop mass in CVMFS and extract the tarball locally. Inside the `InputCards` folder, go to the `run_card` and change `ebeam1` and `ebeam2` from 6500 to 6800, then go to the `proc_card` and change the model from `mssm` to `MSSM_SLHA2`, so that it has the correct centre of mass energy and uses the most up-to-date model. Having the correct cards, follow the procedure available at https://cms-generators.docs.cern.ch/how-to-produce-gridpacks/mg5-amcnlo/ .

The new gridpacks need to be somewhere accessible to CRAB, e.g. the EOS area. There are premade gridpacks for mStop = {300, 325, 500, 700, 900} GeV available at `/eos/user/b/brlopesd/StealthSUSY_gridpacks`, which CRAB can access with a redirector, e.g. `root://eosuser.cern.ch//eos/user/b/brlopesd/StealthSUSY_gridpacks/SMS-StopStop_mStop-*_el8_amd64_gcc10_CMSSW_12_4_8_tarball.tar.xz`, changing the asterisk to the desired mass.

Having the gridpacks, the next step to the setup is to prepare the gen fragments. After cloning this repo into the `src` folder of a new `CMSSW_14_0_21_patch4` workspace and move to this branch with

```
source /cvmfs/cms.cern.ch/cmsset_default.sh
cmsrel CMSSW_14_0_21_patch4
cd CMSSW_14_0_21_patch4/src
git clone https://github.com/brunolopesbr2/Run3ScoutingAnalysisTools.git
cd Run3ScoutingAnalysisTools
git checkout -b Run3_JetHT_Summer24 origin/Run3_JetHT_Summer24
```

then move the fragment template and the generation script, which are under `gen_fragments`, to the path `Configuration/GenProduction/python` (create the folder) inside the CMSSW `src` directory. There are templates for both SHH and SYY models, and the only difference is the stop decay in the SLHA table. Choose the model and parameters by editing `generate_fragments.sh` -- at the top, there are the stop masses and singlino lifetimes, and inside the loop there are a couple of lines for the singlino mass, for whether generating the boosted or resolved topology (just uncomment what you are generating). Parameters chosen, generate the fragments with `bash generate_fragments.sh` and compile with `scram b -j8; scram b python`, so that CMSSW will understand that there are new GEN fragments available.

#### step 1: GEN-SIM (MadGraph + pythia8 + GEANT4)

With the fragments all set, the first step is to create the CMSSW configuration files with `cmsDriver.py`. This can be done by running the first command of the `cmsDriver_workflow.txt`. Alternatively, there's the script `generateStep1.sh` that runs the command in a bash loop to the desired parameters. To use the script, just edit the `mStops`, `ctaus` and `Models` to the desired values (consistent to what was done in the setup), choose the desired singlino mass by uncommenting the line, double-check the cmsDriver parameters (all settings are there, including the Global Tag) and run with `bash generateStep1.sh`

With the configuration files in place, `cmsRun` one of the generated files to test if everything is alright (don't forget to run `cmsenv`). If it runs fine, the next step is to prepare the CRAB jobs. The file used for that is `crab_step1_TEMPLATE.py`. First, have a look at this file, edit the tag (which controls the job and output dataset name) as desired, and choose the total number of events by changing `config.Data.splitting`. By default, it runs 100 events per job to avoid memory crashes, but it can be tuned.

At this point, it is worth mentioning that the MadGraph-Pythia jet matching can veto events. The parameters for this matching are the same as Run 2 and are set at the gen fragments. A few tests show that those parameters lead to an efficiency of around 25%, so that we need to ask 400k events in order to have 100k in the final dataset. In principle, this can be optimised, but for now it is preferred to be consistent with Run 2.

With the CRAB script in place, go to `step1_crab.sh`, edit the `mStops`, `Models`, `ctaus` and `mSo`, and check if everything is alright. Before sending the jobs, don't forget to source the CRAB configurations (once per shell) with:

```
source /cvmfs/cms.cern.ch/common/crab-setup.sh
```

then run with `bash step1_crab.sh`. Instructions to monitor the jobs will be printed on the screen.

#### step 2: HLT

Step 2 takes care of the detector electronics, pileup (by mixing with minimum bias events) and the trigger itself. The configuration file is generated using the second command in `cmsDriver_workflow.txt`. A premade configuration can be found at `DIGIPremix-HLT_cfg.py`. The only difference between what is generated by cmsDriver and the premade configuration is that the number of threads is set to 4 in order to optimise the CRAB jobs, which can be easily edited.

A critical detail here is the pileup premix dataset. Both the premade config file and the workflow use `/Neutrino_E-10_gun/RunIIISummer24PrePremix-Premixlib2024_140X_mcRun3_2024_realistic_v26-v1/PREMIX` for that, which is the only option on disk for Summer24 at the time of writing of this README. It can change in the future, and the config file needs to be regenerated with the new dataset, which can be found in DAS.

As the premix dataset has order of 500k files, the config file becomes large and the CRAB jobs need more memory. Also, be sure to have a few gigabytes of disk available before sending the jobs. If everything is ready, have a look at `crab_step2_TEMPLATE.py`, edit the tag as desired and note it now requires 4 threads and 10000 MB of memory. 

As previously, the script to run is `step2_crab.sh`. This time, however, the input datasets (output from the previous step) are required. As different config files were used in the first step, each output dataset has a different random tag in the name. To handle that, the script uses a bash associative array. Check the outputs of the first step using the instructions provided by CRAB and edit the dataset names accordingly, and also don't forget to change the `mStops`, `Models`, `ctaus`, `mSo` and the dataset indices. When everything is in place, just send the jobs with `bash step2_crab.sh`.

#### step 3: Offline RECO + AOD

This step takes care of the offline reconstruction and saves the output in the AOD format. It can be done with the third command of `cmsDriver_workflow.txt` or using the premade `RECO_cfg.py`, which is set to 4 threads as done in step 2. With the config file in hand, edit the tag and job parameters in `crab_step3_TEMPLATE.py` and go to the running script `step3_crab.sh`.

In the bash script, note that now the dataset name is constructed within the loop. This is possible because the random tag is the same for all step 2 output datasets, as they use the same config file. Edit the `mStops`, `Models`, `ctaus`, `mSo`, change the dataset name to the output of the previous step and run with `bash step3_crab.sh`.

#### step 4: MiniAOD

This step constructs the datasets in the miniAOD format. It can be done with the fourth command of `cmsDriver_workflow.txt` or using the premade `MiniAOD_cfg.py`, which also uses 4 threads as previously. The way to run it is precisely the same as step 3: edit the tag and job parameters in `crab_step4_TEMPLATE.py`, the parameters and input dataset in `step4_crab.sh` and run with `bash step4_crab.sh`.