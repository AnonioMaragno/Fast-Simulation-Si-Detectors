#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"

using std::string;

// const int kMeanNoise = 5;

void PointSmearing(pHit* hit);
void Noise(TClonesArray &hits, int muNoise, Layer lay, TString eventID);

void Smearing(const bool enableNoise = true, const int kMeanNoise = 3) {

    cout << "----------------------------------" << endl;
    cout << "-------- SMEARING --------------" << endl;  
    cout << "----------------------------------" << endl << endl;

    // Dichiarazione oggetti in cui salvare dati dal tree in input
    TString evID; 
    TString* evIDptr = 0;  
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = 0;
    TClonesArray* ptrHitsL2 = 0;

    // Apertura file di input
    TFile hfile1("treeSimulated.root");

    // Apertura file di output
    TFile hfile2("treeSmeared.root", "RECREATE");
    
    // Lettura TTree  e branch
    TTree *treeIn = (TTree*) hfile1.Get("T");
    
    // Definizione degli indirizzi per la lettura dei dati su ttree
    treeIn->SetBranchAddress("eventID", &evIDptr);
    treeIn->SetBranchAddress("zVertex", &zVertex);
    treeIn->SetBranchAddress("Mult", &multi);
    treeIn->SetBranchAddress("HitsL1", &ptrHitsL1);
    treeIn->SetBranchAddress("HitsL2", &ptrHitsL2);

    // Generazione nuovo Tree
    TTree *treeOut = new TTree("TOUT","TTree con 5 branches");
    treeOut->SetDirectory(&hfile2);
    // Dichiarazione dei 5 branch del TTree
    treeOut->Branch("eventID", &evID);
    treeOut->Branch("zVertex", &zVertex);
    treeOut->Branch("Mult", &multi);
    treeOut->Branch("HitsL1", &ptrHitsL1);
    treeOut->Branch("HitsL2", &ptrHitsL2);

    // Variabili di comodo
    int muNoise1, muNoise2;
    int size1, size2;
    pHit* pointL1;
    pHit* pointL2;
    
    TClonesArray &hits1 = *ptrHitsL1;
    TClonesArray &hits2 = *ptrHitsL2;

    // Variabili di debug
    int noiseCountL1 = 0;
    int noiseCountL2 = 0;

    // Progress bar
    int barWidth = 70;

    std::cout << "[";
    for (int j = 0; j < barWidth; ++j) { std::cout << " ";}
    std::cout << "] 0 %\r" << std::flush;

    int nEntries = treeIn->GetEntries();

    // Loop sugli ingressi nel TTree
    for(int ev=0; ev<nEntries; ev++){

        treeIn->GetEvent(ev);
        evID = *evIDptr;

        size1 = ptrHitsL1->GetEntriesFast();
        for (int i = 0; i < size1; i++) {
            pointL1 = (pHit*) ptrHitsL1->At(i);
            PointSmearing(pointL1);
        }

        size2 = ptrHitsL2->GetEntriesFast();
        for (int j = 0; j < size2; j++) {
            pointL2 = (pHit*) ptrHitsL2->At(j);
            PointSmearing(pointL2);
        }
        
        if (enableNoise){
            muNoise1 = gRandom->Poisson(kMeanNoise); // aggiunge un numero di punti di noise estratto da una distribuzione poissoniana
            noiseCountL1 += muNoise1;
            muNoise2 = gRandom->Poisson(kMeanNoise);
            noiseCountL2 += muNoise2;

            if (muNoise1 > 0){
                Noise(hits1, muNoise1, Layer::L1, evID);
            }
            if (muNoise2 > 0){
                Noise(hits2, muNoise2, Layer::L2, evID);
            }
        }
        

        if (ev % 100 == 0 || ev == nEntries-1){
            // ------------- PROGRESS BAR -------------- //
            float progress = (float) (ev+1.) / nEntries;
            
            
            std::cout << "[";
            int pos = barWidth * progress;
            for (int j = 0; j < barWidth; ++j) {
                if (j < pos) std::cout << "=";
                else if (j == pos) std::cout << ">";
                else std::cout << " ";
            }
            // \r riporta il cursore all'inizio, std::flush forza la stampa immediata
            std::cout << "] " << int(progress * 100.0) << " %\r" << std::flush;
        }

        treeOut->Fill();

    }

    std::cout << std::endl;

    cout << "\nPUNTI DI NOISE CREATI SU L1: " << noiseCountL1 << endl;
    cout << "PUNTI DI NOISE CREATI SU L2: " << noiseCountL2 << endl;

    hfile2.Write();

    if (ptrHitsL1){ delete ptrHitsL1; ptrHitsL1 = nullptr;}
    if (ptrHitsL2){ delete ptrHitsL2; ptrHitsL2 = nullptr;}
    if (evIDptr){ delete evIDptr; evIDptr = nullptr;}

    hfile1.Close();
    hfile2.Close();

}

void PointSmearing(pHit* hit) {
    
    double z = hit->GetZ();
    z = z + gRandom->Gaus(0,0.12);
    hit->SetZ(z);

    double phi = hit->GetPhi();
    double R = hit->GetR();
    phi = phi + gRandom->Gaus(0,0.03)/R;
    hit->SetPhi(phi);

}

void Noise(TClonesArray &hits, int muNoise, Layer lay, TString eventID) {
    // Genera muNoise punti di noise in modo casuale sul layer
    for (int i = 0; i<muNoise; i++) {
        double phi = gRandom->Uniform(0,2*acos(-1));
        double z = gRandom->Uniform(-135,135);

        int memPos = hits.GetEntriesFast();
        new(hits[memPos]) pHit(z, phi, lay, -i, eventID);  // aggiunge il punto di noise alla fine del TClonesArray di hit del layer
    }

}
