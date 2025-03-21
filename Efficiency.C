#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TGraph.h"
using std::to_string;

const int nHist = 10;


void Efficiency() {

    cout << "----------------------------------" << endl;
    cout << "-------- EFFICIENCY --------------" << endl;
    cout << "----------------------------------" << endl << endl;

    // Lettura file input e ntuple
    TFile fileIn("treeReconstructed.root");
    float flag, zRec, zTrue, mult;
    TNtuple *ntuple = (TNtuple*) fileIn.Get("nt");
    ntuple->SetBranchAddress("flag", &flag);
    ntuple->SetBranchAddress("zrec", &zRec);
    ntuple->SetBranchAddress("ztrue", &zTrue);
    ntuple->SetBranchAddress("mult", &mult);

    // Creazione file output in cui salvare gli istogrammi
    TFile fileOut("histograms.root", "RECREATE");

    // Variabili ausiliarie
    const double zMin = -4*53;
    const double zMax = 4*53;
    const double residMin = -30;
    const double residMax = 30;

    // Creazione istogrammi
    /*TH1D *vHist[nHist];
    int multArray[nHist]={3,5,10,15,20,30,40,50,60,70};
    char name[30];
    char title[50];
    for (int i=0; i<nHist; i++){
        snprintf(name,15,"h%d",smm[i]);
        snprintf(title,50,"Risoluzione con molteplicità %d",smm[i]);
        vHist[i] = new TH1D(name, title, 100, residMin, residMax);
    }*/
    TH2D *histo = new TH2D("hist", "Histogram", 70, 0., 70., 50, residMin, residMax);
    double resid;

    for(int ev=0; ev<ntuple->GetEntries(); ev++){
        
        ntuple->GetEvent(ev);
        // qui zrec contiene il valore corrente di zrec; mult il corrente valore di mult, etc
        cout << ev << ") " << "zTrue = " << zTrue << " zRec = " << zRec << endl;

        // Riempio histo
        resid = zRec - zTrue;
        if (flag == 1) {
            histo->Fill(mult, resid);

        }
        else {
            histo->Fill(mult, residMin-1);
        }
    }

    TH1D *vt[14];
    char name[80];
    double eff[14];
    double multArray[14];
    double num;
    double den;

    for(int i=0; i<14; i++) {
        
        //snprintf(name, "h%i", name[i]);
        vt[i] = histo->ProjectionY("h", 5*i, 5*(i+1));
        num = vt[i]->Integral();
        den = vt[i]->GetEntries();
        eff[i] = num/den;
        multArray[i] = 5*i;
        vt[i]->Write();
    }

    TGraph *EffMult = new TGraph(14,eff,multArray);



    EffMult->Write();
    histo->Write();
    




    fileIn.Close();
    fileOut.Close();




}