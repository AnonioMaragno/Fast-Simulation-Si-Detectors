#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"


double TrovaTracklet(pHit* h1, pHit* h2);
double* RunningWindow(TH1D* histo);

void Reconstruction(){
    // Dichiarazione oggetti in cui salvare dati dal tree in input
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = new TClonesArray("pHit", 100);
    TClonesArray* ptrHitsL2 = new TClonesArray("pHit", 100);

    
    // Apertura file di input
    TFile infile("treeSmeared.root");

    // Apertura file di output
    TFile outfile("treeReconstructed.root", "RECREATE");

    //Generazione TNtupla
    TNtuple* ntuple = new TNtuple("nt", "Ntupla con dati ricostruiti", "flag:zrec:ztrue:mult");
    ntuple->SetDirectory(&outfile);
    
    // Lettura TTree  e branch
    TTree *tree = (TTree*) infile.Get("TOUT");
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
    double sigma = 53;

    //variabili utili alla ricostruzione
    static TH1D* histoTrack = new TH1D("histo", "Istrogramma di analisi", 100, -4*sigma, 4*sigma);
    static double* ptrZRec;
    double zRec;
    int flag;

    // loop sugli ingressi nel TTree
    for(int ev=0; ev<tree->GetEntries(); ev++){

        tree->GetEvent(ev);
        cout << "zVertex = " << zVertex << endl;
        cout << "multi = " << multi << endl;

        //costruzione tracklet
        int nEntriesL1 = ptrHitsL1->GetEntriesFast();
        int nEntriesL2 = ptrHitsL2->GetEntriesFast();

        for (int i = 0; i<nEntriesL1; i++){
            hitL1 = ( pHit* ) ptrHitsL1->At(i);
            double phiL1 = hitL1->GetPhi();
            for (int j = 0; j<nEntriesL2; i++){
                hitL2 = ( pHit* ) ptrHitsL2->At(j);
                double phiL2 = hitL2->GetPhi(); 
                if (abs(phiL1-phiL2)<dPhi){                         //controllo se sto nel delta Phi max
                    double z_track = TrovaTracklet(hitL1, hitL2);
                    histoTrack->Fill(z_track);
                }

            }
        }
        ptrZRec = RunningWindow(histoTrack);
        // se non sono riuscito a ricsotruire il vertice uso una flag e zRec messo a un valore standard
        if (ptrZRec == nullptr){
            flag = 0;
            zRec = 200.0;
        }
        else{
            zRec = *ptrZRec;
            flag = 1;
        }
        ntuple->Fill(flag, zRec, zVertex, multi);
    }

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

double *RunningWindow(TH1D *histo)
{
    double* ptrZ;
    int binMax = 12;
    double rslt = histo->GetBinCenter(12);
    ptrZ = &rslt;
    return ptrZ;
}
