import ROOT
import numpy as np
doPercents = True

# Define LaTeX table header
latex_table = "\\begin{table}[htbp]\n"
latex_table += "\\centering\n"

processDict = {"StopM200cTau1" : ["/eos/cms/store/group/phys_exotica/DVScouting/StopStopbarTo2Dbar2D_M-200_CTau-1mm_Summer24_100k_v2/StopStopbarTo2Dbar2D_M-200_CTau-1mm_Tree_v8Scouting/250216_214549/StopStopbarTo2Dbar2D_M-200_CTau-1mm.root"],
               "StopM400cTau1" : ["/eos/cms/store/group/phys_exotica/DVScouting/StopStopbarTo2Dbar2D_M-400_CTau-1mm_Summer24_100k_v2/StopStopbarTo2Dbar2D_M-400_CTau-1mm_Tree_v8Scouting/250216_214615/StopStopbarTo2Dbar2D_M-400_CTau-1mm.root"],
               "StopM800cTau1": ["/eos/cms/store/group/phys_exotica/DVScouting/StopStopbarTo2Dbar2D_M-800_CTau-1mm_Summer24_100k_v2/StopStopbarTo2Dbar2D_M-800_CTau-1mm_Tree_v8Scouting/250216_214720/StopStopbarTo2Dbar2D_M-800_CTau-1mm.root"],
               "QCD" : ["/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-200to400_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-200to400_Tree_v8Scouting/250216_214304/QCD-4Jets_Bin-HT-200to400.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-400to600_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-400to600_Tree_v8Scouting/250216_214316/QCD-4Jets_Bin-HT-400to600.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-600to800_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-600to800_Tree_v8Scouting/250216_214328/QCD-4Jets_Bin-HT-600to800.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-800to1000_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-800to1000_Tree_v8Scouting/250216_214340/QCD-4Jets_Bin-HT-800to1000.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-1000to1200_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-1000to1200_Tree_v8Scouting/250216_214405/QCD-4Jets_Bin-HT-1000to1200.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-1200to1500_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-1200to1500_Tree_v8Scouting/250216_214417/QCD-4Jets_Bin-HT-1200to1500.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-1500to2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-1500to2000_Tree_v8Scouting/250216_214429/QCD-4Jets_Bin-HT-1500to2000.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/QCD-4Jets_Bin-HT-2000_TuneCP5_13p6TeV_madgraphMLM-pythia8/QCD-4Jets_Bin-HT-2000_Tree_v8Scouting/250216_214441/QCD-4Jets_Bin-HT-2000.root"],
               "ttbar": ["/eos/cms/store/group/phys_exotica/DVScouting/TTto4Q_TuneCP5_13p6TeV_powheg-pythia8/TTto4Q_Tree_v8Scouting/250216_214508/TTto4Q.root",
                        "/eos/cms/store/group/phys_exotica/DVScouting/TTtoLNu2Q_TuneCP5_13p6TeV_powheg-pythia8/TTtoLNu2Q_Tree_v8Scouting/250216_214458/TTtoLNu2Q.root"]}
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
            latex_table += "\\resizebox{\\textwidth}{!}{\\begin{tabular}{|l|"+len(bin_names)*"c|"+"}\n"
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
        if(doPercents):
            latex_table +=  f" & {weightSum[i]/weightSum[0]:.3f}"
        else:    
            latex_table +=  f" & {weightSum[i]:.3E} $\pm$ {np.sqrt(weightSquaredSum[i]):.3E}"
    latex_table += "\\\\\n"
    firstProcess = False

latex_table += "\\hline\n"
latex_table += "\\end{tabular}}\n"
latex_table += "\\caption{Cutflow table}\n"
latex_table += "\\label{tab:cutflow}\n"
latex_table += "\\end{table}"
print(latex_table)
