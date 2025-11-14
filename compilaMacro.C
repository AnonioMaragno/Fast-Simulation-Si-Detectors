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
  // gSystem->CompileMacro("Simulation.C",opt.Data());
  // gSystem->CompileMacro("EventDisplay.C",opt.Data());
  // gSystem->CompileMacro("Smearing.C",opt.Data());
  // gSystem->CompileMacro("Reconstruction.C",opt.Data());
  // gSystem->CompileMacro("Efficiency.C",opt.Data());


  //gROOT->ProcessLine("Simulation();");
  //gROOT->ProcessLine("Smearing();");


  double wSize[] = {0.06, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1., 1.5, 2., 3., 5., 10.};
  double phis[] = {0.02, 0.04, 0.06, 0.08, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.7, 1., 1.5, 2., 3.};
  
  TString recString = "Reconstruction(";

  for (auto wdw : wSize){
    gROOT->ProcessLine((recString + TString::Format("%.2f", wdw) + ", 0.05" + ");").Data());
    gROOT->ProcessLine("Efficiency();");  
  }

  for (auto phi : phis){
    gROOT->ProcessLine((recString + "1.5," + TString::Format("%.2f", phi) + ");").Data());
    gROOT->ProcessLine("Efficiency();");  
  }

  //gROOT->ProcessLine("Reconstruction();");
  //gROOT->ProcessLine("Efficiency();");
  //gROOT->ProcessLine("EventDisplay()");

}
