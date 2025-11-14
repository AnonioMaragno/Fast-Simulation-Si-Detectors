#include "pEvent.h"
#include <TGeoManager.h>
#include "TVector3.h"
#include <TPolyLine3D.h>
#include "TEllipse.h"
#include <TRandom3.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TButton.h>
#include <TLine.h>
#include <TFile.h>
#include <TTree.h>
#include <vector>
#include <iostream>

TGeoManager* geom = nullptr;
TCanvas* canvas = nullptr;
TCanvas* cL = new TCanvas("cL", "Lateral view", 600, 500, 600, 400);
TCanvas* cF = new TCanvas("cF", "Frontal view", 1200, 500, 600, 600);


TTree* tree = nullptr;
int currentEvent = 0;

std::vector<TPolyLine3D*> tracks;
std::vector<TLine*> linesF;
std::vector<TLine*> linesL;

void DrawGeoProj(){
    cF->cd();

    TH1F* hframe = cF->DrawFrame(-10, -10, 10, 10);
    hframe->SetTitle(";X;Y");

    TEllipse *beampipe = new TEllipse(0, 0, 3, 3);
    beampipe->SetLineColor(kOrange-5);
    beampipe->SetFillStyle(0); 
    beampipe->Draw("same");

    TEllipse *layer1 = new TEllipse(0, 0, 4, 4);
    layer1->SetLineColor(kBlue-3);
    layer1->SetFillStyle(0); 
    layer1->Draw("same");

    TEllipse *layer2 = new TEllipse(0, 0, 7, 7);
    layer2->SetLineColor(kRed-2);
    layer2->SetFillStyle(0); 
    layer2->Draw("same");

    cL->cd();

    TH1F* hframe2 = cL->DrawFrame(-25, -10, 25, 10);
    hframe2->SetTitle(";Z;Y");

    TLine *bp = new TLine(-20, 3, 20, 3);
    bp->SetLineColor(kOrange-5);
    bp->SetLineWidth(2);
    bp->Draw("same");

    TLine *l1 = new TLine(-13.5, 4, 13.5, 4);
    l1->SetLineColor(kBlue-3);
    l1->SetLineWidth(2);
    l1->Draw("same");

    TLine *l2 = new TLine(-13.5, 7, 13.5, 7);
    l2->SetLineColor(kRed-2);
    l2->SetLineWidth(2);
    l2->Draw("same");

    TLine *bp_down = new TLine(-20, -3, 20, -3);
    bp_down->SetLineColor(kOrange-5);
    bp_down->SetLineWidth(2);
    bp_down->Draw("same");

    TLine *l1_down = new TLine(-13.5, -4, 13.5, -4);
    l1_down->SetLineColor(kBlue-3);
    l1_down->SetLineWidth(2);
    l1_down->Draw("same");

    TLine *l2_down = new TLine(-13.5, -7, 13.5, -7);
    l2_down->SetLineColor(kRed-2);
    l2_down->SetLineWidth(2);
    l2_down->Draw("same");

    
}

// Funzione per disegnare una traiettoria
TPolyLine3D* CreateTrack(pPoint* pto0, pPoint* pto1) {

    TPolyLine3D* track = new TPolyLine3D(2);
    track->SetPoint(0, pto0->GetX()/10, pto0->GetY()/10, pto0->GetZ()/10);   // Converto le coordinate da mm in cm (utilizzati per definire la geometria)
    track->SetPoint(1, pto1->GetX()/10, pto1->GetY()/10, pto1->GetZ()/10);
    track->SetLineColor(kGreen+2);
    track->SetLineWidth(2);
    return track;
}

void CreateLines(pPoint* pto0, pPoint* pto1){
    linesF.push_back(new TLine(pto0->GetX()/10, pto0->GetY()/10, pto1->GetX()/10, pto1->GetY()/10));
    linesF.back()->SetLineColor(kGreen+2);
    linesF.back()->SetLineWidth(2);

    linesL.push_back(new TLine(pto0->GetZ()/10, pto0->GetY()/10, pto1->GetZ()/10, pto1->GetY()/10));
    linesL.back()->SetLineColor(kGreen+2);
    linesL.back()->SetLineWidth(2);
}

void allungaTrack(TPolyLine3D* prevTrack){

    float* pts = prevTrack->GetP();
    TVector3 dir(pts[3] - pts[0], pts[4] - pts[1], pts[5] - pts[2]);
    dir = dir.Unit();

    double Ll = 2;
    linesF.back()->SetX2(pts[3] + Ll*dir.X());
    linesF.back()->SetY2(pts[4] + Ll*dir.Y());

    linesL.back()->SetX2(pts[5] + Ll*dir.Z());
    linesL.back()->SetY2(pts[4] + Ll*dir.Y());

    double L = 20;
    prevTrack->SetPoint(1, pts[0] + L*dir.X(), pts[1] + L*dir.Y(), pts[2] + L*dir.Z());   

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

void ClearLines() {
    for (auto* l : linesF) {
        l->Delete();
    }
    linesF.clear();

    for (auto* l_l : linesL) {
        l_l->Delete();
    }
    linesL.clear();
}


// Disegna un evento
void DrawEvent(TTree* tree, TString eventID) {
    ClearTracks();
    ClearLines();

    static Layer layers[3] = {Layer::BP, Layer::L1, Layer::L2};

    // Dichiarazione oggetti in cui salvare dati dal tree in input
    TString* evIDptr = 0;  
    pPoint* vtx = 0;
    double zVertex;
    int multi;
    TClonesArray* ptrHitsL1 = 0;
    TClonesArray* ptrHitsL2 = 0;
    TClonesArray* ptrHitsBP = 0;

    // Definizione degli indirizzi per la lettura dei dati su ttree
    tree->SetBranchAddress("eventID", &evIDptr);
    tree->SetBranchAddress("vtx", &vtx);
    tree->SetBranchAddress("zVertex", &zVertex);
    tree->SetBranchAddress("Mult", &multi);
    tree->SetBranchAddress("HitsL1", &ptrHitsL1);
    tree->SetBranchAddress("HitsL2", &ptrHitsL2);
    tree->SetBranchAddress("HitsBP", &ptrHitsBP);

    TClonesArray* ptrHits;

    for (int ev=0; ev<tree->GetEntries(); ev++) {
        tree->GetEvent(ev);
        if (evIDptr->CompareTo(eventID) == 0) {

            // Variabili di comodo
            pPoint* hit1 = nullptr;
            pHit* hit2 = nullptr;
            
            cout << "Coordinate del vertice = " << "(" << vtx->GetX() << "," << vtx->GetY() << "," << vtx->GetZ() << ")" << endl;
            cout << "Molteplicità: " << multi << ". Entries su BP,L1 e L2: " << ptrHitsBP->GetEntriesFast() << "," << ptrHitsL1->GetEntriesFast() << "," << ptrHitsL2->GetEntriesFast() << endl;

            for(int i=0; i<multi; i++){

                bool hitFound;

                // costruisco il particle id
                TString particleID = *evIDptr + Form("-p%d", i); 
                hit1 = vtx;

                //loop sui layer
                for (const auto& lay : layers){

                    if (lay == Layer::L1){
                        ptrHits = ptrHitsL1;
                    }
                    else if (lay == Layer::L2){
                        ptrHits = ptrHitsL2;
                    }
                    else{
                        ptrHits = ptrHitsBP;
                    }

                    hitFound = false;
                    
                    for (int k=0; k<ptrHits->GetEntriesFast(); k++){                        
                        
                        hit2 = (pHit*) ptrHits->At(k);

                        if (hit2->GetId().CompareTo(particleID) == 0){
                            TPolyLine3D* track = CreateTrack(hit1, hit2);
                            CreateLines(hit1,hit2);
                            //track->Draw();
                            tracks.push_back(track);
                            hit1 = static_cast <pPoint*> (hit2);
                            hitFound = true;
                            break;    
                        }
                    }

                    if (!hitFound){
                        tracks.back()->SetLineColor(kRed);
                        linesF.back()->SetLineColor(kRed);
                        linesL.back()->SetLineColor(kRed);   
                        break;
                    }
                }

                allungaTrack(tracks.back());
            }

            canvas->cd();
            for (auto* t : tracks) {
                t->Draw();
            }

            cF->cd();
            for (auto* l : linesF) {
                l->Draw("same");
            }

            cL->cd();
            for (auto* l : linesL) {
                l->Draw("same");
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
void EventDisplay(TString ev = "e90", const char* filename = "treeSimulated.root") {
    
    // Apre il file e il tree
    TFile* file = new TFile(filename);
    tree = (TTree*)file->Get("T");

    if (!tree) {
        std::cerr << "Errore: 'tree' non trovato nel file '" << filename << "'\n";
        return;
    }

    DrawGeoProj();

    // Costruisce la geometria
    geom = new TGeoManager("Detector", "Detector Geometry");
    BuildGeometry();

    // // Canvas e disegno
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

    DrawEvent(tree, ev); // Disegna l'evento richiesto
    cout << "Visualizing ev = " << ev << endl;
    canvas->Close();
   
}

// void Debug(const char* filename = "treeSimulated.root") {

//     TFile* file = new TFile(filename);
//     file->ls();
//     tree = (TTree*)file->Get("T");
//     tree->Print();

//     tree->Show(0);

//     TString* evID = nullptr;
//     pEvent* event = nullptr;

//     tree->SetBranchAddress("eventID", &evID);
//     tree->SetBranchAddress("Event", &event);



// }