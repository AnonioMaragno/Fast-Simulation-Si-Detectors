#include "pEvent.h"
#include <TGeoManager.h>
#include <TPolyLine3D.h>
#include <TRandom3.h>
#include <TCanvas.h>
#include <TButton.h>
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <iostream>

TGeoManager* geom = nullptr;
TCanvas* canvas = nullptr;
TTree* tree = nullptr;
int currentEvent = 0;

std::vector<TPolyLine3D*> tracks;

// Funzione per disegnare una traiettoria
TPolyLine3D* CreateTrack(double x0, double y0, double z0, double x1, double y1, double z1) {

    TPolyLine3D* track = new TPolyLine3D(2);
    track->SetPoint(0, x0, y0, z0);
    track->SetPoint(1, x1, y1, z1);
    track->SetLineColor(kGreen+2);
    track->SetLineWidth(2);
    return track;
}

// Costruisce i cilindri
void BuildGeometry() {
    TGeoMaterial* matVacuum = new TGeoMaterial("Vacuum", 0, 0, 0);
    TGeoMedium* vacuum = new TGeoMedium("Vacuum", 1, matVacuum);

    TGeoMaterial* matSi = new TGeoMaterial("Silicon", 28.0855, 14, 2.33);
    TGeoMedium* silicon = new TGeoMedium("Silicon", 2, matSi);

    TGeoMaterial* matBe = new TGeoMaterial("Beryllium", 9.0122, 4, 1.85);
    TGeoMedium* beryllium = new TGeoMedium("Beryllium", 3, matBe);

    TGeoVolume* top = geom->MakeBox("TOP", vacuum, 100, 100, 100);
    geom->SetTopVolume(top);

    TGeoVolume* bp = geom->MakeTube("BeamPipe", beryllium, 3, 3.02, 100);
    bp->SetLineColor(kYellow-10);
    top->AddNode(bp, 1);

    TGeoVolume* lay1 = geom->MakeTube("Layer1", silicon, 4.0, 4.02, 13.5);
    lay1->SetLineColor(kBlue-3);
    top->AddNode(lay1, 1);

    TGeoVolume* lay2 = geom->MakeTube("Layer2", silicon, 7.0, 7.02, 13.5);
    lay2->SetLineColor(kRed-3);
    top->AddNode(lay2, 1);

    geom->CloseGeometry();
}

// Pulisce le tracce disegnate
void ClearTracks() {
    for (auto* t : tracks) {
        t->Delete();
    }
    tracks.clear();
}

// Disegna un evento
void DrawEvent(TTree* tree, TString eventID) {
    ClearTracks();

    // Dichiarazione oggetti in cui salvare dati dal tree in input
    TString evID; 
    TString* evIDptr = 0;  
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = new TClonesArray("pHit", 100);
    TClonesArray* ptrHitsL2 = new TClonesArray("pHit", 100);
    TClonesArray* ptrHitsBP = new TClonesArray("pHit", 100);

    // Definizione degli indirizzi per la lettura dei dati su ttree
    tree->SetBranchAddress("eventID", &evIDptr);
    tree->SetBranchAddress("zVertex", &zVertex);
    tree->SetBranchAddress("Mult", &multi);
    tree->SetBranchAddress("HitsL1", &ptrHitsL1);
    tree->SetBranchAddress("HitsL2", &ptrHitsL2);
    tree->SetBranchAddress("HitsBP", &ptrHitsBP);

    for (int ev=0; ev<tree->GetEntries(); ev++) {
        tree->GetEvent(ev);
        if (evIDptr->CompareTo(eventID) == 0) {

            cout << "Ho trovato l'evento che voglio disegnare" << endl;

            // Variabili di comodo
            pHit* hitBP = nullptr;
            pHit* hitL1 = nullptr;
            pHit* hitL2 = nullptr;
            double xV = 0;
            double yV = 0;
            double zV = 0;
            double xBP = 0;
            double yBP = 0;
            double zBP = 0;
            double xL1 = 0;
            double yL1 = 0;
            double zL1 = 0;
            double xL2 = 0;
            double yL2 = 0;
            double zL2 = 0;
            bool isL1on = false;
            bool isL2on = false;

            xV = 0;
            yV = 0;
            zV = zVertex;
            // cout << "Coordinate del vertice = " << "(" << xV << "," << yV << "," << zV << ")" << endl;

            // cout << "Puntatore alle hit di BP = " << ptrHitsBP << endl;
            // cout << "Numero di hit sulla BP = " << ptrHitsBP->GetEntriesFast() << endl;
            for(int i=0; i<ptrHitsBP->GetEntriesFast(); i++){
                
                hitBP = (pHit* )ptrHitsBP->At(i);                
                // cout << "fID beam Pipe = " << hitBP->GetId() << endl;
                xBP = hitBP->GetX();
                yBP = hitBP->GetY();
                zBP = hitBP->GetZ();
                TPolyLine3D* track = CreateTrack(xV/10, yV/10, zV/10, xBP/10, yBP/10, zBP/10);  // Converto le coordinate da mm in cm (utilizzati per definire la geometria)
                track->Draw();
                tracks.push_back(track);

                for (int j=0; j<ptrHitsL1->GetEntriesFast(); j++) {
                    
                    hitL1 = (pHit* )ptrHitsL1->At(j);
                    // cout << "fID layer 1 = " << hitL1->GetId() << endl;
                    if (hitBP->GetId().CompareTo(hitL1->GetId()) == 0) {
                        // cout << "Ho trovato fID L1 uguale a fID BP" << endl;
                        xL1 = hitL1->GetX();
                        yL1 = hitL1->GetY();
                        zL1 = hitL1->GetZ();
                        TPolyLine3D* track = CreateTrack(xV/10, yV/10, zV/10, xL1/10, yL1/10, zL1/10);  // Converto le coordinate da mm in cm (utilizzati per definire la geometria)
                        track->Draw();
                        tracks.push_back(track);

                        for (int k=0; k<ptrHitsL2->GetEntriesFast(); k++) {

                            hitL2 = (pHit* )ptrHitsL2->At(k);
                            // cout << "fID layer 2 = " << hitL2->GetId() << endl;
                            if (hitL1->GetId().CompareTo(hitL2->GetId()) == 0) {
                                // cout << "Ho trovato fID L2 uguale a fID L1" << endl;
                                xL2 = hitL2->GetX();
                                yL2 = hitL2->GetY();
                                zL2 = hitL2->GetZ();
                                TPolyLine3D* track = CreateTrack(xV/10, yV/10, zV/10, xL2/10, yL2/10, zL2/10);  // Converto le coordinate da mm in cm (utilizzati per definire la geometria)
                                track->Draw();
                                tracks.push_back(track);
                                break;
                            }
                        }

                        break;
                    }
                }
            }

            break;
        }
    }

    canvas->Modified();
    canvas->Update();
}

// // Callback per bottoni
// void NextEvent() {
//     ++currentEvent;
//     std::cout << "Visualizing event: " << currentEvent << std::endl;
//     DrawEvent(currentEvent);
// }
// void PrevEvent() {
//     if (currentEvent > 0) {
//         currentEvent--;
//         std::cout << "Visualizing event: " << currentEvent << std::endl;
//         DrawEvent(currentEvent);
//     } else {
//         std::cout << "Sei già all'evento 0.\n";
//     }
// }

// Funzione principale
void EventDisplay(const char* filename = "treeSimulated.root", TString eventID = "e0") {
    
    // Apre il file e il tree
    TFile* file = new TFile(filename);
    tree = (TTree*)file->Get("T");

    if (!tree) {
        std::cerr << "Errore: 'tree' non trovato nel file '" << filename << "'\n";
        return;
    }

    // Costruisce la geometria
    geom = new TGeoManager("Detector", "Detector Geometry");
    BuildGeometry();

    // Canvas e disegno
    canvas = new TCanvas("canvas", "Event Display", 1000, 800);
    geom->GetTopVolume()->Draw("ogl");
    
    // // Bottone "Next Event"
    // TButton* nextBtn = new TButton("Next Event", "NextEvent()", 0.85, 0.9, 0.98, 0.95);
    // nextBtn->SetFillColor(kYellow);
    // nextBtn->Draw();
    // // Bottone "Prev Event"
    // TButton* prevBtn = new TButton("Prev Event", "PrevEvent()", 0.7, 0.9, 0.83, 0.95);
    // prevBtn->SetFillColor(kCyan);
    // prevBtn->Draw();

    DrawEvent(tree, eventID); // Disegna l'evento richiesto

    std::cout << "Visualizing event: " << eventID << std::endl;

}

void Debug(const char* filename = "treeSimulated.root") {

    TFile* file = new TFile(filename);
    file->ls();
    tree = (TTree*)file->Get("TreeED");
    tree->Print();

    tree->Show(0);

    TString* evID = nullptr;
    pEvent* event = nullptr;

    tree->SetBranchAddress("eventID", &evID);
    tree->SetBranchAddress("Event", &event);



}