#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"

void PointSmearing(pHit* hit);
void Noise(TClonesArray &hits, double size, double muNoise, Layer lay, double event);

void Smearing() {

    cout << "----------------------------------" << endl;
    cout << "-------- SMEARING --------------" << endl;
    cout << "----------------------------------" << endl << endl;

    // Dichiarazione oggetti in cui salvare dati dal tree in input
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = new TClonesArray("pHit", 100);
    TClonesArray* ptrHitsL2 = new TClonesArray("pHit", 100);

    // Apertura file di input
    TFile hfile1("treeSimulated.root");

    // Apertura file di output
    TFile hfile2("treeSmeared.root", "RECREATE");
    
    // Lettura TTree  e branch
    TTree *treeIn = (TTree*) hfile1.Get("T");
    TBranch *bZVert = treeIn->GetBranch("zVertex");
    TBranch *bMult = treeIn->GetBranch("Mult");
    TBranch *bHitsL1 = treeIn->GetBranch("HitsL1");
    TBranch *bHitsL2 = treeIn->GetBranch("HitsL2");
  
    // Definizione degli indirizzi per la lettura dei dati su ttree
    bZVert->SetAddress(&zVertex);
    bMult->SetAddress(&multi);
    bHitsL1->SetAddress(&ptrHitsL1);
    bHitsL2->SetAddress(&ptrHitsL2);

    // Generazione nuovo Tree
    TTree *treeOut = new TTree("TOUT","TTree con 4 branches");
    treeOut->SetDirectory(&hfile2);
    // Dichiarazione dei 4 branch del TTree
    treeOut->Branch("zVertex", &zVertex);
    treeOut->Branch("Mult", &multi);
    treeOut->Branch("HitsL1", &ptrHitsL1);
    treeOut->Branch("HitsL2", &ptrHitsL2);

    // Variabili di comodo
    double muNoise1, muNoise2;
    int size1, size2;
    pHit* pointL1;
    pHit* pointL2;
    TClonesArray &hits1 = *ptrHitsL1;
    TClonesArray &hits2 = *ptrHitsL2;

    //debug
    int noiseCountL1 = 0;
    int noiseCountL2 = 0;

    // loop sugli ingressi nel TTree
    for(int ev=0; ev<treeIn->GetEntries(); ev++){

        treeIn->GetEvent(ev);
        cout << "EVENTO NUMERO: " << ev << endl;
        cout << "\nzVertex = " << zVertex << endl;
        cout << "multi = " << multi << endl << endl;

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
        
        muNoise1 = gRandom->Poisson(5);
        noiseCountL1 += muNoise1;
        muNoise2 = gRandom->Poisson(5);
        noiseCountL2 += muNoise2;
        Noise(hits1, size1, muNoise1, Layer::L1, ev);
        Noise(hits2, size2, muNoise2, Layer::L2, ev);
        
        treeOut->Fill();

    }

    cout << "PUNTI DI NOISE CREATI SU L1: " << noiseCountL1 << endl;
    cout << "PUNTI DI NOISE CREATI SU L2: " << noiseCountL2 << endl;

    hfile2.Write();
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

void Noise(TClonesArray &hits, double size, double muNoise, Layer lay, double event) {
    
    for (int i = 0; i<muNoise; i++) {
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        new(hits[size+i]) pHit(x, y, z, lay, -i, event);
        pHit* hit = (pHit* ) hits[size+i];
        hit->SetZ(gRandom->Uniform(-135,135));
        hit->SetPhi(gRandom->Uniform(0,2*acos(-1)));
    }

}
