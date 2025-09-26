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

    TString* evID = nullptr;
    pEvent* event = nullptr;

    tree->SetBranchAddress("eventID", &evID);
    tree->SetBranchAddress("Event", &event);

    for (int ev=0; ev<tree->GetEntries(); ev++) {
        tree->GetEvent(ev);
        if (evID->CompareTo(eventID) == 0) {

            cout << "Sono entrato nell'if" << endl;
            // Variabili di comodo
            static pHit* hitBP = nullptr;
            static pHit* hitL1 = nullptr;
            static pHit* hitL2 = nullptr;
            static double xV = 0;
            static double yV = 0;
            static double zV = 0;
            static double xBP = 0;
            static double yBP = 0;
            static double zBP = 0;
            static double xL1 = 0;
            static double yL1 = 0;
            static double zL1 = 0;
            static double xL2 = 0;
            static double yL2 = 0;
            static double zL2 = 0;
            bool isL1on = false;
            bool isL2on = false;

            xV = event->GetVertex()->GetX();
            yV = event->GetVertex()->GetY();
            zV = event->GetVertex()->GetZ();

            for(int i=0; i<event->GetPtrHitsBP()->GetEntriesFast(); i++){

                hitBP = (pHit* )event->GetPtrHitsBP()->At(i);                
                for (int j=0; j<event->GetPtrHitsL1()->GetEntriesFast(); j++) {

                    hitL1 = (pHit* )event->GetPtrHitsL1()->At(j);
                    if (hitBP->GetId().CompareTo(hitL1->GetId())) {
                        // cout << "sono qui " << endl;
                        for (int k=0; k<event->GetPtrHitsL2()->GetEntriesFast(); k++) {

                            hitL2 = (pHit* )event->GetPtrHitsL2()->At(k);
                            if (hitL1->GetId().CompareTo(hitL2->GetId())) {
                                
                                xL2 = hitL2->GetX();
                                yL2 = hitL2->GetY();
                                zL2 = hitL2->GetZ();
                                TPolyLine3D* track = CreateTrack(xV/10, yV/10, zV/10, xL2/10, yL2/10, zL2/10);  // Converto le coordinate da mm in cm (utilizzati per definire la geometria)
                                track->Draw();
                                tracks.push_back(track);
                                isL2on = true;
                                isL1on = true;
                                break;
                            }
                        }
                        if (isL2on == false) {
                            
                            xL1 = hitL1->GetX();
                            yL1 = hitL1->GetY();
                            zL1 = hitL1->GetZ();
                            TPolyLine3D* track = CreateTrack(xV/10, yV/10, zV/10, xL1/10, yL1/10, zL1/10);  // Converto le coordinate da mm in cm (utilizzati per definire la geometria)
                            track->Draw();
                            tracks.push_back(track);
                            isL2on = true;
                        }
                        break;
                    }
                }
                if (isL1on == false) {

                    xBP = hitBP->GetX();
                    yBP = hitBP->GetY();
                    zBP = hitBP->GetZ();
                    TPolyLine3D* track = CreateTrack(xV/10, yV/10, zV/10, xBP/10, yBP/10, zBP/10);  // Converto le coordinate da mm in cm (utilizzati per definire la geometria)
                    track->Draw();
                    tracks.push_back(track);
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
void EventDisplay(const char* filename = "treeEventDisplay.root", TString eventID = "e0") {
    // Apre il file e il tree
    TFile* file = new TFile(filename);
    tree = (TTree*)file->Get("TreeED");

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

void Debug(const char* filename = "treeEventDisplay.root") {

    TFile* file = new TFile(filename);
    tree = (TTree*)file->Get("TreeED");

    TString* evID = nullptr;
    pEvent* event = nullptr;

    tree->SetBranchAddress("eventID", &evID);
    tree->SetBranchAddress("Event", &event);

    for (int ev=0; ev<tree->GetEntries(); ev++) {
        tree->GetEvent(ev);
        cout << "Il problema è appena dopo di qui" << endl;
        cout << "Event ID = " << event->GetEventID() << endl;
    }
}