#!/usr/bin/bash

# Parameter values for the loop
mStops=(325 500 700 900)
ctaus=(0.1 1 3 10)
Models=("SHH" "SYY")

for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        for Model in "${Models[@]}"; do
            ctauLabel="${ctau/./p}"
            #mSo=(100)
            mSo=$((mStop - 225)) # Boosted case: mSo = 100 GeV, Resolved case: mSo = mStop - 225 GeV

            dataset="/Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}mm100kEvts_v2/brlopesd-Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}mm_step2_100kEvts_v7-48cd555cdf6be04e2ea7b276952e1164/USER"

            echo "Sending CRAB step 3 for for mStop = ${mStop}, mSo = ${mSo}, ctau = ${ctau}"
            echo "The dataset is ${dataset}"

            sed -e "s|ChangeMstop|$mStop|g" \
            -e "s|ChangeMso|$mSo|g" \
            -e "s|ChangeCtau|$ctauLabel|g" \
            -e "s|ChangeModel|$Model|g" \
            -e "s|ChangeDataset|$dataset|g" \
            crab_step3_TEMPLATE.py > Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}_CRAB_step3.py

            crab submit -c Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}_CRAB_step3.py
            rm Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}_CRAB_step3.py
        done
    done
done