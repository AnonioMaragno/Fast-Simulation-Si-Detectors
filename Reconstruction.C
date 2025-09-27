#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TMath.h"
#include "TLine.h"
#include "pEvent.h"
#include "TROOT.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"

// const double kwSize = 1;
// const double kPhiWindow = 0.15;
const bool shouldDraw = false;

double TrovaTracklet(pHit* h1, pHit* h2);
void RunningWindow(TH1D* histo, double wSize, double &zRec, int &flag);

void Reconstruction(const double kwSize = 1, const double kPhiWindow = 0.15){

    cout << "----------------------------------" << endl;
    cout << "-------- RECONSTRUCTION --------------" << endl;
    cout << "----------------------------------" << endl << endl;

    // Dichiarazione oggetti in cui salvare dati dal tree in input
    TString evID; 
    TString* evIDptr = 0;
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = 0;
    TClonesArray* ptrHitsL2 = 0;

    
    // Apertura file di input
    TFile infile("treeSimulated.root");         //da cambiare con treeSmeared

    // Apertura file di output
    TFile outfile("treeReconstructed.root", "RECREATE");

    //Generazione TNtupla
    TNtuple* ntuple = new TNtuple("nt", "Ntupla con dati ricostruiti", "flag:zrec:ztrue:mult");
    ntuple->SetDirectory(&outfile);
    
    // Lettura TTree  e branch
    TTree *tree = (TTree*) infile.Get("T");             //da cambiare col nome del tree smeared
  
    // Definizione degli indirizzi per la lettura dei dati su ttree
    tree->SetBranchAddress("eventID", &evIDptr);
    tree->SetBranchAddress("zVertex", &zVertex);
    tree->SetBranchAddress("Mult", &multi);
    tree->SetBranchAddress("HitsL1", &ptrHitsL1);
    tree->SetBranchAddress("HitsL2", &ptrHitsL2);

    //puntatori a hit a cui voglio accedere
    pHit* hitL1;
    pHit* hitL2; 

    //parametri di ricostruzione
    double dPhi = kPhiWindow;
    double initialGuess = 0;
    const double sigma = 53;

    //variabili utili alla ricostruzione (in histo salvo le z_tracklet su cui poi faccio l'analisi con running window)
    TH1D* histoTrack = new TH1D("histo", "Istrogramma di analisi", 100000, -5*sigma, 5*sigma); //non considero le z oltre 5 sigma

    double zRec;
    int flag; //per capire se l'evento è stato ricostruito o no

    // se non sono riuscito a ricsotruire il vertice uso una flag 0 e zRec messo a un valore standard

    //Canvas per debug
    TCanvas *c = new TCanvas("cDeb", "Canvas debug");
    int countRec = 0;
    vector <TString> nonREC;

    // loop sugli ingressi nel TTree per costruire TRACKLET e VERTEX REC
    for(int ev=0; ev<tree->GetEntries(); ev++){

        tree->GetEvent(ev);
        //cout << "\nEVENTO " << evIDptr->Data() << endl << endl;
        
        int nEntriesL1 = ptrHitsL1->GetEntriesFast();
        int nEntriesL2 = ptrHitsL2->GetEntriesFast();

        //gestione degli eventi che non hanno entries su L1 o L2 (in quel caso lo considero non ricostruito)
        if(nEntriesL1>0 && nEntriesL2>0){
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
            RunningWindow(histoTrack, kwSize, zRec, flag);
        }
        else {
            //cout << "Nessuna entry sui layer" << endl;  
            flag = 0;
            zRec = 2000.;
        }
        
        if (shouldDraw && !flag){
            gStyle->SetOptStat(11111111);
            histoTrack->SetTitle(Form("Evento con molt %d, nEntriesL1 %d, nEntriesL2 %d", multi, nEntriesL1, nEntriesL2));
            histoTrack->GetXaxis()->SetRangeUser(zVertex-1, zVertex+1);
            histoTrack->Draw();
            TLine* lzTrue = new TLine(zVertex, 0, zVertex, 1.1*histoTrack->GetMaximum());
            lzTrue->SetLineColor(kBlue);
            lzTrue->Draw("same");
            if (flag){
                TLine* lzRec = new TLine(zRec, 0, zRec, 1.1*histoTrack->GetMaximum());
                lzRec->SetLineColor(kRed);
                lzRec->Draw("same");
            }
            TString nomeFile = "./images/canvas_" + *evIDptr + ".jpg";
            c->SaveAs(nomeFile.Data());

        }

        if (flag) {
            countRec += 1;
        }
        else
        {
            nonREC.push_back(*evIDptr);
        }

        ntuple->Fill(flag, zRec, zVertex, multi);
        histoTrack->Reset(); //per riutilizzare lo stesso istogramma
        histoTrack->GetXaxis()->UnZoom();
    }

    cout << "\n EVENTI NON RICOSTRUITI: " << endl;

    for (auto evNON : nonREC){
        cout << evNON << endl;
    }

    cout << "\n EVENTI RICOSTRUITI: " << countRec << endl;
    cout << "EVENTI TOTALI: " << tree->GetEntries() << endl << endl;
    cout << "Efficienza tot.: " << ((double) countRec)/((double) tree->GetEntries()) << endl;

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

void RunningWindow(TH1D* histo, double wSize, double &zRec, int &flag)
{
    //passaggio da larghezza della running window in mm a larghezza in numero di bin
    double binSize = histo->GetBinWidth(1); 
    int wWidth = (int) (wSize / binSize);

    // inizializzazione delle variabili di comodo
    bool maxFound = true;
    double initial = 1;//histo->GetXaxis()->FindBin(guess) - wWidth;
    double partialSum = histo->Integral(initial, initial + wWidth - 1 );
    double max = partialSum;
    int binMax = wWidth; // qui viene salvato il bin dx della running window in cui c'è il max
    int extension = 0;  // con questa variabile vedo se estendere la running window

    //-----------ALGORITMO DI RUNNING WINDOW -------------

    int nbins = histo->GetNbinsX();

    //gestione di istogrammi a una entries
    if (histo->Integral() > 1){
        for (int i=initial+wWidth; i<=nbins; i++){
            // tolgo il valore di un bin e ne aggiungo uno alla fine
            partialSum = partialSum + histo->GetBinContent(i) - histo->GetBinContent(i-wWidth);
    
            if (partialSum > max){
                max = partialSum;
                binMax = i;
                maxFound = true;
                extension = 0;

            }
            // else if (partialSum == max && i-binMax <= wWidth){
            //     binMax = i;
            //     extension += 1;
            //     // cout << "ho usato l'estensione" << endl;
            //     maxFound = true;
            // }
            else if (partialSum-max == 0 && i-binMax > wWidth){
                maxFound = false;
            }
        }

    }
    else{
        maxFound = true;
        binMax = histo->GetMaximumBin();
    }
    

    static double rslt;
    if (maxFound){
        rslt = 0.0;
        for (int j=0; j<(wWidth+extension); j++){
            rslt += histo->GetBinContent(binMax-j) * histo->GetBinCenter(binMax-j);
        }
        rslt = rslt / (histo->Integral(binMax-wWidth-extension, binMax));
        flag = 1;
        zRec = rslt;       
    }
    else{
        // cout << "EVENTO NON RICOSTRUITO" << endl;
        flag = 0;
        zRec = 2000.0;   
    }
    
}
