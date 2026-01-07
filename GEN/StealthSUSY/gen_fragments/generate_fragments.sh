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

            sed -e "s|ChangeMstop|$mStop|g" \
            -e "s|ChangeMso|$mSo|g" \
            -e "s|ChangeCtau|$ctau|g" \
            Stealth${Model}_TEMPLATE.py > Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}.py

            echo "Generated fragment for Stealth${Model} with mStop = ${mStop}, mSo = ${mSo}, ctau = ${ctau}"
        done
    done
done