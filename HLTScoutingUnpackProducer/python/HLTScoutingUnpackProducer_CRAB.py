from CRABClient.UserUtilities import config
config = config()

mass = "M-800"
lifetime = "CTau-1mm"

theTag = "StopStopbarTo2Dbar2D_"+mass+"_"+lifetime+ "_Summer24_InputToVertexer_v1"
config.General.requestName = theTag

config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = 'HLTScoutingUnpackProducer.py'
#config.JobType.maxMemoryMB = 5000

config.Data.inputDBS = 'phys03'
config.Data.inputDataset = '/StopStopbarTo2Dbar2D_M-800_CTau-1mm_Summer24_100k_v1/brlopesd-StopStopbarTo2Dbar2D_M-800_CTau-1mm_Summer24_100k_v1-e5438bb52f42c4163dc01d5bb2e896e9/USER'
config.Data.splitting = 'Automatic'
#config.Data.unitsPerJob = 1
config.Data.publication = True
# This string is used to construct the output dataset name
config.Data.outputDatasetTag = theTag

# These values only make sense for processing data
#    Select input data based on a lumi mask
#config.Data.lumiMask = 'Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
#    Select input data based on run-ranges
#config.Data.runRange = '190456-194076'

# Where the output files will be transmitted to
config.Site.storageSite = 'T2_BR_SPRACE'
