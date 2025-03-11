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

void pHit::operator=(const pHit &source)
{
    if (this != &source){
    pPoint::operator=(source);
    fId = source.fId;
    fLayer = source.fLayer;
  }
}

// Destructor
pHit::~pHit() {

}

void pHit::SetPhi(double phi)
{
    static double r = 0.0;
    r = GetR();
    SetX(r*cos(phi));
    SetY(r*sin(phi));
}

double pHit::GetR()
{
    static double R = 0.0;
    Layer lay = GetLayer();
    if (lay == Layer::L1){
        R = 40;
    }
    else if (lay == Layer::L2){
        R = 70;
    }
    else{
        R = 30; 
    }
    return R;
}

double pHit::GetPhi() const
{
    return ATan(GetY()/GetX());
}
