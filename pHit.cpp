#include "pHit.h"
#include "TMath.h"
using std::to_string;
using TMath::ATan;

ClassImp(pHit)

// Default constructor
pHit::pHit(): pPoint(), fId(), fLayer()  {

}

// Standard constructor
pHit::pHit(double x, double y, double z, Layer layer, int index, int count): pPoint(x,y,z), fLayer(layer) {
    fId = "e" + to_string(count) + "-p" + to_string(index);
}

// Copy constructor
pHit::pHit(const pHit &source): pPoint(source), fId(source.fId), fLayer(source.fLayer) {

}

// Destructor
pHit::~pHit() {

}

double pHit::GetPhi() const {
    return ATan(GetY()/GetX());
}
