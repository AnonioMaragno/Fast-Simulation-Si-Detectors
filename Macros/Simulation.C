#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TMath.h"
#include "pEvent.h"
#include "TClonesArray.h"
#include "Riostream.h"
#include "TRandom3.h"
#include "TH1F.h"

const int kNoEvents = 10; //Numero di eventi da simulare
const bool kFlagMS = false; //Flag per vedere se simulare o no il Multiple scattering 

void findCosDirection(double *cosDir, double th, double phi);
void MultipleScattering(double* cd);

void Simulation() {

    // Lettura distribuzioni di molteplicità e psedorapidità dal file kinem.root
    TFile *f = new TFile("kinem.root");
    TH1F *eta = (TH1F*)f->Get("heta");
    TH1F *mul = (TH1F*)f->Get("hmul");
/*
    // Creazione di un tree
    TFile hfile("htree.root", "RECREATE");
    TTree *tree = new TTree("T","TTree con 3 branches");

    // Dichiarazione dei 2 branch del TTree
    tree->Branch("VertMult",&point.X,"X/D:Y:Z:mult/I");
    tree->Branch("Hits",&ptrhits);

*/
    // Generazione del vertice
    double xVert = gRandom->Gaus(0,0.1);
    double yVert = gRandom->Gaus(0,0.1);
    double zVert = gRandom->Gaus(0,53);

    // Generazione molteplicità
    double mult = mul->GetRandom();

    // Generazione theta e phi
    double phi = 2*TMath::ACos(-1)*gRandom->Rndm();
    double pseudor = eta->GetRandom();
    double theta = 2*TMath::ATan(exp(-pseudor));





    double Theta, Phi;
    pPoint vertex(3, 4, 5);
    int multi = 10;
    pPoint* tempPoint;
    pPoint* ptrPoint;


    double cd[3]; //array che conterrà i coseni direttori  

    Layer layers[3] = {Layer::BP, Layer::L1, Layer::L2};//array che serve per simulare cronologicamente dove incide la particella

    for (int k=0; k<kNoEvents; k++){
        // crei pPoint del vertice
        // genera molteplicità
        pEvent* ev = new pEvent(&vertex, multi);
        tempPoint = &pPoint(vertex);
        for (int index = 0; index<multi; index++){
            //generi theta e phi
            findCosDirection(cd, Theta, Phi);
            for (const auto& l : layers){
                ptrPoint = ev->Trasporto(tempPoint, cd, l, index);//Qui si può ridurre tutto a tempPoint ossia riaggiornarlo stesso qui dentro
                if (ptrPoint == nullptr){
                    break;
                }
                else{
                    tempPoint = ptrPoint;
                    if (kFlagMS){
                        MultipleScattering(cd);
                    }
                }
            }

        }
        
        delete ev;
    }







}

void findCosDirection(double *cosDir, double th, double phi)
{
    cosDir[0] = sin(th) * cos(phi);
    cosDir[1] = sin(th) * sin(phi);
    cosDir[2] = cos(th);
}




















































void MultipleScattering(double *cd){
    
    double mr[3][3];
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

    double thp = 0.001;
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