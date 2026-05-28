from CRABClient.UserUtilities import config
config = config()

mstop = "ChangeMstop"
lifetime = "ChangeCtau"
dset = "ChangeDataset"

fileTag = f'StopStopbarTo2Dbar2D_M-{mstop}_ctau-{lifetime}mm'
fileVersion = '_100kEvts_step3_v1'
theTag = fileTag+fileVersion

config.General.requestName = theTag

config.JobType.pluginName = 'Analysis'
# Name of the CMSSW configuration file
config.JobType.psetName = 'RECO_cfg.py'
config.JobType.numCores = 4
config.JobType.maxMemoryMB = 10000

config.Data.inputDBS = 'phys03'
config.Data.inputDataset = dset
config.Data.splitting = 'FileBased'
config.Data.unitsPerJob = 1
config.Data.publication = True
# This string is used to construct the output dataset name
config.Data.outputDatasetTag = theTag
config.Data.ignoreLocality = True

# These values only make sense for processing data
#    Select input data based on a lumi mask
#config.Data.lumiMask = 'Cert_190456-208686_8TeV_PromptReco_Collisions12_JSON.txt'
#    Select input data based on run-ranges
#config.Data.runRange = '190456-194076'

# Where the output files will be transmitted to
config.Site.whitelist = [
    'T2_BR_SPRACE',   # SPRACE Tier-2 in Brazil
    'T2_US_Purdue',   # Purdue (USA) Tier-2 – good for MC generation / analysis (in USCMS) :contentReference[oaicite:1]{index=1}
    'T2_US_Caltech',  # Caltech (USA) Tier-2 :contentReference[oaicite:2]{index=2}
    'T2_US_UCSD',     # UCSD (USA) Tier-2 :contentReference[oaicite:3]{index=3}
    'T2_US_Wisconsin',# Wisconsin (USA) Tier-2 :contentReference[oaicite:4]{index=4}
    'T2_AT_Vienna',   # Vienna (Austria) Tier-2 — used in example CRAB runs :contentReference[oaicite:5]{index=5}
    'T2_DE_RWTH',     # RWTH Aachen (Germany) Tier-2 — also appears as possible site in sample CRAB output :contentReference[oaicite:6]{index=6}
]
config.Site.storageSite = 'T2_BR_SPRACE'