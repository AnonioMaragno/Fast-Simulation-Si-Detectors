#include "pEvent.h"

ClassImp(pEvent)

// Array che conterranno le hit
TClonesArray* pEvent::fHitsBP = new TClonesArray("pHit", 100);
TClonesArray* pEvent::fHitsL1 = new TClonesArray("pHit", 100);
TClonesArray* pEvent::fHitsL2 = new TClonesArray("pHit", 100);


// Default Constructor
// metto molteplicità a 0, vertice a (0,0,0) 
pEvent::pEvent(): TObject(),
fM(0){
    pPoint *pto = new pPoint();
    SetVertex(pto);
    feventID = "eInd";
}

// Standard Constructor
// metto molteplicità a mult, vertice a (x,y,z) 
pEvent::pEvent(pPoint *vert, int mult, int eventCounter): TObject(),
fM(mult){
    pPoint *pto = new pPoint(*vert);
    SetVertex(pto);
    feventID = Form("e%d", eventCounter);
}


// Destructor
pEvent::~pEvent()
{    
    // libera memoria
    delete fVertex;
    fVertex = nullptr;
    fHitsBP->Clear();
    fHitsL1->Clear();
    fHitsL2->Clear();
}


// Trasporto della particella da un punto iniziale ad un layer (c -> coseni direttori, lay -> layer di partenza, numParticle -> n-esima particella dell'evento)
bool pEvent::Trasporto(pPoint* pIniz, double* c, Layer lay, int numParticle)
{
    
    bool successFlag = true; // tiene conto del successo o meno del trasporto

    static double t, x0, y0, z0, x, y, z, R, delta, b, sqs; // delta, b e sqs servono come variabili ausiliari per snellire la scrittura
    
    // Prendo le coordinate iniziali
    x0 = pIniz->GetX();
    y0 = pIniz->GetY();
    z0 = pIniz->GetZ();

    static TClonesArray* ptrhits;

    if (lay == Layer::L1){
        R = 40;
        ptrhits = fHitsL1;
    }
    else if (lay == Layer::L2){
        R = 70;
        ptrhits = fHitsL2;
    }
    else{
        R = 30; 
        ptrhits = fHitsBP;
    }

 
    TClonesArray &hits = *ptrhits;
    int memPosition = ptrhits->GetEntriesFast(); // trova il numero di elementi già salvati e usa questo numero per aggiungere un elemento alla fine del TClonesArray

    // Calcolo di t
    b = x0*c[0] + y0*c[1];
    sqs = c[0]*c[0] + c[1]*c[1];
    delta = b*b - sqs * ( x0*x0 + y0*y0 - R*R );
    t = (-b+sqrt(delta))/sqs;

    // Calcolo di x, y, z finali
    x = x0 + c[0]*t;
    y = y0 + c[1]*t;
    z = z0 + c[2]*t;

    // Creo la hit se rispetto la condizione di essere nel rivelatore
    if ( lay == Layer::BP ){
        pIniz->SetCoord(x,y,z);
        new (hits[memPosition]) pHit(pIniz,lay,numParticle,feventID);   
    }
    else if (z<135 && z>-135){
        pIniz->SetCoord(x,y,z);
        new (hits[memPosition]) pHit(pIniz,lay,numParticle,feventID);
    }
    else{
        successFlag = false;
    }

    return successFlag;
}