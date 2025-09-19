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
#include "TF1.h"
#include "TCanvas.h"
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
    const double zMin = -200;
    const double zMax = 200;
    const double residMin = -30;
    const double residMax = 30;
    const int multMin = 0;
    const int multMax = 70;

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
    TH2D *histo1 = new TH2D("hist1", "Histogram 1", 71, multMin-0.5, multMax+0.5, 427, residMin-0.5, residMax+0.5);
    TH2D *histo2 = new TH2D("hist2", "Histogram 2", 200, zMin, zMax, 427, residMin-0.5, residMax+0.5);
    double resid;

    for(int ev=0; ev<ntuple->GetEntries(); ev++){
        
        ntuple->GetEvent(ev);
        // qui zrec contiene il valore corrente di zrec; mult il corrente valore di mult, etc
        // cout << ev << ") " << "zTrue = " << zTrue << " zRec = " << zRec << endl;

        // Riempio histo1
        resid = zRec - zTrue;
        if (flag == 1) {
            histo1->Fill(mult, resid);
        }
        else {
            histo1->Fill(mult, residMin-1);
        }

        // Riempio histo2
        if (flag == 1) {
            histo2->Fill(zTrue, resid);
        }
        else {
            histo2->Fill(zTrue, residMin-1);
        }

    }
    histo1->Write();
    histo2->Write();

    // RESIDUI
    // Per qualsiasi molteplicità
    TH1D *hRes = histo1->ProjectionY("hRes");
    hRes->SetTitle("Residui");
    hRes->Write();
    // // Per ciascun valore di molteplicità
    // TH1D *hResMult1[68];
    // char nameResMult1[80];
    // char titleResMult1[100];
    // for(int i=3; i<71; i++) {
    //     sprintf(nameResMult1, "hRM%i", i);
    //     sprintf(titleResMult1, "Residui con molteplicità %i", i);
    //     hResMult1[i] = histo1->ProjectionY(nameResMult1, histo1->GetXaxis()->FindBin(i-0.499), histo1->GetXaxis()->FindBin(i+0.499));
    //     hResMult1[i]->SetTitle(titleResMult1);
    //     hResMult1[i]->Write();
    // }

    // Per intervalli di molteplicità
    int nMult[] = {3, 4, 5, 7, 10, 13, 19, 28, 37, 46, 55, 64};
    TH1D *hResMult[12];
    char name[80];
    char title[100];
    for(int i=0; i<12; i++) {
        if(nMult[i] < 6){    
            sprintf(name, "hResMult%i", nMult[i]);
            sprintf(title, "Residui con molteplicità %i +/- 0.5", nMult[i]);
            hResMult[i] = histo1->ProjectionY(name, histo1->GetXaxis()->FindBin(nMult[i]-0.499), histo1->GetXaxis()->FindBin(nMult[i]+0.499));
            hResMult[i]->SetTitle(title);
            hResMult[i]->Write();
        } else if(nMult[i] > 6 && nMult[i] < 14) {
            sprintf(name, "hResMult%i", nMult[i]);
            sprintf(title, "Residui con molteplicità %i +/- 1.5", nMult[i]);
            hResMult[i] = histo1->ProjectionY(name, histo1->GetXaxis()->FindBin(nMult[i]-1.499), histo1->GetXaxis()->FindBin(nMult[i]+1.499));
            hResMult[i]->SetTitle(title);
            hResMult[i]->Write();
        } else {
            sprintf(name, "hResMult%i", nMult[i]);
            sprintf(title, "Residui con molteplicità %i +/- 4.5", nMult[i]);
            hResMult[i] = histo1->ProjectionY(name, histo1->GetXaxis()->FindBin(nMult[i]-4.499), histo1->GetXaxis()->FindBin(nMult[i]+4.499));
            hResMult[i]->SetTitle(title);
            hResMult[i]->Write();
        }
    }

    // Efficienza vs molteplicità
    double effMult[12];
    double multArray[12];
    double num;
    double den;
    for(int i=0; i<12; i++) {
        num = hResMult[i]->Integral();
        den = hResMult[i]->GetEntries();
        if(den != 0) {
            effMult[i] = num/den;
        }
        else {
            effMult[i] = 0;
        }
        multArray[i] = nMult[i]; 
    }
    TCanvas* cEffMult = new TCanvas("cEffMult", "cEffMult", 0, 1, 600, 400);
    cEffMult->cd();
    TGraph *gEffMult = new TGraph(12, multArray, effMult);
    gEffMult->SetTitle("Efficiency vs Multiplicity");
    gEffMult->GetXaxis()->SetTitle("Multiplicity");
    gEffMult->GetYaxis()->SetTitle("Efficiency");
    gEffMult->SetMarkerSize(0.8);
    gEffMult->SetMarkerStyle(21);
    gEffMult->SetMarkerColor(kRed);
    gEffMult->Draw("ALP");
    gEffMult->Write();

    // Risoluzione vs molteplicità
    TF1 *fResMult[12];
    double resolutionMult[12];
    double errResolutionMult[12];
    for (int i = 0; i<12; i++) {
        sprintf(name, "fResMult%i", nMult[i]);
        fResMult[i] = new TF1(name, "gaus", -5, 5);
        fResMult[i]->SetParameters(hResMult[i]->GetEntries()/25, 0, 1.5);
        hResMult[i]->Fit(fResMult[i], "R+");
        resolutionMult[i] = fResMult[i]->GetParameter(2);
        errResolutionMult[i] = fResMult[i]->GetParError(2);
    }
    TGraph *gResolMult = new TGraph(12, multArray, resolutionMult);
    gResolMult->Write();

    // Efficienza vs zTrue
    double nZTrue[] = {-175, -125, -75, -37.5, -12.5, 12.5, 37.5, 75, 125, 175};
    TH1D *hResZTrue[10];
    for(int i=0; i<10; i++) {
        if(abs(nZTrue[i]) < 38){
            sprintf(name, "hResZTrue%i", i);
            sprintf(title, "Residui con vertice %f +/- 12.5", nZTrue[i]);
            hResZTrue[i] = histo2->ProjectionY(name, histo2->GetXaxis()->FindBin(nZTrue[i]-12.5), histo2->GetXaxis()->FindBin(nZTrue[i]+12.5));
            hResZTrue[i]->SetTitle(title);
            hResZTrue[i]->Write();
        } else {
            sprintf(name, "hResZTrue%i", i);
            sprintf(title, "Residui con vertice %f +/- 25", nZTrue[i]);
            hResZTrue[i] = histo2->ProjectionY(name, histo2->GetXaxis()->FindBin(nZTrue[i]-25), histo2->GetXaxis()->FindBin(nZTrue[i]+25));
            hResZTrue[i]->SetTitle(title);
            hResZTrue[i]->Write();
        }
    }
    double effZTrue[10];
    for(int i=0; i<10; i++) {
        num = hResZTrue[i]->Integral();
        den = hResZTrue[i]->GetEntries();
        if(den != 0) {
            effZTrue[i] = num/den;
        }
        else {
            effZTrue[i] = 0;
        } 
    }
    TCanvas* cEffZTrue = new TCanvas("cEffZTrue", "cEffZTrue", 0, 1, 600, 400);
    cEffZTrue->cd();
    TGraph *gEffZTrue = new TGraph(10, nZTrue, effZTrue);
    gEffZTrue->SetTitle("Efficiency vs Z_{vert}");
    gEffZTrue->GetXaxis()->SetTitle("Z_{true}");
    gEffZTrue->GetYaxis()->SetTitle("Efficiency");
    gEffZTrue->SetMarkerSize(0.8);
    gEffZTrue->SetMarkerStyle(21);
    gEffZTrue->SetMarkerColor(kRed);
    gEffZTrue->Draw("ALP");
    gEffZTrue->Write();

    // // Risoluzione vs zTrue
    // TF1 *fResZTrue[10];
    // double resolutionZTrue[10];
    // double errResolutionZTrue[10];
    // for (int i = 0; i<10; i++) {
    //     sprintf(name, "fResZTrue%i", nZTrue[i]);
    //     fResZTrue[i] = new TF1(name, "gaus", -1000, 1000);
    //     fResZTrue[i]->SetParameters(ntuple->GetEntries()/500, 0, 100);
    //     hResZTrue[i]->Fit(fResZTrue[i], "R+");
    //     resolutionZTrue[i] = fResZTrue[i]->GetParameter(2);
    //     errResolutionZTrue[i] = fResZTrue[i]->GetParError(2);
    // }
    // TGraph *gResolZTrue = new TGraph(10, multArray, resolutionZTrue);
    // gResolZTrue->Write();




    
    
    




    fileIn.Close();
    fileOut.Close();




}