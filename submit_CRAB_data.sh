#!/usr/bin/bash

# List of parameter ranges to substitute
ranges=(
    '40to70'
    '70to100'
    '100to200'
    '200to400'
    '400to600'
    '600to800'
    '800to1000'
    '1000to1200'
    '1200to1500'
    '1500to2000'
    '2000'
)

Eras=(
    'C'
    'D'
    'E'
    'F'
    'G'
    'H'
    'I'
)

# Original config file
template="VertexNTupleMaker_CRAB_data.py"

# Loop over all ranges and submit jobs
for Era in "${Eras[@]}"; do
    # Create a temp config file
    temp_config="VertexNTupleMaker_CRAB_${Era}.py"

    # Replace "something" with the current range
    sed "s/fillme/${Era}/g" "$template" > "$temp_config"

    # Submit the job
    crab submit -c "$temp_config"

    # Optional: Remove the temp file afterward
    rm "$temp_config"
done
