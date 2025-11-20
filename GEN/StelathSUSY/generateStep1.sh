#!/usr/bin/bash

# Parameter values for the loop
mStops=(300 500 700 900)
ctaus=(0.1 1 3 10)
Models=("SHH" "SYY")

for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        for Model in "${Models[@]}"; do
            ctauLabel="${ctau/./p}"
            #mSo=(100)
            mSo=$((mStop - 225)) # Boosted case: mSo = 100 GeV, Resolved case: mSo = mStop - 225 GeV

            cmsDriver.py Configuration/GenProduction/python/Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}.py \
            --python_filename GENSIM_stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}.py \
            --eventcontent RAWSIM \
            --customise Configuration/DataProcessing/Utils.addMonitoring \
            --datatier GEN-SIM \
            --fileout file:GEN-SIM.root \
            --conditions 140X_mcRun3_2024_realistic_v26 \
            --beamspot DBrealistic \
            --step LHE,GEN,SIM \
            --geometry DB:Extended \
            --era Run3_2024 \
            --no_exec \
            --mc \
            -n 10

            echo "Created step 1 file for for mStop = ${mStop}, mSo = ${mSo}, ctau = ${ctau}"
        done
    done
done