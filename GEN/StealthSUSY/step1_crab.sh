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
            echo "Sendig CRAB step 1 for for mStop = ${mStop}, mSo = ${mSo}, ctau = ${ctau}"

            sed -e "s|ChangeMstop|$mStop|g" \
            -e "s|ChangeMso|$mSo|g" \
            -e "s|ChangeCtau|$ctau|g" \
            -e "s|ChangeModel|$Model|g" \
            crab_step1_TEMPLATE.py > Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}_CRAB.py

            crab submit -c Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}_CRAB.py
        done
    done
done