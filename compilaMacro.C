#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"


void compilaMacro(TString myopt="fast"){
  TString opt;
  if(myopt.Contains("force")){
    opt = "kfg";
  }
  else {
    opt = "kg";
  }
  gSystem->CompileMacro("pPoint.cpp",opt.Data());
  gSystem->CompileMacro("pHit.cpp",opt.Data());
  gSystem->CompileMacro("pEvent.cpp",opt.Data());
  gSystem->CompileMacro("Simulation.C",opt.Data());
  gSystem->CompileMacro("Smearing.C",opt.Data());
  gSystem->CompileMacro("Reconstruction.C",opt.Data());
  gSystem->CompileMacro("Efficiency.C",opt.Data());

  double wSize = 1.;
  double phis[] = {0.01, 0.03, 0.05, 0.07, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4};
  
  gROOT->ProcessLine("Simulation();");
  gROOT->ProcessLine("Smearing();");
  gROOT->ProcessLine("Reconstruction();");
  gROOT->ProcessLine("Efficiency();");

}
