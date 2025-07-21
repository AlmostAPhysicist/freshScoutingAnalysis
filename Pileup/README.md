## Pileup files for reweighting

Files in this folder were produced in July, 2025. Centrally produced files are not available for 2024 data by now, so they were produced using the pileupCalc CMSSW tool. More information on the [TWiki](https://twiki.cern.ch/twiki/bin/viewauth/CMS/PileupJSONFileforData).

NB: certification files in `/eos/user/c/cmsdqm/www/CAF/certification/Collisions24/DCSOnly_JSONS/` are only available until Era D by the time of this writing. Will be updated when the other Eras are available.

## Example: workflow to get the 2024D era PU file:

First, use brilcalc to get the lumi info. To do that, source this:

```
source /cvmfs/cms-bril.cern.ch/cms-lumi-pog/brilws-docker/brilws-env
```

Then, generate the luminosity information csv with 
```
brilcalc lumi --xing -i /eos/user/c/cmsdqm/www/CAF/certification/Collisions24/DCSOnly_JSONS/Collisions24_13p6TeV_eraD_380306_380947_DCSOnly_TkPx.json -b "STABLE BEAMS" --normtag /cvmfs/cms-bril.cern.ch/cms-lumi-pog/Normtags/normtag_BRIL.json --xingTr 0.1 -o lumi_DCSONLY.csv
```

Note the BRIL normtag used. Normally, one would use the `normtag_PHYSICS.json`, but it is not yet available for Run3. The BRIL normtag is not as stable, and often changed by the BRIL group. Use this only until the PHYSICS normtag is done for Run3.

Then use it as input to get the pileup file with:
```
makePileupJSON.py lumi_DCSONLY.csv pileup_JSON.txt
```

Finally, use it on `pileupCalc.py` as:
```
pileupCalc.py -i GoldenJSON/2024D_Golden.json --inputLumiJSON pileup_JSON.txt --calcMode true --minBiasXsec 69200 --maxPileupBin 100 --numPileupBins 100 MyDataPileupHistogram.root
```