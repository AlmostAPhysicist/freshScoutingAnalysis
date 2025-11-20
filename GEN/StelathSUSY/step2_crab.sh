#!/usr/bin/bash

# Parameter values for the loop
mStops=(300 500 700 900)
ctaus=(0.1 1 3 10)
Models=("SHH" "SYY")

declare -A datasets

datasets["SHH_300_0p1"]="/StealthSHH_mStop-300_mSo-75_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-300_mSo-75_ctau-0p1mm100kEvts_v1-e3d7aed73172d91c26431479a1354c70/USER"
datasets["SHH_300_10"]="/StealthSHH_mStop-300_mSo-75_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-300_mSo-75_ctau-10mm100kEvts_v1-fe9985ae447f46b3dbee5b751cca3f61/USER"
datasets["SHH_300_1"]="/StealthSHH_mStop-300_mSo-75_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-300_mSo-75_ctau-1mm100kEvts_v1-aa69c36d0367d2d1cdfe9adfc12359d5/USER"
datasets["SHH_300_3"]="/StealthSHH_mStop-300_mSo-75_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-300_mSo-75_ctau-3mm100kEvts_v1-e65bb68ae5084fdd052b2fe7a84cb41e/USER"
datasets["SHH_500_0p1"]="/StealthSHH_mStop-500_mSo-275_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-500_mSo-275_ctau-0p1mm100kEvts_v1-5dc02b8f2da951746f24f7dfcd608fad/USER"
datasets["SHH_500_10"]="/StealthSHH_mStop-500_mSo-275_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-500_mSo-275_ctau-10mm100kEvts_v1-b333a13972e3f1c38baa0519fe3566f4/USER"
datasets["SHH_500_1"]="/StealthSHH_mStop-500_mSo-275_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-500_mSo-275_ctau-1mm100kEvts_v1-6aae094be19b0fd2407e97d497d99ef7/USER"
datasets["SHH_500_3"]="/StealthSHH_mStop-500_mSo-275_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-500_mSo-275_ctau-3mm100kEvts_v1-e3c2160cab09e6f9341e9a6e227c5547/USER"
datasets["SHH_700_0p1"]="/StealthSHH_mStop-700_mSo-475_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-700_mSo-475_ctau-0p1mm100kEvts_v1-7ec3e242f200ebbcf80d9333af9e2c86/USER"
datasets["SHH_700_10"]="/StealthSHH_mStop-700_mSo-475_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-700_mSo-475_ctau-10mm100kEvts_v1-337d5a2f5510dd84866fe0c3271e9ba6/USER"
datasets["SHH_700_1"]="/StealthSHH_mStop-700_mSo-475_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-700_mSo-475_ctau-1mm100kEvts_v1-476d9804504b531099ced171adb91981/USER"
datasets["SHH_700_3"]="/StealthSHH_mStop-700_mSo-475_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-700_mSo-475_ctau-3mm100kEvts_v1-ea5151d6b04da2a631dd6348287eb90e/USER"
datasets["SHH_900_0p1"]="/StealthSHH_mStop-900_mSo-675_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-900_mSo-675_ctau-0p1mm100kEvts_v1-9f52b730de1cb8d84c658cf5f9915c22/USER"
datasets["SHH_900_10"]="/StealthSHH_mStop-900_mSo-675_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-900_mSo-675_ctau-10mm100kEvts_v1-4cf561330a21448a3e07cb0bdcc5dea5/USER"
datasets["SHH_900_1"]="/StealthSHH_mStop-900_mSo-675_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-900_mSo-675_ctau-1mm100kEvts_v1-fc6d9a23bf05f2d9762946433b6e8646/USER"
datasets["SHH_900_3"]="/StealthSHH_mStop-900_mSo-675_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSHH_mStop-900_mSo-675_ctau-3mm100kEvts_v1-4e900a58e33dc5415cefec6b7707def9/USER"
datasets["SYY_300_0p1"]="/StealthSYY_mStop-300_mSo-75_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-300_mSo-75_ctau-0p1mm100kEvts_v1-37d47d0d17de8ae039d89fdb313de466/USER"
datasets["SYY_300_10"]="/StealthSYY_mStop-300_mSo-75_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-300_mSo-75_ctau-10mm100kEvts_v1-d2d506ad79f370ba8706449594d53f7a/USER"
datasets["SYY_300_1"]="/StealthSYY_mStop-300_mSo-75_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-300_mSo-75_ctau-1mm100kEvts_v1-31695130509d0b8b79efda0129777088/USER"
datasets["SYY_300_3"]="/StealthSYY_mStop-300_mSo-75_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-300_mSo-75_ctau-3mm100kEvts_v1-1bbd3e31479c2cb4fe0ce2bde07e64f9/USER"
datasets["SYY_500_0p1"]="/StealthSYY_mStop-500_mSo-275_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-500_mSo-275_ctau-0p1mm100kEvts_v1-f69dc88e750ae2a2f80de9bc4d57be5b/USER"
datasets["SYY_500_10"]="/StealthSYY_mStop-500_mSo-275_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-500_mSo-275_ctau-10mm100kEvts_v1-c94315e6e720181253e4aa7a54070165/USER"
datasets["SYY_500_1"]="/StealthSYY_mStop-500_mSo-275_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-500_mSo-275_ctau-1mm100kEvts_v1-90735148c9fc24550f4fcefb5e88ca53/USER"
datasets["SYY_500_3"]="/StealthSYY_mStop-500_mSo-275_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-500_mSo-275_ctau-3mm100kEvts_v1-c5d8fb309ae58044324010799570e0d3/USER"
datasets["SYY_700_0p1"]="/StealthSYY_mStop-700_mSo-475_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-700_mSo-475_ctau-0p1mm100kEvts_v1-5b1eed6c3f85cf3461caa86678a66db7/USER"
datasets["SYY_700_10"]="/StealthSYY_mStop-700_mSo-475_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-700_mSo-475_ctau-10mm100kEvts_v1-7540d22f40045dfe70511cea2ccdadf1/USER"
datasets["SYY_700_1"]="/StealthSYY_mStop-700_mSo-475_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-700_mSo-475_ctau-1mm100kEvts_v1-9d3ee24d82c9ced367e25584896135c7/USER"
datasets["SYY_700_3"]="/StealthSYY_mStop-700_mSo-475_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-700_mSo-475_ctau-3mm100kEvts_v1-356d22c9a8947bfd8583145e5160cdd3/USER"
datasets["SYY_900_0p1"]="/StealthSYY_mStop-900_mSo-675_ctau-0p1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-900_mSo-675_ctau-0p1mm100kEvts_v1-00b4975d4d85924eac3a3271e9ddddda/USER"
datasets["SYY_900_10"]="/StealthSYY_mStop-900_mSo-675_ctau-10mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-900_mSo-675_ctau-10mm100kEvts_v1-8c12384efaad73e2bb41bc155b332167/USER"
datasets["SYY_900_1"]="/StealthSYY_mStop-900_mSo-675_ctau-1mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-900_mSo-675_ctau-1mm100kEvts_v1-1f8b451efe0065bc7314414780be4eaa/USER"
datasets["SYY_900_3"]="/StealthSYY_mStop-900_mSo-675_ctau-3mm100kEvts_v1/brlopesd-CRAB3_MC_StealthSYY_mStop-900_mSo-675_ctau-3mm100kEvts_v1-10a42c3b44e4e4b78ac13c655f12a9e6/USER"


for mStop in "${mStops[@]}"; do
    for ctau in "${ctaus[@]}"; do
        for Model in "${Models[@]}"; do
            ctauLabel="${ctau/./p}"
            #mSo=(100)
            mSo=$((mStop - 225)) # Boosted case: mSo = 100 GeV, Resolved case: mSo = mStop - 225 GeV

            key="${Model}_${mStop}_${ctauLabel}"
            dataset="${datasets[$key]}"

            echo "Sending CRAB step 2 for for mStop = ${mStop}, mSo = ${mSo}, ctau = ${ctau}"
            echo "The dataset is ${dataset}"

            sed -e "s|ChangeMstop|$mStop|g" \
            -e "s|ChangeMso|$mSo|g" \
            -e "s|ChangeCtau|$ctauLabel|g" \
            -e "s|ChangeModel|$Model|g" \
            -e "s|ChangeDataset|$dataset|g" \
            crab_step2_TEMPLATE.py > Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}_CRAB_step2.py

            crab submit -c Stealth${Model}_mStop-${mStop}_mSo-${mSo}_ctau-${ctauLabel}_CRAB_step2.py
        done
    done
done
