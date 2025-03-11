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
    int zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = new TClonesArray("pHit", 100);
    TClonesArray* ptrHitsL2 = new TClonesArray("pHit", 100);
    
    // Apertura file di input
    TFile hfile("treeSimulated.root");
    
    // Lettura TTree  e branch
    TTree *tree = (TTree*) hfile.Get("T");
    TBranch *bZVert = tree->GetBranch("zVertex");
    TBranch *bMult = tree->GetBranch("Mult");
    TBranch *bHitsL1 = tree->GetBranch("HitsL1");
    TBranch *bHitsL2 = tree->GetBranch("HitsL2");
  
    // Definizione degli indirizzi per la lettura dei dati su ttree
    bZVert->SetAddress(&zVertex);
    bMult->SetAddress(&multi);
    bHitsL1->SetAddress(ptrHitsL1);
    bHitsL2->SetAddress(ptrHitsL2);

    // loop sugli ingressi nel TTree
    for(int ev=0; ev<tree->GetEntries(); ev++){
        tree->GetEvent(ev);
        cout << "zVertex = " << zVertex << endl;
        cout << "multi = " << multi << endl;
        

    }
    



}

void Smearing(pHit& hit) {
    
    double z = hit.GetZ();
    z = z + gRandom->Gaus(0,0.12);
    hit.SetZ(z);

    double phi = hit.GetPhi();
    double R = hit.GetR();
    phi = phi + gRandom->Gaus(0,0.03)/R;
    hit.SetPhi(phi);

}

void Noise(Layer lay) {

    

}
