#include "pEvent.h"

ClassImp(pEvent)

// contatore per vedere il numero dell'evento (per debug)
int pEvent::fCounter = 0;

//contatori di registrazione eventi
int pEvent::fRegisteredL1 = 0;
int pEvent::fRegisteredL2 = 0;


//array che conterranno le hit
TClonesArray* pEvent::fHitsBP = new TClonesArray("pHit", 50);
TClonesArray* pEvent::fHitsL1 = new TClonesArray("pHit", 50);
TClonesArray* pEvent::fHitsL2 = new TClonesArray("pHit", 50);



// Default Constructor
// metto molteplicità a zero, vertice a (0,0,0) 
pEvent::pEvent(): TObject(),
fM(0){
    pPoint *pto = new pPoint(0., 0., 0.);
    SetVertex(pto);
    fCounter += 1;
}

// Standard Constructor
// metto molteplicità a mult, vertice a (x,y,z) 
pEvent::pEvent(pPoint *vert, int mult): TObject(),
fM(mult){
    SetVertex(vert);
    fCounter += 1;
}

pEvent::pEvent(const pEvent& source): 
TObject(source),
fVertex(source.fVertex),
fM(source.fM){
    

}

// distruttore
pEvent::~pEvent()
{    
    // debug
    cout << "\n\nCancello l'evento numero: " << fCounter << endl;

    //libera memoria
    fVertex = nullptr; //si occupa già il programma di simulazione liberare la memoria heap
    fHitsL1->Clear();
    fHitsL2->Clear();
}



// Trasporto della particella da un punto iniziale ad un layer (c -> coseni direttori)
pPoint* pEvent::Trasporto(pPoint* pIniz, double* c, Layer lay, int index)
{
    static double t, x0, y0, z0, x, y, z, R, delta, b, sqs;
    //delta, b e sqs servono come variabili ausiliare per snellire la scrittura
    
    x0 = pIniz->GetX();
    y0 = pIniz->GetY();
    z0 = pIniz->GetZ();

    static TClonesArray* ptrhits = nullptr;
    //static pPoint* ptrNewPoint;
    static int* ptrRegCounter;

    if (lay == Layer::L1){
        R = 40;
        ptrhits = fHitsL1;
        ptrRegCounter = &fRegisteredL1;
    }
    else if (lay == Layer::L2){
        R = 70;
        ptrhits = fHitsL2;
        ptrRegCounter = &fRegisteredL2;
    }
    else{
        R = 30; 
        ptrhits = fHitsBP;
    } 

    cout << "Puntatore" << ptrhits << "  Taglia del TclonesArray: " << ptrhits->GetSize() << endl;

 
    TClonesArray &hits = *ptrhits; 

    // calcolo di t
    b = x0*c[0] + y0*c[1];
    sqs = c[0]*c[0] + c[1]*c[1];
    delta = b*b - sqs * ( x0*x0 + y0*y0 - R*R );
    t = (-b+sqrt(delta))/sqs;

    //calcolo di x, y, z finali
    x = x0 + c[0]*t;
    y = y0 + c[1]*t;
    z = z0 + c[2]*t;

    cout << "Coordinate del punto (funzione Trasporto): X = " << x << " Y = " << y << " Z = " << z << endl;

    //creo la hit se rispetto la condizione di essere nel rivelatore
    if ( lay == Layer::BP ){
        new (hits[index]) pHit(x,y,z,lay,index,fCounter);
        
        pIniz->~pPoint();
        new (pIniz) pPoint(x,y,z);
    }
    else if (z<135 && z>-135){
        new (hits[*ptrRegCounter]) pHit(x,y,z,lay,index,fCounter);
        *ptrRegCounter += 1;
        pIniz->~pPoint();
        new (pIniz) pPoint(x,y,z);
    }
    else{
        pIniz->~pPoint();
        pIniz = nullptr;
    }

    cout << "Sono fuori dal tunnel del Trasporto" << endl;

    return pIniz;
}