inputDatasets = ["/QCD-4Jets_Bin-HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/TTtoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/TTto4Q_TuneCP5_13p6TeV_powheg-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM"]

crossSections = [1961000000, 95620000, 13540000, 3033000, 883700, 383500, 125200, 26490, 762100, 762100]
tagSuffix = "v6Scouting"
scouting = True

for i in range(len(inputDatasets)):
    dataset = inputDatasets[i]
    crossSection = crossSections[i]
    tag = dataset[1:].split("_Tune")[0] + "_Tree_" + tagSuffix 
    string = f"""from CRABClient.UserUtilities import config \n
config = config() \n
theTag = '{tag}' \n
config.General.requestName = theTag \n
config.JobType.pluginName = 'Analysis' \n
config.JobType.psetName = '../HLTScoutingUnpackProducer.py' \n
config.Data.inputDBS = 'global' \n
config.Data.inputDataset = '{dataset}' \n
config.Data.splitting = 'EventAwareLumiBased' \n
config.Data.unitsPerJob = 200 \n
config.Data.totalUnits = 1000000 \n
config.Data.allowNonValidInputDataset = True \n
config.Data.publication = False \n
config.JobType.pyCfgParams = ['isScouting={scouting}','lumi=108.96','crossSection={crossSection}']
config.Data.outputDatasetTag = theTag \n
config.Data.outLFNDirBase = '/store/group/phys_exotica/DVScouting' \n
config.Site.storageSite = 'T2_CH_CERN'""" 



    # Write to a temporary file
    with open("crabSubmitScripts/"+dataset[1:].split("_Tune")[0]+"_crabConfig.py", "w") as f:
        f.write(string)
