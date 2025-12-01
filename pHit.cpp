#include "pHit.h"
#include "TMath.h"
using TMath::ATan2;
using TMath::Pi;

ClassImp(pHit)

// Default constructor
pHit::pHit(): pPoint(), fId(), fLayer(), fPhi()  {

}

// Standard constructor 1
pHit::pHit(pPoint *pto, Layer layer, int numPart, TString evID): pPoint(*pto), fLayer(layer)
{
    fId = evID + Form("-p%d", numPart);
    fPhi = ATan2(GetY(), GetX()); //tra -pi e pi
    // porta di nuovo tra 0 e 2*pi (per comodità)
    if (fPhi < 0){
        fPhi = fPhi + 2*Pi();
    }

}

// Standard constructor 2
pHit::pHit(double z, double phi, Layer layer, int numPart, TString evID): pPoint(), fLayer(layer), fPhi(phi)
{
    fId = evID + Form("-p%d", numPart);
    double r = GetR();
    SetX(r*cos(phi));
    SetY(r*sin(phi));
    SetZ(z); 
}

// Copy constructor
pHit::pHit(const pHit &source): pPoint(source), fId(source.fId), fLayer(source.fLayer), fPhi(source.fPhi) {
}

void pHit::operator=(const pHit &source)
{
    if (this != &source){
        pPoint::operator=(source);
        fId = source.fId;
        fLayer = source.fLayer;
        fPhi = source.fPhi;
    }
}

// Destructor
pHit::~pHit() {

}

// Setter
void pHit::SetPhi(double phi)
{
    if (phi > 2*acos(-1.)){
        phi = phi - 2*acos(-1);
    }
    else if ( phi < 0.){
        phi = phi + 2*acos(-1);
    }
    fPhi = phi;
    static double r = 0.0;
    r = GetR();
    SetX(r*cos(phi));
    SetY(r*sin(phi));
}

// Getter
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
