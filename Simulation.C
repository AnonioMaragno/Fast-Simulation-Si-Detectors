#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"

const int kNoEvents = 20; //Numero di eventi da simulare
const bool kFlagMS = false; //Flag per vedere se simulare o no il Multiple scattering 

pPoint* generaVertice();
void generaDirezione(TH1F* etaDist, double& Th, double& Ph);
void findCosDirection(double *cosDir, double th, double phi);
void MultipleScattering(double* cd);

void Simulation() {

    // Lettura distribuzioni di molteplicità e psedorapidità dal file kinem.root
    TFile *f = new TFile("kinem.root");
    TH1F *eta = (TH1F*)f->Get("heta2");
    TH1F *mul = (TH1F*)f->Get("hm");

    // Creazione di un tree
    TFile* outfile = new TFile("treeSimulated.root", "RECREATE");
    TTree *tree = new TTree("T","TTree con 4 branches");

    TClonesArray* ptrHitsL1 = pEvent::GetPtrHitsL1();
    TClonesArray* ptrHitsL2 = pEvent::GetPtrHitsL2();

    pPoint* vertex;//qui dichiaro e poi instanzio memoria su vertex nella funzione generaVertice
    double zVert = 0.0;//per salvare solo z del vertice
    int multi = 0;//molteplicità

    
    // Dichiarazione dei 4 branch del TTree
    tree->Branch("zVertex", &zVert);
    tree->Branch("Mult", &multi);
    //tree->Branch("HitsL1", "TClonesArray", &ptrHitsL1, 64000, 0);
    //tree->Branch("HitsL2", "TClonesArray", &ptrHitsL2, 64000, 0);
    tree->Branch("HitsL1", &ptrHitsL1);
    tree->Branch("HitsL2", &ptrHitsL2);

    double theta, phi; //direzione iniziale

    //puntatori ausiliari    
    pPoint* tempPoint;
    //pPoint* ptrPoint;


    double cd[3]; //array che conterrà i coseni direttori  

    Layer layers[3] = {Layer::BP, Layer::L1, Layer::L2};//array che serve per simulare cronologicamente dove incide la particella

    for (int k=0; k<kNoEvents; k++){
        vertex = generaVertice(); //generazione vertice
        cout << "Evento n" << k+1 << ". Cooridnate del vertice: X = " << vertex->GetX() << ", Y = " << vertex->GetY() << ", Z = " << vertex->GetZ() << endl; 

        multi = (int) mul->GetRandom();//genera molteplicità
        cout << "Molteplicità di particelle nell'evento = " << multi << endl;

        pEvent* ev = new pEvent(vertex, multi);
        
        for (int index = 0; index<multi; index++){
            cout << "Particella n" << index+1 << endl;

            tempPoint = new pPoint(*vertex);//metto sullo stack perché tanto la copia viene cancellata alla fine dell'iterazione
            generaDirezione(eta, theta, phi);//generi theta e phi
            cout << "Direzione inziale: theta = " << theta << ", phi = " << phi << endl;
            //cout << "tempPoint coordinate (dovrebbero essere le stesse di vertex): X = " << tempPoint->GetX() << " Y = " << tempPoint->GetY() << " Z = " << tempPoint->GetZ() << endl;

            findCosDirection(cd, theta, phi);//trova coseni direttori
            //cout << "Coseni direttori: cd[0] = " << cd[0] << " cd[1] = " << cd[1] << " cd[2] = " << cd[2] << endl;

            for (const auto& l : layers){
                tempPoint = ev->Trasporto(tempPoint, cd, l, index);//Qui si può ridurre tutto a tempPoint ossia riaggiornarlo stesso qui dentro
                if (tempPoint == nullptr){
                    break;
                }
                else{
                    if (kFlagMS){
                        MultipleScattering(cd);
                    }
                }
            }
            delete tempPoint;
        }
        zVert = ev->GetZVertex();

        cout << "Se arrivo qua il problema è col tree" << endl;
        cout << "Puntatore al fHitsL1 " << ptrHitsL1 << " con ingressi in L1 pari a: " << ptrHitsL1->GetEntries() << endl;
        cout << "Puntatore al fHitsL2 " << ptrHitsL2 << " con ingressi in L2 pari a: " << ptrHitsL2->GetEntries() << endl;


        tree -> Fill();

        cout << "Se arrivo qua il problema è dopo il tree" << endl;

        delete vertex;
        vertex = nullptr;
        delete ev;

        cout << "Puntatore al fHitsL1 " << ptrHitsL1 << " con ingressi in L1 pari a: " << ptrHitsL1->GetEntries() << endl;
        cout << "Puntatore al fHitsL2 " << ptrHitsL2 << " con ingressi in L2 pari a: " << ptrHitsL2->GetEntries() << endl;

    }



    outfile->Write();
    outfile->Close();

    pEvent::disallocateMemory();


}

pPoint* generaVertice(){
    // Generazione del vertice
    double xVert = gRandom->Gaus(0,0.1);
    double yVert = gRandom->Gaus(0,0.1);
    double zVert = gRandom->Gaus(0,53);
    pPoint* v = new pPoint(xVert, yVert, zVert);
    return v;
};

void generaDirezione(TH1F* etaDist, double& Th, double& Ph){
    // Generazione theta e phi
    Ph = 2*acos(-1)*(gRandom->Rndm());
    double pseudor = etaDist->GetRandom();
    Th = 2*atan(exp(-pseudor));
}


void findCosDirection(double *cosDir, double th, double phi)
{
    cosDir[0] = sin(th) * cos(phi);
    cosDir[1] = sin(th) * sin(phi);
    cosDir[2] = cos(th);
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
    double thp = gRandom->Gaus(0,0.001);
    double php = 2*acos(-1)*gRandom->Rndm();
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
