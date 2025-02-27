#include "pEvent.h"

// contatore per vedere il numero dell'evento (per debug)
int pEvent::fCounter = 0;

//array che conterranno le hit
TClonesArray* pEvent::fHitsL1 = new TClonesArray("pHit", 50);
TClonesArray* pEvent::fHitsL2 = new TClonesArray("pHit", 50);



// Default Constructor
// metto molteplicità a zero, vertice a (0,0,0) 
pEvent::pEvent(): TObject(),
fM(0),
fVertex(pPoint()){
    fCounter += 1;
}

// Standard Constructor
// metto molteplicità a mult, vertice a (x,y,z) 
pEvent::pEvent(pPoint *vert, int mult): TObject(),
fM(mult),
fVertex(*vert){
    fCounter += 1;
}

// distruttore
pEvent::~pEvent()
{
    // debug
    cout << "Cancello l'evento numero: " << fCounter << endl;
    fHitsL1->Clear();
    fHitsL2->Clear();
}



// Trasporto della particella da un punto iniziale ad un layer
pHit pEvent::Trasporto(pPoint pIniz, double Theta, double Phi)
{
    static double *cosDir[3]; //array che conterrà i coseni direttori
    findCosDirection(cosDir, th, phi);//trova coseni direttori
    static double t, x0, y0, z0, delta;
    x0 = pIniz.GetX();
    y0 = pIniz.GetY();
    z0 = pIniz.GetZ();

    t = 


    return ;
}
