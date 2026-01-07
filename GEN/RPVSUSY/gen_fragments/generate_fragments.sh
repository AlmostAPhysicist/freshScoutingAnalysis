#!/usr/bin/bash

# Parameter values for the loop
mStops=(200 400 600 800)
ctaus=(0.1 0.3 0.7)

for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        ctauLabel="${ctau/./p}"

        sed -e "s|ChangeMass|$mStop|g" \
        -e "s|ChangeCtau|$ctau|g" \
        fragment_TEMPLATE.py > fragment_mStop-${mStop}_ctau-${ctauLabel}.py

        echo "Generated fragment with mStop = ${mStop}, ctau = ${ctau}"
    done
done