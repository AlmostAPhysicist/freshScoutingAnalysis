from CRABClient.UserUtilities import config
config = config()

mStop = ChangeMstop
ctau = ChangeCtau

mStopString = str(int(mStop))
ctauString = str(ctau).replace('.','p')
    
fileTag = f'StopStopbarTo2Dbar2D_M-{mStopString}_ctau-{ctauString}mm'
fileVersion = '_100kEvts_v2'
fullTag = fileTag+fileVersion
psetName = f'GENSIM_mStop-{mStopString}_ctau-{ctauString}.py'

config.General.requestName   = fullTag
config.JobType.pluginName = 'PrivateMC'
# Name of the CMSSW configuration file
config.JobType.psetName = psetName

# This string determines the primary dataset of the newly-produced outputs.
# For instance, this dataset will be named /CrabTestSingleMu/something/USER
config.Data.outputPrimaryDataset = fullTag
config.Data.splitting = 'EventBased'
config.Data.unitsPerJob = 30
config.Data.totalUnits = 100000
config.Data.publication = True

# This string is used to construct the output dataset name
config.Data.outputDatasetTag = 'CRAB3_MC_'+fullTag

# Where the output files will be transmitted to
config.Site.storageSite = 'T2_BR_SPRACE'