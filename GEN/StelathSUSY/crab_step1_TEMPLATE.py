from CRABClient.UserUtilities import config
config = config()

mStop = ChangeMstop
mSo = ChangeMso
ctau = ChangeCtau
Model = 'ChangeModel'

mStopString = str(int(mStop))
mSoString = str(int(mSo))
ctauString = str(ctau).replace('.','p')
    
fileTag = f'Stealth{Model}_mStop-{mStopString}_mSo-{mSoString}_ctau-{ctauString}mm'
fileVersion = '100kEvts_v1'
fullTag = fileTag+fileVersion
psetName = f'GENSIM_stealth{Model}_mStop-{mStopString}_mSo-{mSoString}_ctau-{ctauString}.py'

config.General.requestName   = fullTag
config.JobType.pluginName = 'PrivateMC'
# Name of the CMSSW configuration file
config.JobType.psetName = psetName

# This string determines the primary dataset of the newly-produced outputs.
# For instance, this dataset will be named /CrabTestSingleMu/something/USER
config.Data.outputPrimaryDataset = fullTag
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 100
config.Data.totalUnits = 100000
config.Data.publication = True

# This string is used to construct the output dataset name
config.Data.outputDatasetTag = 'CRAB3_MC_'+fullTag

# Where the output files will be transmitted to
config.Site.storageSite = 'T2_BR_SPRACE'