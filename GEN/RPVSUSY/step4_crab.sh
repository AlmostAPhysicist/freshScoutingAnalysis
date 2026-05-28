#!/usr/bin/bash

# Parameter values for the loop
mStops=(200 400 600 800)
ctaus=(0.1 0.3 0.7)

for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        ctauLabel="${ctau/./p}"

        dataset="/StopStopbarTo2Dbar2D_M-${mStop}_ctau-${ctauLabel}mm_100kEvts_v2/brlopesd-StopStopbarTo2Dbar2D_M-${mStop}_ctau-${ctauLabel}mm_100kEvts_step3_v1-89578c67bc58e175e14cb8efc9d9e047/USER"

        echo "Sending CRAB step 4 for for mStop = ${mStop}, ctau = ${ctau}"
        echo "The dataset is ${dataset}"

        sed -e "s|ChangeMstop|$mStop|g" \
        -e "s|ChangeCtau|$ctauLabel|g" \
        -e "s|ChangeDataset|$dataset|g" \
        crab_step4_TEMPLATE.py > step4_mStop-${mStop}_ctau-${ctauLabel}_CRAB.py

        crab submit -c step4_mStop-${mStop}_ctau-${ctauLabel}_CRAB.py
        rm step4_mStop-${mStop}_ctau-${ctauLabel}_CRAB.py
    done
done