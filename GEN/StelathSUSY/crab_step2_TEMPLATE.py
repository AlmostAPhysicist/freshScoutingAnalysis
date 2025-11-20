from CRABClient.UserUtilities import config
config = config()

mstop = "ChangeMstop"
mso = "ChangeMso"
lifetime = "ChangeCtau"
model = "ChangeModel"
dset = "ChangeDataset"

theTag = f"Stealth{model}_mStop-{mstop}_mSo-{mso}_ctau-{lifetime}mm_step2_100kEvts_v3"
config.General.requestName = theTag

config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = 'DIGIPremix-HLT_cfg.py'
config.JobType.numCores = 4
config.JobType.maxMemoryMB = 5000

config.Data.inputDBS = 'phys03'
config.Data.inputDataset = dset
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
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