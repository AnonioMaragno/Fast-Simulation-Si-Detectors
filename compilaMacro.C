#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include <iostream>

const bool kScan = false;


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
  //gSystem->CompileMacro("EventDisplay.C",opt.Data());
  gSystem->CompileMacro("Smearing.C",opt.Data());
  gSystem->CompileMacro("Reconstruction.C",opt.Data());
  gSystem->CompileMacro("Efficiency.C",opt.Data());
  gSystem->CompileMacro("scans.C",opt.Data());

  gROOT->ProcessLine("Simulation();");
  gROOT->ProcessLine("Smearing();");

  if (kScan){

    double wSize[] = {2., 2.1, 2.2, 2.3, 2.4, 2.5}; // in mm
    double phis[] = {0.005, 0.01, 0.015, 0.02, 0.025};

    //double wSize[] = {0.06, 0.1, 0.15, 0.2, 0.25, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1., 1.5, 2., 3., 5., 10.};
    //double phis[] = {0.02, 0.04, 0.06, 0.08, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 0.7, 1., 1.5, 2., 3.};


    const int nW = std::size(wSize);
    double effW[nW], rmsW[nW]; //risoluzione in micron

    const int nP = std::size(phis);
    double effP[nP], rmsP[nP]; 

    gROOT->ProcessLine(Form("scanProcess(%d, (double*) %p, 0.05, (double*) %p, (double*) %p, \"wSize\")", nW, wSize, effW, rmsW));
    gROOT->ProcessLine(Form("scanProcess(%d, (double*) %p, 2.3, (double*) %p, (double*) %p, \"dPhi\")", nP, phis, effP, rmsP));    

  }
  else{
    gROOT->ProcessLine("Reconstruction(2.3, 0.05);"); //dPhi ottimale 0.01 senza MS e 0.05 con MS
    gROOT->ProcessLine("Efficiency();");
  }

  //gROOT->ProcessLine("EventDisplay()");

}
