#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TMath.h"
#include "TLine.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"

double kwSize = 20;

double TrovaTracklet(pHit* h1, pHit* h2);
void RunningWindow(TH1D* histo, double wSize, double *ptrZ);

void Reconstruction(){
    // Dichiarazione oggetti in cui salvare dati dal tree in input
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = new TClonesArray("pHit", 100);
    TClonesArray* ptrHitsL2 = new TClonesArray("pHit", 100);

    
    // Apertura file di input
    TFile infile("treeSimulated.root");         //da cambiare con treeSmeared

    // Apertura file di output
    TFile outfile("treeReconstructed.root", "RECREATE");

    //Generazione TNtupla
    TNtuple* ntuple = new TNtuple("nt", "Ntupla con dati ricostruiti", "flag:zrec:ztrue:mult");
    ntuple->SetDirectory(&outfile);
    
    // Lettura TTree  e branch
    TTree *tree = (TTree*) infile.Get("T");             //da cambiare col nome del tree smeared
    TBranch *bZVert = tree->GetBranch("zVertex");
    TBranch *bMult = tree->GetBranch("Mult");
    TBranch *bHitsL1 = tree->GetBranch("HitsL1");
    TBranch *bHitsL2 = tree->GetBranch("HitsL2");
  
    // Definizione degli indirizzi per la lettura dei dati su ttree
    bZVert->SetAddress(&zVertex);
    bMult->SetAddress(&multi);
    bHitsL1->SetAddress(&ptrHitsL1);
    bHitsL2->SetAddress(&ptrHitsL2);

    //puntatori a hit a cui voglio accedere
    static pHit* hitL1;
    static pHit* hitL2; 

    //parametri di ricostruzione
    double dPhi = 0.15;
    const double sigma = 53;

    //variabili utili alla ricostruzione
    TH1D* histoTrack = new TH1D("histo", "Istrogramma di analisi", 100, -4*sigma, 4*sigma); //non considero le z oltre 4 sigma
    static double ptrZRec[2];
    double zRec;
    int flag; //per capire se l'evento è stato ricostruito o no

    //Canvas per debug
    TCanvas *c = new TCanvas("c1", "Canvas debug");
    int countRec = 0;

    // loop sugli ingressi nel TTree
    for(int ev=0; ev<tree->GetEntries(); ev++){

        tree->GetEvent(ev);
        cout << "\nEVENTO NUMERO = " << ev << endl << endl;
        

        //costruzione tracklet
        int nEntriesL1 = ptrHitsL1->GetEntriesFast();
        int nEntriesL2 = ptrHitsL2->GetEntriesFast();


        for (int i = 0; i<nEntriesL1; i++){
            // accedo alla phi della hit L1
            hitL1 = ( pHit* ) ptrHitsL1->At(i);
            double phiL1 = hitL1->GetPhi();
            for (int j = 0; j<nEntriesL2; j++){
                // accedo alla phi della hit L2
                hitL2 = ( pHit* ) ptrHitsL2->At(j);
                double phiL2 = hitL2->GetPhi(); 
                if (abs(phiL1-phiL2)<dPhi){                         //controllo se sto nel delta Phi max
                    double z_track = TrovaTracklet(hitL1, hitL2);
                    histoTrack->Fill(z_track);
                }

            }
        }
        RunningWindow(histoTrack, kwSize, ptrZRec);
        gStyle->SetOptStat(11111111);
        histoTrack->Draw();
        TLine* lzTrue = new TLine(zVertex, 0, zVertex, 1.1*histoTrack->GetMaximum());
        lzTrue->SetLineColor(kBlue);
        lzTrue->Draw("same");
        if (ptrZRec[0] == 1.){
            countRec += 1;
            TLine* lzRec = new TLine(ptrZRec[1], 0, ptrZRec[1], 1.1*histoTrack->GetMaximum());
            lzRec->SetLineColor(kRed);
            lzRec->Draw("same");

        }
        char testo[80];
        sprintf(testo, "./canvas_ev%i.jpg", ev);
        c->SaveAs(testo);
        
        // se non sono riuscito a ricsotruire il vertice uso una flag e zRec messo a un valore standard
        flag = ptrZRec[0];
        zRec = ptrZRec[1];

        ntuple->Fill(flag, zRec, zVertex, multi);
        histoTrack->Reset(); //per riutilizzare lo stesso istogramma
    }

    cout << "\n EVENTI RICOSTRUITI: " << countRec << endl;
    cout << "EVENTI TOTALI: " << tree->GetEntries() << endl;

    c->Close();
    ntuple->Write();

    infile.Close();
    outfile.Close();

}


//trova il pto di intersezione con l'asse z della retta per le due hit nel piano R-z 
double TrovaTracklet(pHit *h1, pHit *h2)
{
    double z1 = h1->GetZ();
    double z2 = h2->GetZ();
    double r1 = h1->GetR();
    double r2 = h2->GetR();
    double q = (r2*z1 - z2*r1) / (r2 - r1); 
    return q;
}

void RunningWindow(TH1D *histo, double wSize, double* ptrZ)
{
    //passaggio da larghezza della running window in mm a larghezza in numero di bin
    double binSize = histo->GetBinWidth(1); 
    int wWidth = (int) wSize / binSize;

    // inizializzazione delle variabili di comodo
    bool maxFound = true;
    double partialSum = histo->Integral(1, wWidth);
    double max = partialSum;
    int binMax = wWidth;
    int extension = 0;

    //algoritmo di running window
    int nbins = histo->GetNbinsX();
    if (histo->Integral() > 1){
        for (int i=wWidth+1; i<=nbins; i++){
            // tolgo il valore di un bin e ne aggiungo uno alla fine
            partialSum = partialSum + histo->GetBinContent(i) - histo->GetBinContent(i-wWidth);
    
            if (partialSum > max){
                max = partialSum;
                binMax = i;
                maxFound = true;
                extension = 0;

            }
            else if (partialSum == max && i-binMax <= wWidth){
                binMax = i;
                extension += 1;
                maxFound = true;
            }
            else if (abs(partialSum-max) <= 1 && i-binMax > wWidth){
                cout << "MAX SU DUE PICCHI DIVERSI" << endl;
                maxFound = false;
            }
        }

    }
    else{
        maxFound = true;
        binMax = histo->GetMaximumBin();
    }
    

    double rslt;
    if (maxFound){
        rslt = 0.0;
        for (int j=0; j<(wWidth+extension); j++){
            rslt += histo->GetBinContent(binMax-j) * histo->GetBinCenter(binMax-j);
        }
        rslt = rslt / (histo->Integral(binMax-wWidth-extension, binMax));
        ptrZ[0] = 1.;
        ptrZ[1] = rslt;       
    }
    else{
        cout << "EVENTO NON RICOSTRUITO" << endl;
        ptrZ[0] = 0.;
        ptrZ[1] = 200.0;   
    }
    
    return;
}
