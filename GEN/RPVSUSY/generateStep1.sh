#!/usr/bin/bash

# Parameter values for the loop
mStops=(200 400 600 800)
ctaus=(0.1 0.3 0.7)

for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        ctauLabel="${ctau/./p}"

        cmsDriver.py Configuration/GenProduction/python/fragment_mStop-${mStop}_ctau-${ctauLabel}.py \
        --python_filename GENSIM_mStop-${mStop}_ctau-${ctauLabel}.py \
        --eventcontent RAWSIM \
        --customise Configuration/DataProcessing/Utils.addMonitoring \
        --datatier GEN-SIM \
        --fileout file:GEN-SIM.root \
        --conditions 140X_mcRun3_2024_realistic_v26 \
        --beamspot DBrealistic \
        --step GEN,SIM \
        --geometry DB:Extended \
        --era Run3_2024 \
        --no_exec \
        --mc \
        -n 10

        echo "Created step 1 file for mStop = ${mStop}, ctau = ${ctau}"
    done
done