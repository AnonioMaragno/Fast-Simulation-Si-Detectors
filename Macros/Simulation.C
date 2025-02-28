#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"


void Simulation() {


    // Prendo molteplicità e psedorapidità dal file kinem.root
    TFile *f = new TFile("kinem.root");
    TH1F *eta = (TH1F*)f->Get("heta");
    TH1F *mul = (TH1F*)f->Get("hmul");

    // Creo un tree
    TFile hfile("htree.root", "RECREATE");
    TTree *tree = new TTree("T","TTree con 3 branches");

    // Dichiarazione dei 2 branch del TTree
    tree->Branch("VertMult",&point.X,"X/D:Y:Z:mult/I");
    tree->Branch("Hits",&ptrhits);






}