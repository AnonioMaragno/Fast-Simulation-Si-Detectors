#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TROOT.h"
#include "TKey.h"
#include "TH1F.h"
#include "TF1.h"
#include "pPoint.h"

const int kNoEvents = 10000; //Numero di eventi da simulare
const bool kFlagMS = false; //Flag per vedere se simulare o no il Multiple scattering 
const bool kFlagUniform = false; //Flag per vedere se usare una distribuzione uniforme o no per la molteplicità


void generaVertice(pPoint* vtx);
void generaDirezione(TH1F* etaDist, double *cosDir);
void MultipleScattering(double* cd);

void Simulation() {

    cout << "----------------------------------" << endl;
    cout << "-------- SIMULATION --------------" << endl;
    cout << "----------------------------------" << endl << endl;

    // Lettura distribuzioni di molteplicità e psedorapidità dal file kinem.root
    TFile *f = new TFile("kinem.root");
    TH1F *eta = (TH1F*)f->Get("heta2");
    TH1F* mul;


    if (kFlagUniform){
        // se si vuole distribuzione uniforme per molteplicità
        mul = new TH1F("h", "Distribuzione uniforme", 100, 2, 100); //minimo 2 particelle
        TF1* func = new TF1("func", "1", 2, 100);

        mul->FillRandom("func", 100000); 
    }
    else{
        mul = (TH1F*)f->Get("hm");
    }


    // Creazione di un tree
    TFile* outfile = new TFile("treeSimulated.root", "RECREATE");
    TTree *tree = new TTree("T","TTree con 5 branches");

    TString eventID; //eventID per debug
    double zVert = 0.0;//per salvare solo z del vertice
    int multi = 0;//molteplicità
    TClonesArray* ptrHitsL1 = pEvent::GetPtrHitsL1(); //per salvare hit  su layer 1
    TClonesArray* ptrHitsL2 = pEvent::GetPtrHitsL2(); //per salvare hit  su layer 2

    
    // Dichiarazione dei 5 branch del TTree
    tree->Branch("eventID", &eventID);
    tree->Branch("zVertex", &zVert);
    tree->Branch("Mult", &multi);
    tree->Branch("HitsL1", &ptrHitsL1);
    tree->Branch("HitsL2", &ptrHitsL2);

    pPoint* vertex = new pPoint();     //qui dichiaro punto generico e poi setto coordinate in generaVertice
    pPoint* tempPoint = new pPoint();    //punto ausiliare per la funzione Trasporto 


    double cd[3]; //array che conterrà i coseni direttori  

    Layer layers[3] = {Layer::BP, Layer::L1, Layer::L2};//array che serve per simulare cronologicamente dove incide la particella

    for (int k=0; k<kNoEvents; k++){

        generaVertice(vertex); //generazione vertice
        multi = (int) mul->GetRandom();//genera molteplicità

        pEvent* ev = new pEvent(vertex, multi, k);
        
        for (int nParticle = 0; nParticle < multi; nParticle++){
            
            tempPoint->SetEqualTo(*vertex); //copia le coordinate del vertice nel punto ausiliario
            generaDirezione(eta, cd); //genera theta e phi e trova coseni direttori

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
        
        eventID = ev->GetEventID();
        zVert = ev->GetZVertex();

        tree -> Fill();

        delete ev;
        ev = nullptr;
    }

    outfile->Write();
    outfile->Close();
    f->Close();

    delete vertex;
    vertex = nullptr;
    delete tempPoint;
    tempPoint = nullptr;
    pEvent::disallocateMemory();


}

void generaVertice(pPoint* vtx){
    // Generazione del vertice
    double xVert = 0.; //gRandom->Gaus(0,0.1);
    double yVert = 0.; //gRandom->Gaus(0,0.1);
    double zVert = gRandom->Gaus(0,53);

    vtx->SetCoord(xVert, yVert, zVert);
}


void generaDirezione(TH1F* etaDist, double *cosDir){
    // Generazione theta e phi
    double Phi = 2*acos(-1)*(gRandom->Rndm());
    double pseudor = etaDist->GetRandom();
    double Theta = 2*atan(exp(-pseudor));

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
    double php = 2*acos(-1)*gRandom->Rndm();
    double thp = thplane/sin(php);
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
