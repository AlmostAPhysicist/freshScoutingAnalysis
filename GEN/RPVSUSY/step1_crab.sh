#!/usr/bin/bash

# Parameter values for the loop
mStops=(200 400 600 800)
ctaus=(0.1 0.3 0.7)

for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        ctauLabel="${ctau/./p}"
        echo "Sendig CRAB step 1 for for mStop = ${mStop}, ctau = ${ctau}"

        sed -e "s|ChangeMstop|$mStop|g" \
        -e "s|ChangeCtau|$ctau|g" \
        crab_step1_TEMPLATE.py > step1_mStop-${mStop}_ctau-${ctauLabel}_CRAB.py

        crab submit -c step1_mStop-${mStop}_ctau-${ctauLabel}_CRAB.py
    done
done