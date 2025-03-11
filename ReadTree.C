#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"

void ReadTree() {

    // Dichiarazione oggetti in cui salvare dati dal tree in input
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = new TClonesArray("pHit", 100);
    TClonesArray* ptrHitsL2 = new TClonesArray("pHit", 100);

    
    // Apertura file di input
    TFile hfile("treeSimulated.root");
    
    // Lettura TTree  e branch
    TTree *treeIn = (TTree*) hfile.Get("T");
    TBranch *bZVert = treeIn->GetBranch("zVertex");
    TBranch *bMult = treeIn->GetBranch("Mult");
    TBranch *bHitsL1 = treeIn->GetBranch("HitsL1");
    TBranch *bHitsL2 = treeIn->GetBranch("HitsL2");
  
    // Definizione degli indirizzi per la lettura dei dati su ttree
    bZVert->SetAddress(&zVertex);
    bMult->SetAddress(&multi);
    bHitsL1->SetAddress(&ptrHitsL1);
    bHitsL2->SetAddress(&ptrHitsL2);

    // Variabili di comodo
    double muNoise1, muNoise2;
    int size1, size2;
    pHit* pointL1;
    pHit* pointL2;
    TClonesArray &hits1 = *ptrHitsL1;
    TClonesArray &hits2 = *ptrHitsL2;

    // loop sugli ingressi nel TTree
    for(int ev=0; ev<treeIn->GetEntries(); ev++){

        treeIn->GetEvent(ev);
        cout << "zVertex = " << zVertex << endl;
        cout << "multi = " << multi << endl;

        size1 = ptrHitsL1->GetEntriesFast();
        for (int i = 0; i < size1; i++) {
            pointL1 = (pHit*) ptrHitsL1->At(i);
            Smearing(pointL1);
        }

        size2 = ptrHitsL2->GetEntriesFast();
        for (int i = 0; i < size1; i++) {
            pointL2 = (pHit*) ptrHitsL2->At(i);
            Smearing(pointL2);
        }
        
        muNoise1 = gRandom->Poisson(5);
        muNoise2 = gRandom->Poisson(5);
        Noise(hits1, size1, muNoise1, Layer::L1, ev);
        Noise(hits2, size2, muNoise2, Layer::L2, ev);
        
        


    }


    hfile.Write();
    hfile.Close();
    



}

void Smearing(pHit* hit) {
    
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
        hit->SetZ(gRandom->Uniform(-13.5,13.5));
        hit->SetPhi(gRandom->Uniform(0,2*acos(-1)));
    }
    

}
