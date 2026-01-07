#!/usr/bin/bash

# Parameter values for the loop
mStops=(200 400 600 800)
ctaus=(0.1 0.3 0.7)

declare -A datasets

datasets["200_0p1"]="/StopStopbarTo2Dbar2D_M-200_ctau-0p1mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-200_ctau-0p1mm_100kEvts_v2-44745f5249811ac9741ca571bf46be02/USER"
datasets["200_0p3"]="/StopStopbarTo2Dbar2D_M-200_ctau-0p3mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-200_ctau-0p3mm_100kEvts_v2-abba3eb1a0edc71c057c11acf96c4b22/USER"
datasets["200_0p7"]="/StopStopbarTo2Dbar2D_M-200_ctau-0p7mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-200_ctau-0p7mm_100kEvts_v2-654f41373c9ddfca1862adb4b1fa443c/USER"
datasets["400_0p1"]="/StopStopbarTo2Dbar2D_M-400_ctau-0p1mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-400_ctau-0p1mm_100kEvts_v2-45fc77b60febfc690e82efd00feda1fb/USER"
datasets["400_0p3"]="/StopStopbarTo2Dbar2D_M-400_ctau-0p3mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-400_ctau-0p3mm_100kEvts_v2-2f9e35fe23e2fd3fa191a2a6d9a20b78/USER"
datasets["400_0p7"]="/StopStopbarTo2Dbar2D_M-400_ctau-0p7mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-400_ctau-0p7mm_100kEvts_v2-be18111a564090a75c0e233426e026da/USER"
datasets["600_0p1"]="/StopStopbarTo2Dbar2D_M-600_ctau-0p1mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-600_ctau-0p1mm_100kEvts_v2-9489fd7c2e41dc8e8edae5b7c2a39db2/USER"
datasets["600_0p3"]="/StopStopbarTo2Dbar2D_M-600_ctau-0p3mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-600_ctau-0p3mm_100kEvts_v2-8ac781fcbd5b200ee9a6c3ca506b163f/USER"
datasets["600_0p7"]="/StopStopbarTo2Dbar2D_M-600_ctau-0p7mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-600_ctau-0p7mm_100kEvts_v2-c09f509f5b04f5711f9c6895d6583e2e/USER"
datasets["800_0p1"]="/StopStopbarTo2Dbar2D_M-800_ctau-0p1mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-800_ctau-0p1mm_100kEvts_v2-cd65fd8b98eaf805cbbccd40e42e2813/USER"
datasets["800_0p3"]="/StopStopbarTo2Dbar2D_M-800_ctau-0p3mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-800_ctau-0p3mm_100kEvts_v2-f3e9c4f15a1eedc203314021bf3de9bc/USER"
datasets["800_0p7"]="/StopStopbarTo2Dbar2D_M-800_ctau-0p7mm_100kEvts_v2/brlopesd-CRAB3_MC_StopStopbarTo2Dbar2D_M-800_ctau-0p7mm_100kEvts_v2-87a3daa83d489ca77198f76c924bc463/USE"R

for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        ctauLabel="${ctau/./p}"

        key="${mStop}_${ctauLabel}"
        dataset="${datasets[$key]}"

        echo "Sending CRAB step 2 for for mStop = ${mStop}, ctau = ${ctau}"
        echo "The dataset is ${dataset}"

        sed -e "s|ChangeMstop|$mStop|g" \
        -e "s|ChangeCtau|$ctauLabel|g" \
        -e "s|ChangeDataset|$dataset|g" \
        crab_step2_TEMPLATE.py > step2_mStop-${mStop}_ctau-${ctauLabel}_CRAB_step2.py

        crab submit -c step2_mStop-${mStop}_ctau-${ctauLabel}_CRAB_step2.py
    done
done
