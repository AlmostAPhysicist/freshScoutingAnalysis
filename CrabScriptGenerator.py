inputDatasets = ["/QCD-4Jets_Bin-HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-40to70_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-70to100_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/QCD-4Jets_Bin-HT-100to200_TuneCP5_13p6TeV_madgraphMLM-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/TTtoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/TTto4Q_TuneCP5_13p6TeV_powheg-pythia8/RunIII2024Summer24MiniAOD-140X_mcRun3_2024_realistic_v26-v2/MINIAODSIM",
                 "/StopStopbarTo2Dbar2D_M-200_CTau-10mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-200_CTau-10mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-200_CTau-1mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-200_CTau-1mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-200_CTau-3mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-200_CTau-3mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-400_CTau-10mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-400_CTau-10mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-400_CTau-1mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-400_CTau-1mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-400_CTau-3mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-400_CTau-3mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-600_CTau-10mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-600_CTau-10mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-600_CTau-1mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-600_CTau-1mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-600_CTau-3mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-600_CTau-3mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-800_CTau-10mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-800_CTau-10mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-800_CTau-1mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-800_CTau-1mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/StopStopbarTo2Dbar2D_M-800_CTau-3mm_Summer24_100k_v2/brlopesd-StopStopbarTo2Dbar2D_M-800_CTau-3mm_Summer24_100k_miniAOD_v2-df1e99b50d14b85be33e7e4ab518ee3a/USER",
                 "/ScoutingPFRun3/Run2024C-v1/HLTSCOUT",
                 "/ScoutingPFRun3/Run2024D-v1/HLTSCOUT",
                 "/ScoutingPFRun3/Run2024E-v1/HLTSCOUT",
                 "/ScoutingPFRun3/Run2024F-v1/HLTSCOUT",
                 "/ScoutingPFRun3/Run2024G-v1/HLTSCOUT",
                 "/ScoutingPFRun3/Run2024H-v1/HLTSCOUT",
                 "/ScoutingPFRun3/Run2024I-v1/HLTSCOUT"
                 ]

crossSections = [1961000000, 95620000, 13540000, 3033000, 883700, 383500, 125200, 26490, 311400000000, 58500000000, 25400000000, 762100, 762100] #Backgrounds should go first in dataset list with corresponding cross sections listed here
tagSuffix = "v19-4TrackMin"
scouting = True

for i in range(len(inputDatasets)):
    dataset = inputDatasets[i]
    if ("TTto" in dataset) or ("QCD" in dataset):
        crossSection = crossSections[i]
    else:
        crossSection = 1
    if "Stop" in dataset:
        tag = dataset[1:].split("_Summer24")[0] + "_Tree_" + tagSuffix
        dataBase = "phys03"
        totalUnits = 100000
        isMC = True
        hasReco = True
        unitsPerJob = 60000
        splitting = 'EventAwareLumiBased'
    elif "ScoutingPF" in dataset:
        tag = dataset[16:].split("-v1")[0] + "_Tree_" + tagSuffix
        dataBase = "global"
        totalUnits = 300000000
        isMC = False
        hasReco = False
        unitsPerJob = 300000
        splitting = 'EventAwareLumiBased'
    else:
        tag = dataset[1:].split("_Tune")[0] + "_Tree_" + tagSuffix
        dataBase = "global"
        totalUnits = 125000000
        isMC = True
        hasReco = True
        unitsPerJob = 100000
        splitting = 'EventAwareLumiBased'
    string = f"""from CRABClient.UserUtilities import config \n
config = config() \n
theTag = '{tag}' \n
config.General.requestName = theTag \n
config.JobType.pluginName = 'Analysis' \n
config.JobType.psetName = '../VertexNTupleMaker.py' \n
config.Data.inputDBS = '{dataBase}' \n
config.Data.inputDataset = '{dataset}' \n
config.Data.splitting = '{splitting}' \n
config.Data.unitsPerJob = {unitsPerJob} \n
config.Data.totalUnits = {totalUnits} \n
config.Data.allowNonValidInputDataset = True \n
config.Data.publication = False \n
config.JobType.pyCfgParams = ['isScouting={scouting}','lumi=108.96','crossSection={crossSection}','isMC={isMC}','hasReco={hasReco}']
config.Data.outputDatasetTag = theTag \n
config.Data.outLFNDirBase = '/store/group/phys_exotica/DVScouting' \n
config.Site.storageSite = 'T2_CH_CERN' \n
config.Site.blacklist=['T2_US_MIT'] \n""" 

    # Write to a temporary file
    if "Stop" in dataset:
        string += """config.Site.whitelist = ['T2_CH_CERN'] \n
config.Data.ignoreLocality = True"""
        with open("crabSubmitScripts/"+dataset[1:].split("_Summer24")[0]+"_crabConfig.py", "w") as f:
            f.write(string)
    elif "ScoutingPF" in dataset:
        string += f"""config.Data.lumiMask = '../GoldenJSON/2024' + '{dataset[23]}' + '_Golden.json'"""
        with open("crabSubmitScripts/"+dataset[16:].split("-v1")[0]+"_crabConfig.py", "w") as f:
            f.write(string)
    else:
        with open("crabSubmitScripts/"+dataset[1:].split("_Tune")[0]+"_crabConfig.py", "w") as f:
            f.write(string)
