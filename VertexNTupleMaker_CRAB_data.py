from CRABClient.UserUtilities import config
config = config()

Era = 'fillme'

theTag = "ScoutingData_2024" + Era + "_30Mevts_v9"
config.General.requestName = theTag

config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = 'VertexNTupleMaker.py'
#config.JobType.maxMemoryMB = 5000

config.Data.inputDBS = 'global'
config.Data.inputDataset = '/ScoutingPFRun3/Run2024' + Era + '-v1/HLTSCOUT'
config.Data.splitting = 'EventAwareLumiBased'
config.Data.totalUnits = 30000000
config.Data.unitsPerJob = 10000
config.Data.outLFNDirBase = '/store/user/brlopesd/DataVsBG_30M_v9/Data'
config.Data.publication = False
# This string is used to construct the output dataset name
config.Data.outputDatasetTag = theTag


# These values only make sense for processing data
#    Select input data based on a lumi mask
config.Data.lumiMask = 'GoldenJSON/2024' + Era + '_Golden.json'
#    Select input data based on run-ranges
#config.Data.runRange = '190456-194076'

# Where the output files will be transmitted to
config.Site.storageSite = 'T3_CH_CERNBOX'
