#include "TString.h"
#include "TSystem.h"


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
  //gSystem->CompileMacro("Smearing.C",opt.Data());
  gSystem->CompileMacro("Reconstruction.C",opt.Data());
  gSystem->CompileMacro("Efficiency.C",opt.Data());

}
