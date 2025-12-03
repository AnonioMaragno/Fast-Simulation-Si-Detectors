#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"
#include "TF1.h"
#include "pPoint.h"
#include "TSystem.h"

const int kNoEvents = 1000;         // Numero di eventi da simulare
const bool kFlagMS = true;          // Flag per vedere se simulare o no il Multiple scattering 
const bool kFlagUniform = false;    // Flag per vedere se usare una distribuzione uniforme o no per la molteplicità
const bool kProgress = true;

void generaVertice(pPoint* vtx);
void generaDirezione(TH1F* etaDist, double *cosDir);
void MultipleScattering(double* cd);

void Simulation() {

    gRandom->SetSeed(0);

    cout << "----------------------------------" << endl;
    cout << "-------- SIMULATION --------------" << endl;
    cout << "----------------------------------" << endl << endl;

    // Lettura distribuzioni di molteplicità e psedorapidità dal file kinem.root
    TFile *f = new TFile("kinem.root");
    TH1F *eta = (TH1F*)f->Get("heta2");
    TH1F* mul;


    if (kFlagUniform){
        // se si vuole distribuzione uniforme per molteplicità
        mul = new TH1F("h", "Distribuzione uniforme", 100, 2, 100); // minimo 2 particelle
        TF1* func = new TF1("func", "1", 2, 100);

        mul->FillRandom("func", 100000); 
    }
    else{
        mul = (TH1F*)f->Get("hm");
    }


    // Creazione di un file di output e di un tree
    TFile* outfile = new TFile("treeSimulated.root", "RECREATE");
    TTree *tree = new TTree("T","TTree con 7 branches");

    TString eventID;    // eventID per debug
    double zVert = 0.0; // per salvare solo z del vertice
    int multi = 0;      // molteplicità
    TClonesArray* ptrHitsL1 = pEvent::GetPtrHitsL1(); // per salvare hit  su layer 1
    TClonesArray* ptrHitsL2 = pEvent::GetPtrHitsL2(); // per salvare hit  su layer 2
    TClonesArray* ptrHitsBP = pEvent::GetPtrHitsBP(); // per salvare hit  su beam pipe, serve per Event Display

    pPoint* vertex = new pPoint();     // qui dichiaro punto generico e poi setto coordinate in generaVertice
    pPoint* tempPoint = new pPoint();  // punto ausiliare per la funzione Trasporto 


    // Dichiarazione dei 7 branch del TTree
    tree->Branch("eventID", &eventID);
    tree->Branch("vtx", &vertex);
    tree->Branch("zVertex", &zVert);
    tree->Branch("Mult", &multi);
    tree->Branch("HitsL1", &ptrHitsL1);
    tree->Branch("HitsL2", &ptrHitsL2);
    tree->Branch("HitsBP", &ptrHitsBP);

    double cd[3]; // array che conterrà i coseni direttori  

    Layer layers[3] = {Layer::BP, Layer::L1, Layer::L2}; // array che serve per simulare cronologicamente dove incide la particella

    // Implementazione di una barra di progresso
    int barWidth = 70;

    if (kProgress){
        std::cout << "[";
        for (int j = 0; j < barWidth; ++j) { std::cout << " ";}
        std::cout << "] 0 %\r" << std::flush;
    }
    

    for (int k=0; k<kNoEvents; k++){

        generaVertice(vertex); // generazione vertice
        multi = (int) mul->GetRandom(); // generazione molteplicità

        pEvent* ev = new pEvent(vertex, multi, k);
        
        for (int nParticle = 0; nParticle < multi; nParticle++){
            
            tempPoint->SetEqualTo(*vertex); // copia le coordinate del vertice nel punto ausiliario
            generaDirezione(eta, cd); // genera theta e phi e trova coseni direttori

            for (const auto& l : layers){
                bool success = ev->Trasporto(tempPoint, cd, l, nParticle);
                if (success){
                    if (kFlagMS){
                        MultipleScattering(cd);
                    }
                }
                else{  
                    break;
                }
            }
        }

        if (kProgress && (k % 100 == 0 || k == kNoEvents-1)){
            // ------------- PROGRESS BAR -------------- //
            float progress = (float) (k+1.) / kNoEvents;
            
            
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
        
        eventID = ev->GetEventID();
        zVert = ev->GetZVertex();

        tree -> Fill();

        if (k > 0 && k % 1000000 == 0) {
            tree->AutoSave("SaveSelf"); // forza lo svuotamento dei buffer su disco, l'opzione SaveSelf assicura che il tree sia aggiornato nella sua posizione
        }

        delete ev;
        ev = nullptr;
    }

    std::cout << std::endl; // va a capo alla fine

    outfile->Write();
    outfile->Close();
    f->Close();

    // Libera memoria allocata
    delete vertex;
    vertex = nullptr;
    delete tempPoint;
    tempPoint = nullptr;
    pEvent::disallocateMemory();


}

void generaVertice(pPoint* vtx){
    // Generazione del vertice
    double xVert = gRandom->Gaus(0,0.1);
    double yVert = gRandom->Gaus(0,0.1);
    double zVert = gRandom->Gaus(0,53);

    //distribuzione uniforme in Z
    //double zVert = 400.*(gRandom->Rndm()) - 200.0;

    vtx->SetCoord(xVert, yVert, zVert);
}


void generaDirezione(TH1F* etaDist, double *cosDir){
    // Generazione theta e phi
    double Phi = 2*acos(-1)*(gRandom->Rndm());
    double pseudor = etaDist->GetRandom();
    double Theta = 2*atan(exp(-pseudor));

    //per generare uniformemente in cosTheta
    //Theta = acos(1 - 2*(gRandom->Rndm()));

    //generazione coseni direttori
    cosDir[0] = sin(Theta) * cos(Phi);
    cosDir[1] = sin(Theta) * sin(Phi);
    cosDir[2] = cos(Theta);
}


void MultipleScattering(double *cd){
    
    static double mr[3][3];
    double sintheta = sqrt(1 - cd[2]*cd[2]);
    mr[0][0] = -cd[1]/sintheta;
    mr[1][0] = cd[0]/sintheta;
    mr[2][0] = 0.;
    mr[0][1] = -cd[0]*cd[2]/(sintheta);
    mr[1][1] = -cd[1]*cd[2]/(sintheta);
    mr[2][1] = sintheta;
    mr[0][2] = cd[0];
    mr[1][2] = cd[1];
    mr[2][2] = cd[2];

    // Generazione delle direzioni dopo lo scttaering multiplo
    double thplane = gRandom->Gaus(0,0.001);
    double php;
    if (thplane > 0){
        php = acos(-1)*gRandom->Rndm();
    } 
    else{
        php = acos(-1) + acos(-1)*gRandom->Rndm();
    }
    double thp = atan(tan(thplane)/sin(php)); // passaggio da theta plane a theta spaziale
    double cdp[3];
    cdp[0] = sin(thp)*cos(php);
    cdp[1] = sin(thp)*sin(php);
    cdp[2] = cos(thp);
    
    for(int i=0; i<3; i++){
        cd[i] = 0;
        for(int j=0; j<3; j++){
            cd[i] += mr[i][j] * cdp[j];
        }
    }

}
