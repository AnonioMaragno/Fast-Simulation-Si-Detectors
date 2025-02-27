#include "pEvent.h"

// contatore per vedere il numero dell'evento (per debug)
int pEvent::fCounter = 0;


// Default Constructor
// metto molteplicità a zero, vertice a (0,0,0) e creo un array vuoto di pHits
pEvent::pEvent(): TObject(),
fM(0),
fVertex(pPoint()),
fHits(NULL){
    fCounter += 1;
    fHits = new TClonesArray("pHit", 100);
}

// Standard Constructor
// metto molteplicità a zero, vertice a (0,0,0) e creo un array vuoto di pHits
pEvent::pEvent(pPoint *vert, int mult): TObject(),
fM(mult),
fVertex(*vert),
fHits(NULL){
    fCounter += 1;
    fHits = new TClonesArray("pHit", 100);
}

