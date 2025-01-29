import ROOT
import numpy as np

# Define LaTeX table header
latex_table = "\\begin{table}[htbp]\n"
latex_table += "\\centering\n"

processDict = {"StopToDDbar" : ["tree.root"], "QCD" : ["/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-200to400_Summer24/QCD-4Jets_Bin-HT-200to400_Summer24_Tree_v3Weights/250129_092208/0000/QCD-4Jets_HT-200to400.root","/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-400to600_Summer24/QCD-4Jets_Bin-HT-400to600_Summer24_Tree_v3Weights/250128_122223/0000/QCD-4Jets_HT-400to600.root","/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-600to800_Summer24_Tree_v3Weights/250128_122233/0000/QCD-4Jets_HT-600to800.root","/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-800to1000_Summer24_Tree_v3Weights/250128_122244/0000/QCD-4Jets_HT-800to1000.root","/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-1000to1200_Summer24/QCD-4Jets_Bin-HT-1000to1200_Summer24_Tree_v3Weights/250128_122256/0000/QCD-4Jets_HT-1000to1200.root","/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-1200to1500_Summer24_Tree_v3Weights/250128_122307/0000/QCD-4Jets_HT-1200to1500.root","/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-1500to2000_Summer24_Tree_v3Weights/250128_122322/0000/QCD-4Jets_HT-1500to2000.root","/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-2000_Summer24_Tree_v3Weights/250128_122333/0000/QCD-4Jets_HT-2000.root"], "ttbar": ["/eos/cms/store/group/phys_exotica/DVScouting/TTtoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/TTtoLNu2Q_Summer24_Tree_v3Weights/250128_122355/0000/TTtoLNu2Q.root","/eos/cms/store/group/phys_exotica/DVScouting/TTto4Q_TuneCP5_13p6TeV_powheg-pythia8/TTto4Q_Summer24_Tree_v3Weights/250128_122346/0000/TTto4Q.root"]}
firstProcess = True
for process, rootFiles in processDict.items():
    firstFile = True
    weightSum = []
    weightSquaredSum = []
    for rootFile in rootFiles:
        f = ROOT.TFile.Open(rootFile, "READ")
        h_weights = f.Get("scoutingTree/weights")
        h_weightsSquared = f.Get("scoutingTree/weightsSquared")
        h_genWeights = f.Get("scoutingTree/genWeights")
        
        # Extract content
        n_bins = h_weights.GetNbinsX()
        weights = np.array([h_weights.GetBinContent(i) for i in range(1, n_bins + 1)])
        bin_names = np.array([h_weights.GetXaxis().GetLabels().At(i-1).GetName() for i in range(1, n_bins + 1)])
        weightsSquared = np.array([h_weightsSquared.GetBinContent(i) for i in range(1, n_bins + 1)])
        genWeights = np.array([h_genWeights.GetBinContent(i) for i in range(1, n_bins + 1)])
        
        if(firstFile and firstProcess):
            latex_table += "\\begin{tabular}{|l|"+len(bin_names)*"c|"+"}\n"
            latex_table += "\\hline\n"
            latex_table += "Process "
            for name in bin_names:
                latex_table += "& {} ".format(name)
                weightSum.append(0)
                weightSquaredSum.append(0)
            latex_table += "\\\\\n"
            latex_table += "\\hline\n"
            firstFile = False
        elif(firstFile):
            for name in bin_names:
                weightSum.append(0)
                weightSquaredSum.append(0)
            firstFile = False
                
        for i in range(len(weights)):
            weightSum[i] += weights[i] / genWeights[0]
            weightSquaredSum[i] += weightsSquared[i] / pow(genWeights[0],2)
    latex_table +=  f"{process}"
    for i in range(len(weightSum)):
        latex_table +=  f" & {weightSum[i]:.3E} $\pm$ {np.sqrt(weightSquaredSum[i]):.3E}"
    latex_table += "\\\\\n"
    firstProcess = False

latex_table += "\\hline\n"
latex_table += "\\end{tabular}\n"
latex_table += "\\caption{Cutflow table}\n"
latex_table += "\\label{tab:cutflow}\n"
latex_table += "\\end{table}"
print(latex_table)
